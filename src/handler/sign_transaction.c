/*****************************************************************************
 *   Ledger App ALEO.
 *   (c) 2025 Ledger SAS.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *****************************************************************************/

#include <stdint.h>   // uint*_t
#include <stdbool.h>  // bool
#include <stddef.h>   // size_t
#include <string.h>   // memset, explicit_bzero

#include "os.h"
#include "cx.h"
#include "ledger_assert.h"
#include "nbgl_use_case.h"
#include "menu.h"
#include "io.h"
#include "buffer.h"
#include "crypto_helpers.h"

#include "globals.h"
#include "types.h"
#include "sw.h"
#include "display.h"
#include "tlv_library.h"
#include "send_response.h"
#include "account.h"
#include "signature.h"
#include "tx.h"

static uint8_t  rx_transaction_array[1024 * 8];
static buffer_t apdu_rx_buffer;

static int sign_root_tx(buffer_t *cdata)
{
    int status = 0;

    explicit_bzero(&G_context, sizeof(G_context));

    // Extract bip32 path
    if (!buffer_read_u8(cdata, &G_context.bip32_path_len)
        || !buffer_read_bip32_path(
            cdata, G_context.bip32_path, (size_t) G_context.bip32_path_len)) {
        return io_send_sw(SWO_WRONG_DATA_LENGTH);
    }
    status
        = account_generate_keys(G_context.bip32_path, G_context.bip32_path_len, &G_context.account);
    if (status < 0) {
        explicit_bzero(&G_context.account, sizeof(G_context.account));
#ifndef FUZZ
        nbgl_useCaseReviewStatus(STATUS_TYPE_TRANSACTION_REJECTED, ui_menu_main);
#endif  // FUZZ
        return io_send_sw(SW_DISPLAY_BIP32_PATH_FAIL);
    }

    // Bypass intent length
    cdata->offset += 2;
    cdata->ptr += cdata->offset;
    cdata->size -= cdata->offset;
    cdata->offset = 0;

    // Extract intent
    if ((status = tx_extract_intent(cdata)) < 0) {
        explicit_bzero(&G_context.account, sizeof(G_context.account));
#ifndef FUZZ
        nbgl_useCaseReviewStatus(STATUS_TYPE_TRANSACTION_REJECTED, ui_menu_main);
#endif  // FUZZ
        goto end;
    }
    G_context.sign_transaction_datas.prepared_request.is_root = true;
    G_context.nested_call_count
        = G_context.sign_transaction_datas.prepared_request.nested_call_count;

    // Parse intent
    if ((status = tx_parse(&G_context.sign_transaction_datas, &G_context.tx)) < 0) {
        explicit_bzero(&G_context.account, sizeof(G_context.account));
#ifndef FUZZ
        nbgl_useCaseReviewStatus(STATUS_TYPE_TRANSACTION_REJECTED, ui_menu_main);
#endif  // FUZZ
        goto end;
    }

    G_context.signing_state = SIGNING_STATE_INTENT;

    // Display & sign transaction
    if ((status = ui_display_transaction()) < 0) {
        explicit_bzero(&G_context.account, sizeof(G_context.account));
#ifndef FUZZ
        nbgl_useCaseReviewStatus(STATUS_TYPE_TRANSACTION_REJECTED, ui_menu_main);
#endif  // FUZZ
    }

end:
    return status;
}

static int sign_nested_call_tx(buffer_t *cdata)
{
    int status = 0;

    if (G_context.signing_state == SIGNING_STATE_WAIT_NESTED_CALL) {
        G_context.nested_call_offset = 0;
        G_context.signing_state      = SIGNING_STATE_NESTED_CALL;
    }

    if (G_context.signing_state != SIGNING_STATE_NESTED_CALL) {
        PRINTF("sign_nested_call_tx wrong state : %d\n", G_context.signing_state);
        explicit_bzero(&G_context.account, sizeof(G_context.account));
        return io_send_sw(SWO_CONDITIONS_NOT_SATISFIED);
    }

    explicit_bzero(&G_context.sign_transaction_datas.prepared_request, sizeof(prepared_request_t));
    G_context.sign_transaction_datas.prepared_request.is_root = false;

    // Bypass prepared request length
    cdata->offset += 2;
    cdata->ptr += cdata->offset;
    cdata->size -= cdata->offset;
    cdata->offset = 0;

    // Extract prepared request
    if ((status
         = tx_extract_prepared_request(cdata, &G_context.sign_transaction_datas.prepared_request))
        < 0) {
        goto end;
    }
    G_context.sign_transaction_datas.prepared_request.is_root = false;

    if (G_context.sign_transaction_datas.prepared_request.nested_call_count) {
        status = -1;
        goto end;
    }

    // Sign
    if ((status = sign_prepared_request(&G_context.account,
                                        &G_context.sign_transaction_datas.prepared_request))
        < 0) {
        goto end;
    }

    if ((status = helper_send_response_sign_transaction()) < 0) {
        goto end;
    }

    G_context.nested_call_offset++;
    if (G_context.nested_call_offset >= G_context.nested_call_count) {
        if ((G_context.sign_transaction_datas.max_base_fee != 0)
            || (G_context.sign_transaction_datas.max_priority_fee != 0)) {
            G_context.fees_waiting_time_ms = 0;
            G_context.signing_state        = SIGNING_STATE_WAIT_FEES;
#ifndef FUZZ
            nbgl_useCaseSpinner("Calculating fees");
#endif  // FUZZ
        }
        else {
            nbgl_useCaseReviewStatus(STATUS_TYPE_TRANSACTION_SIGNED, ui_menu_main);
            G_context.signing_state = SIGNING_STATE_WAIT_INTENT;
        }
    }

end:
    return status;
}

