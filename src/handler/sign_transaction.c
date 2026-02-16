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

static void sign_transaction_init(void)
{
    explicit_bzero(&G_context, sizeof(G_context));
}

static int sign_root_tx(buffer_t *cdata)
{
    int status = 0;

    sign_transaction_init();

    // Extract bip32 path
    if (!buffer_read_u8(cdata, &G_context.bip32_path_len)
        || !buffer_read_bip32_path(
            cdata, G_context.bip32_path, (size_t) G_context.bip32_path_len)) {
        return io_send_sw(SW_WRONG_DATA_LENGTH);
    }
    account_generate_keys(G_context.bip32_path, G_context.bip32_path_len, &G_context.account);

    // Bypass intent length
    cdata->offset += 2;
    cdata->ptr += cdata->offset;
    cdata->size -= cdata->offset;
    cdata->offset = 0;

    // Extract intent
    tx_extract_intent(cdata);
    G_context.sign_transaction_datas.prepared_request.is_root = true;
    G_context.nested_call_count
        = G_context.sign_transaction_datas.prepared_request.nested_call_count;

    G_context.signing_state = SIGNING_STATE_INTENT;

    // Parse intent
    tx_parse(&G_context.sign_transaction_datas, &G_context.tx);

    // Display & sign transaction
    ui_display_transaction();

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
        return io_send_sw(SW_BAD_STATE);
    }

    explicit_bzero(&G_context.sign_transaction_datas.prepared_request, sizeof(prepared_request_t));
    G_context.sign_transaction_datas.prepared_request.is_root = false;

    // Bypass prepared request length
    cdata->offset += 2;
    cdata->ptr += cdata->offset;
    cdata->size -= cdata->offset;
    cdata->offset = 0;

    // Extract prepared request
    tx_extract_prepared_request(cdata, &G_context.sign_transaction_datas.prepared_request);
    G_context.sign_transaction_datas.prepared_request.is_root = false;

    if (G_context.sign_transaction_datas.prepared_request.nested_call_count) {
        return io_send_sw(SWO_INCORRECT_DATA);
    }

    // Sign
    sign_prepared_request(&G_context.account, &G_context.sign_transaction_datas.prepared_request);

    status = helper_send_response_sign_transaction();

    G_context.nested_call_offset++;
    if (G_context.nested_call_offset == G_context.nested_call_count) {
        G_context.fees_waiting_time_ms = 0;
        G_context.signing_state        = SIGNING_STATE_WAIT_FEES;
    }

    return status;
}

static int sign_fee_tx(buffer_t *cdata)
{
    int status = 0;

    if (G_context.signing_state != SIGNING_STATE_WAIT_FEES) {
        return io_send_sw(SW_BAD_STATE);
    }
    explicit_bzero(&G_context.sign_transaction_datas.prepared_request, sizeof(prepared_request_t));

    // Bypass prepared request length
    cdata->offset += 2;
    cdata->ptr += cdata->offset;
    cdata->size -= cdata->offset;
    cdata->offset = 0;

    // Extract prepared request
    tx_extract_prepared_request(cdata, &G_context.sign_transaction_datas.prepared_request);
    G_context.sign_transaction_datas.prepared_request.is_root = true;

    if (G_context.sign_transaction_datas.prepared_request.nested_call_count) {
        return io_send_sw(SWO_INCORRECT_DATA);
    }

    G_context.signing_state = SIGNING_STATE_FEES;

    // Parse fees
    tx_parse(&G_context.sign_transaction_datas, &G_context.tx);

    // Check fees limit
    if ((G_context.tx.fee.base_fee > G_context.sign_transaction_datas.max_base_fee)
        || (G_context.tx.fee.priority_fee > G_context.sign_transaction_datas.max_priority_fee)) {
        return io_send_sw(SWO_INCORRECT_DATA);
    }

    // Display & sign fees
    ui_display_transaction();

    return status;
}

int handler_sign_transaction(buffer_t *cdata, uint8_t mode, bool more)
{
    int status = 0;
    UNUSED(more);
    switch (mode) {
        case SIGN_MODE_ROOT:
            status = sign_root_tx(cdata);
            break;

        case SIGN_MODE_NESTED_CALL:
            status = sign_nested_call_tx(cdata);
            break;

        case SIGN_MODE_FEE:
            status = sign_fee_tx(cdata);
            break;

        default:
            return io_send_sw(SW_WRONG_P1P2);
            break;
    }

    return status;
}