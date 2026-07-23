/*****************************************************************************
 *   Ledger App Aleo.
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

#include <stddef.h>  // size_t
#include <stdint.h>  // uint*_t
#include <string.h>  // memmove

#include "buffer.h"

#include "swap.h"
#include "ledger_assert.h"
#include "send_response.h"
#include "constants.h"
#include "globals.h"
#include "sw.h"
#include "write.h"

#include "group.h"

#define RESPONSE_BUFFER_MAX_LENGTH (500)

static uint8_t response_buffer[RESPONSE_BUFFER_MAX_LENGTH];

static int add_tlv_field(uint8_t *in_buffer, size_t in_buffer_size, uint8_t type, field_t *f)
{
    int          offset = 0;
    bigint_256_t b;

    if (type <= 0x7f) {
        if (in_buffer_size < (1 + 1 + 32)) {
            return -1;
        }
        in_buffer[offset++] = type;
    }
    else {
        if (in_buffer_size < (2 + 1 + 32)) {
            return -1;
        }
        in_buffer[offset++] = 0x81;
        in_buffer[offset++] = type;
    }
    in_buffer[offset++] = 32;

    field_to_big_int(f, &b);
    big_int_to_bn(&b, &in_buffer[offset]);
    bn_reverse(&in_buffer[offset]);
    offset += 32;

    return offset;
}

static int add_tlv_group(uint8_t *in_buffer, size_t in_buffer_size, uint8_t type, group_t *g)
{
    return add_tlv_field(in_buffer, in_buffer_size, type, &g->x);
}

static int add_tlv_uint8(uint8_t *in_buffer, size_t in_buffer_size, uint8_t type, uint8_t u)
{
    int offset = 0;

    if (type <= 0x7f) {
        if (in_buffer_size < (1 + 1 + 1)) {
            return -1;
        }
        in_buffer[offset++] = type;
    }
    else {
        if (in_buffer_size < (2 + 1 + 1)) {
            return -1;
        }
        in_buffer[offset++] = 0x81;
        in_buffer[offset++] = type;
    }
    in_buffer[offset++] = 1;
    in_buffer[offset++] = u;

    return offset;
}

static size_t add_tlv_signature(uint8_t             *in_buffer,
                                size_t               in_buffer_size,
                                uint8_t              type,
                                const scalar_t      *challenge,
                                const scalar_t      *response,
                                const compute_key_t *compute_key)
{
    size_t       offset = 0;
    bigint_256_t b;

    if (type <= 0x7f) {
        if (in_buffer_size < (1 + 1 + 128)) {
            return -1;
        }
        in_buffer[offset++] = type;
    }
    else {
        if (in_buffer_size < (2 + 1 + 128)) {
            return -1;
        }
        in_buffer[offset++] = 0x81;
        in_buffer[offset++] = type;
    }
    in_buffer[offset++] = 128;

    // Challenge
    PRINTF("Challenge\n");
    scalar_to_big_int(challenge, &b);
    big_int_println(&b);
    big_int_to_bn(&b, &in_buffer[offset]);
    bn_print(&in_buffer[offset]);
    bn_reverse(&in_buffer[offset]);
    bn_print(&in_buffer[offset]);
    offset += 32;

    // Response
    scalar_to_big_int(response, &b);
    big_int_to_bn(&b, &in_buffer[offset]);
    bn_reverse(&in_buffer[offset]);
    offset += 32;

    // pk_sig
    field_to_big_int(&compute_key->pk_sig.x, &b);
    big_int_to_bn(&b, &in_buffer[offset]);
    bn_reverse(&in_buffer[offset]);
    offset += 32;

    // pr_sig
    field_to_big_int(&compute_key->pr_sig.x, &b);
    big_int_to_bn(&b, &in_buffer[offset]);
    bn_reverse(&in_buffer[offset]);
    offset += 32;

    return offset;
}

int helper_send_response_get_address(void)
{
    _Static_assert((ADDRESS_LEN + 1) < RESPONSE_BUFFER_MAX_LENGTH,
                   "response_buffer size won't fit");
    size_t offset = 0;

    memset(response_buffer, 0, sizeof(response_buffer));
    response_buffer[offset++] = ADDRESS_LEN;
    memmove(&response_buffer[offset], G_context.address, ADDRESS_LEN);
    offset += ADDRESS_LEN;
    response_buffer[offset] = 0;

    return io_send_response_pointer(response_buffer, offset, SWO_SUCCESS);
}

int helper_send_response_get_view_key(void)
{
    _Static_assert((VIEW_KEY_LEN + 1) < RESPONSE_BUFFER_MAX_LENGTH,
                   "response_buffer size won't fit");
    size_t offset = 0;

    memset(response_buffer, 0, sizeof(response_buffer));
    response_buffer[offset++] = VIEW_KEY_LEN;
    memmove(&response_buffer[offset], G_context.view_key, VIEW_KEY_LEN);
    offset += VIEW_KEY_LEN;
    response_buffer[offset] = 0;

    return io_send_response_pointer(response_buffer, offset, SWO_SUCCESS);
}

int helper_send_response_sign_transaction(void)
{
    _Static_assert(RESPONSE_BUFFER_MAX_LENGTH >= 492, "response_buffer size won't fit");

    size_t offset = 0;
    size_t i      = 0;
    int    len    = 0;

    memset(response_buffer, 0, sizeof(response_buffer));
    // Type
    if ((len = add_tlv_uint8(
             &response_buffer[offset], RESPONSE_BUFFER_MAX_LENGTH - offset, 0x01, 0x2a))  // 3 bytes
        < 0) {
        return len;
    }
    offset += len;
    // Version
    if ((len = add_tlv_uint8(
             &response_buffer[offset], RESPONSE_BUFFER_MAX_LENGTH - offset, 0x02, 0x01))  // 3 bytes
        < 0) {
        return len;
    }
    offset += len;

    // Signature
    if ((len = add_tlv_signature(&response_buffer[offset],
                                 RESPONSE_BUFFER_MAX_LENGTH - offset,
                                 0x15,
                                 &G_context.sign_transaction_datas.prepared_request.challenge,
                                 &G_context.sign_transaction_datas.prepared_request.response,
                                 &G_context.account.compute_key))  // 130 bytes
        < 0) {
        return len;
    }
    offset += len;
    // TVK
    if ((len = add_tlv_field(&response_buffer[offset],
                             RESPONSE_BUFFER_MAX_LENGTH - offset,
                             0xbf,
                             &G_context.sign_transaction_datas.prepared_request.tvk))  // 35 bytes
        < 0) {
        return len;
    }
    offset += len;
    // TPK
    if ((len = add_tlv_group(&response_buffer[offset],
                             RESPONSE_BUFFER_MAX_LENGTH - offset,
                             0xc0,
                             &G_context.sign_transaction_datas.prepared_request.tpk))  // 35 bytes
        < 0) {
        return len;
    }
    offset += len;
    // Gammas count
    if ((len = add_tlv_uint8(
             &response_buffer[offset],
             RESPONSE_BUFFER_MAX_LENGTH - offset,
             0xc1,
             G_context.sign_transaction_datas.prepared_request.gammas_count))  // 4 bytes
        < 0) {
        return len;
    }
    offset += len;

    for (i = 0; i < G_context.sign_transaction_datas.prepared_request.gammas_count;
         i++) {  // 35*8 bytes
        if ((len = add_tlv_group(
                 &response_buffer[offset],
                 RESPONSE_BUFFER_MAX_LENGTH - offset,
                 0xc2,
                 &G_context.sign_transaction_datas.prepared_request.gammas[i]))  // 35 bytes
            < 0) {
            return len;
        }
        offset += len;
    }

    if ((RESPONSE_BUFFER_MAX_LENGTH - offset) < 2) {
        return -1;
    }
    write_u16_be(response_buffer, offset, SWO_SUCCESS);  // 2 bytes
    offset += 2;

    G_swap_response_ready = false;
    if (G_context.signing_state == SIGNING_STATE_FEES) {
        // Signing fees
        G_swap_response_ready = true;
    }
    else if ((G_context.signing_state == SIGNING_STATE_INTENT) && (G_context.nested_call_count == 0)
             && (G_context.sign_transaction_datas.max_base_fee == 0)
             && (G_context.sign_transaction_datas.max_priority_fee == 0)) {
        // No nested calls & no fees to sign
        G_swap_response_ready = true;
    }
    else if ((G_context.signing_state == SIGNING_STATE_NESTED_CALL)
             && ((G_context.nested_call_offset + 1) >= G_context.nested_call_count)
             && (G_context.sign_transaction_datas.max_base_fee == 0)
             && (G_context.sign_transaction_datas.max_priority_fee == 0)) {
        // No more nested calls & no fees to sign
        G_swap_response_ready = true;
    }

    // If we are in swap mode and have validated a TX, we send it and immediately quit
    if (G_called_from_swap && G_swap_response_ready) {
        PRINTF("Swap answer is processed. Send it\n");

        if (io_legacy_apdu_tx(response_buffer, offset) >= 0) {
            *G_swap_signing_return_value_address = 1;
            PRINTF("Returning to Exchange with status %d\n", *G_swap_signing_return_value_address);
            PRINTF("os_lib_end\n");
            os_lib_end();
        }
        else {
            PRINTF("Unrecoverable\n");
#ifndef USE_OS_IO_STACK
            os_io_stop();
#endif  // USE_OS_IO_STACK
            os_sched_exit(-1);
        }
    }

    return io_legacy_apdu_tx(response_buffer, offset);
}

int helper_send_response_get_tvk(field_t *tvk)
{
    _Static_assert(RESPONSE_BUFFER_MAX_LENGTH >= 35, "response_buffer size won't fit");

    size_t       offset = 0;
    bigint_256_t b;

    memset(response_buffer, 0, sizeof(response_buffer));

    response_buffer[offset++] = 32;

    field_to_big_int(tvk, &b);
    big_int_to_bn(&b, &response_buffer[offset]);
    bn_reverse(&response_buffer[offset]);
    offset += 32;

    return io_send_response_pointer(response_buffer, offset, SWO_SUCCESS);
}
