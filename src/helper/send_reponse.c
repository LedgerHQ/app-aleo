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

#include "send_response.h"
#include "constants.h"
#include "globals.h"
#include "sw.h"
#include "write.h"

#include "group.h"

static uint8_t response_buffer[512];

static size_t add_tlv_field(uint8_t *in_buffer, uint8_t type, field_t *f)
{
    size_t       offset = 0;
    bigint_256_t b;

    if (type <= 0x7f) {
        in_buffer[offset++] = type;
    }
    else {
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

static size_t add_tlv_group(uint8_t *in_buffer, uint8_t type, group_t *g)
{
    return add_tlv_field(in_buffer, type, &g->x);
}

static size_t add_tlv_uint8(uint8_t *in_buffer, uint8_t type, uint8_t u)
{
    size_t offset = 0;
    if (type <= 0x7f) {
        in_buffer[offset++] = type;
    }
    else {
        in_buffer[offset++] = 0x81;
        in_buffer[offset++] = type;
    }
    in_buffer[offset++] = 1;
    in_buffer[offset++] = u;

    return offset;
}

static size_t add_tlv_signature(uint8_t             *in_buffer,
                                uint8_t              type,
                                const scalar_t      *challenge,
                                const scalar_t      *response,
                                const compute_key_t *compute_key)
{
    size_t       offset = 0;
    bigint_256_t b;

    if (type <= 0x7f) {
        in_buffer[offset++] = type;
    }
    else {
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
    size_t offset = 0;

    memset(response_buffer, 0, sizeof(response_buffer));
    response_buffer[offset++] = VIEW_KEY_LEN;
    memmove(&response_buffer[offset], G_context.view_key, VIEW_KEY_LEN);
    offset += VIEW_KEY_LEN;
    response_buffer[offset] = 0;

    return io_send_response_pointer(response_buffer, offset, SWO_SUCCESS);
}

int helper_send_response_get_private_key(void)
{
    size_t offset = 0;

    memset(response_buffer, 0, sizeof(response_buffer));
    response_buffer[offset++] = PRIVATE_KEY_LEN;
    memmove(&response_buffer[offset], G_context.private_key, PRIVATE_KEY_LEN);
    offset += PRIVATE_KEY_LEN;
    response_buffer[offset] = 0;

    return io_send_response_pointer(response_buffer, offset, SWO_SUCCESS);
}

int helper_send_response_sign_transaction(void)
{
    size_t offset = 0;
    size_t i      = 0;

    memset(response_buffer, 0, sizeof(response_buffer));
    // Type
    offset += add_tlv_uint8(&response_buffer[offset], 0x01, 0x2a);
    // Version
    offset += add_tlv_uint8(&response_buffer[offset], 0x02, 0x01);
    offset += add_tlv_signature(&response_buffer[offset],
                                0x15,
                                &G_context.sign_transaction_datas.prepared_request.challenge,
                                &G_context.sign_transaction_datas.prepared_request.response,
                                &G_context.account.compute_key);
    // TVK
    offset += add_tlv_field(
        &response_buffer[offset], 0xbf, &G_context.sign_transaction_datas.prepared_request.tvk);
    // TPK
    offset += add_tlv_group(
        &response_buffer[offset], 0xc0, &G_context.sign_transaction_datas.prepared_request.tpk);
    // Gammas count
    offset += add_tlv_uint8(&response_buffer[offset],
                            0xc1,
                            G_context.sign_transaction_datas.prepared_request.gammas_count);

    for (i = 0; i < G_context.sign_transaction_datas.prepared_request.gammas_count; i++) {
        offset += add_tlv_group(&response_buffer[offset],
                                0xc2,
                                &G_context.sign_transaction_datas.prepared_request.gammas[i]);
    }

    write_u16_be(response_buffer, offset, SWO_SUCCESS);
    offset += 2;

    return io_legacy_apdu_tx(response_buffer, offset);
}