static int sign_fee_tx(buffer_t *cdata)
{
    int status = 0;

    if (G_context.signing_state != SIGNING_STATE_WAIT_FEES) {
        PRINTF("sign_fee_tx wrong state : %d\n", G_context.signing_state);
        explicit_bzero(&G_context.account, sizeof(G_context.account));
        return io_send_sw(SWO_CONDITIONS_NOT_SATISFIED);
    }
    explicit_bzero(&G_context.sign_transaction_datas.prepared_request, sizeof(prepared_request_t));

    // Bypass prepared request length
    cdata->offset += 2;
    cdata->ptr += cdata->offset;
    cdata->size -= cdata->offset;
    cdata->offset = 0;

    // Extract prepared request
    if ((status
         = tx_extract_prepared_request(cdata, &G_context.sign_transaction_datas.prepared_request))
        < 0) {
        goto end;
    }
    G_context.sign_transaction_datas.prepared_request.is_root = true;

    if (G_context.sign_transaction_datas.prepared_request.nested_call_count) {
        status = -1;
        goto end;
    }

    G_context.signing_state = SIGNING_STATE_FEES;

    // Parse fees
    if ((status = tx_parse(&G_context.sign_transaction_datas, &G_context.tx)) < 0) {
        goto end;
    }

    // Check fees limit
    if ((G_context.tx.fee.base_fee > G_context.sign_transaction_datas.max_base_fee)
        || (G_context.tx.fee.priority_fee > G_context.sign_transaction_datas.max_priority_fee)) {
        status = -1;
        goto end;
    }

    // Display & sign fees
    status = ui_display_transaction();

end:
    return status;
}

int handler_sign_transaction(buffer_t *cdata, uint8_t mode, bool next_chunk)
{
    int status = 0;

    LEDGER_ASSERT(cdata != NULL, "NULL cdata");

    if (!cdata->size) {
        // Reject empty data
        return io_send_sw(SWO_WRONG_DATA_LENGTH);
    }

    // Handle fragmentation
    if (!next_chunk) {
        size_t length_offset = 0;
        if (mode == SIGN_MODE_ROOT) {
            length_offset = 1 + cdata->ptr[0] * 4;
        }
        if (cdata->size < length_offset + 2) {
            return io_send_sw(SWO_WRONG_DATA_LENGTH);
        }

        uint16_t declared_len = U2BE(cdata->ptr, length_offset);
        if ((declared_len + 2 + length_offset) > sizeof(rx_transaction_array)) {
            apdu_rx_buffer.size = 0;
            return io_send_sw(SWO_INSUFFICIENT_MEMORY);
        }
        apdu_rx_buffer.offset = 0;
        apdu_rx_buffer.size   = length_offset + 2 + declared_len;
    }
    else if (!apdu_rx_buffer.size) {
        // Before a next we do need a begin
        return io_send_sw(SWO_CONDITIONS_NOT_SATISFIED);
    }

    if (apdu_rx_buffer.offset + cdata->size > apdu_rx_buffer.size) {
        return io_send_sw(SWO_WRONG_DATA_LENGTH);
    }

    memmove((void *) &apdu_rx_buffer.ptr[apdu_rx_buffer.offset], cdata->ptr, cdata->size);
    apdu_rx_buffer.offset += cdata->size;

    if (apdu_rx_buffer.offset < apdu_rx_buffer.size) {
        // Wait next chunk
        return io_send_sw(SWO_SUCCESS);
    }

    apdu_rx_buffer.offset = 0;
    switch (mode) {
        case SIGN_MODE_ROOT:
            status = sign_root_tx(&apdu_rx_buffer);
            break;

        case SIGN_MODE_NESTED_CALL:
            status = sign_nested_call_tx(&apdu_rx_buffer);
            break;

        case SIGN_MODE_FEE:
            status = sign_fee_tx(&apdu_rx_buffer);
            break;

        default:
            return io_send_sw(SWO_INCORRECT_P1_P2);
            break;
    }

    if (status < 0) {
        return io_send_sw(SWO_INCORRECT_DATA);
    }

    return status;
}

void sign_transaction_init(void)
{
    apdu_rx_buffer.ptr    = rx_transaction_array;
    apdu_rx_buffer.size   = 0;
    apdu_rx_buffer.offset = 0;
}
