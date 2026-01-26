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

static void print_signature_data(sign_transaction_datas_t *data)
{
    PRINTF("max_base_fee      : %d\n", data->max_base_fee);
    PRINTF("max_priority_fee  : %d\n", data->max_priority_fee);
    PRINTF("fee_function_name : %s\n", data->fee_function_name);
    PRINTF("fee_program_id    : %s\n", data->fee_program_id);
    PRINTF("\n");
    PRINTF("is_root       : %d\n", data->root_prepared_request.is_root);
    PRINTF("network_id    : %d\n", data->root_prepared_request.network_id);
    PRINTF("program_id    : %s\n", data->root_prepared_request.program_id);
    PRINTF("function_name : %s\n", data->root_prepared_request.function_name);
    PRINTF("inputs_count   : %d\n", data->root_prepared_request.inputs_count);
    for (int i = 0; i < data->root_prepared_request.inputs_count; i++) {
        PRINTF("%d - type[%d]  : ", i, data->root_prepared_request.inputs[i].type_length);
        for (int j = 0; j < data->root_prepared_request.inputs[i].type_length; j++) {
            PRINTF("%02x", data->root_prepared_request.inputs[i].type[j]);
        }
        PRINTF("\n");
        PRINTF("%d - value[%d] : ", i, data->root_prepared_request.inputs[i].value_length);
        for (int j = 0; j < data->root_prepared_request.inputs[i].value_length; j++) {
            PRINTF("%02x", data->root_prepared_request.inputs[i].value[j]);
        }
        PRINTF("\n");
    }
}

// **** Prepared Request TLV parser ****

// Calbacks
static bool get_network_id(const tlv_data_t *data, prepared_request_t *cookie)
{
    return get_uint16_t_from_tlv_data(data, &cookie->network_id);
}

static bool get_program_id(const tlv_data_t *data, prepared_request_t *cookie)
{
    buffer_t buff;
    if (!get_buffer_from_tlv_data(data, &buff, 1, 64)) {
        return false;
    }
    cookie->program_id_length = buff.size;
    cookie->program_id        = (char *) buff.ptr;
    return true;
}

static bool get_function_name(const tlv_data_t *data, prepared_request_t *cookie)
{
    buffer_t buff;
    if (!get_buffer_from_tlv_data(data, &buff, 1, 32)) {
        return false;
    }
    cookie->function_name_length = buff.size;
    cookie->function_name        = (char *) buff.ptr;
    return true;
}

static bool get_input_count(const tlv_data_t *data, prepared_request_t *cookie)
{
    return get_uint8_t_from_tlv_data(data, &cookie->inputs_count);
}

static bool get_input_value(const tlv_data_t *data, prepared_request_t *cookie)
{
    buffer_t buff;
    if (!get_buffer_from_tlv_data(data, &buff, 1, 128)) {
        return false;
    }
    input_t *input      = &cookie->inputs[cookie->inputs_value_offset++];
    input->value_length = buff.size;
    input->value        = (uint8_t *) buff.ptr;
    return true;
}

static bool get_input_type(const tlv_data_t *data, prepared_request_t *cookie)
{
    buffer_t buff;
    if (!get_buffer_from_tlv_data(data, &buff, 1, 128)) {
        return false;
    }
    input_t *input     = &cookie->inputs[cookie->inputs_type_offset++];
    input->type_length = buff.size;
    input->type        = (uint8_t *) buff.ptr;
    return true;
}

#define PREPARED_REQUEST_TLV_TAGS(X)                                                   \
    X(0x02, TAG_PREPARED_REQUEST_VERSION, NULL, ENFORCE_UNIQUE_TAG)                    \
    X(0x5a, TAG_PREPARED_REQUEST_IS_ROOT, NULL, ENFORCE_UNIQUE_TAG)                    \
    X(0x5b, TAG_PREPARED_REQUEST_NETWORK_ID, get_network_id, ENFORCE_UNIQUE_TAG)       \
    X(0x45, TAG_PREPARED_REQUEST_PROGRAM_ID, get_program_id, ENFORCE_UNIQUE_TAG)       \
    X(0x46, TAG_PREPARED_REQUEST_FUNCTION_NAME, get_function_name, ENFORCE_UNIQUE_TAG) \
    X(0x47, TAG_PREPARED_REQUEST_INPUT_COUNT, get_input_count, ENFORCE_UNIQUE_TAG)     \
    X(0x48, TAG_PREPARED_REQUEST_INPUT_VALUE, get_input_value, ALLOW_MULTIPLE_TAG)     \
    X(0x49, TAG_PREPARED_REQUEST_INPUT_TYPE, get_input_type, ALLOW_MULTIPLE_TAG)       \
    X(0x4a, TAG_PREPARED_REQUEST_NESTED_CALL_COUNT, NULL, ENFORCE_UNIQUE_TAG)          \
    X(0x4c, TAG_PREPARED_REQUEST_RECORD_COMM_COUNT, NULL, ENFORCE_UNIQUE_TAG)          \
    X(0x4d, TAG_PREPARED_REQUEST_RECORD_COMM, NULL, ENFORCE_UNIQUE_TAG)

DEFINE_TLV_PARSER(PREPARED_REQUEST_TLV_TAGS, NULL, prepared_request_tlv_parser)

static int extract_prepared_request(const buffer_t *cdata, prepared_request_t *prepared_request)
{
    TLV_reception_t received_tags;

    explicit_bzero(prepared_request, sizeof(prepared_request_t));
    if (!prepared_request_tlv_parser(cdata, prepared_request, &received_tags)) {
        return -1;
    }
    return 0;
}

// **** Intent TLV parser ****
// Calbacks
static bool get_max_base_fee(const tlv_data_t *data, sign_transaction_datas_t *cookie)
{
    PRINTF("get_max_base_fee\n");
    return get_uint32_t_from_tlv_data(data, &cookie->max_base_fee);
}
static bool get_max_priority_fee(const tlv_data_t *data, sign_transaction_datas_t *cookie)
{
    PRINTF("get_max_priority_fee\n");
    return get_uint32_t_from_tlv_data(data, &cookie->max_priority_fee);
}

static bool get_fee_function_name(const tlv_data_t *data, sign_transaction_datas_t *cookie)
{
    PRINTF("get_fee_function_name\n");
    buffer_t buff;
    if (!get_buffer_from_tlv_data(data, &buff, 1, 32)) {
        return false;
    }
    cookie->fee_function_name_length = buff.size;
    memcpy(cookie->fee_function_name, buff.ptr, buff.size);
    return true;
}

static bool get_fee_program_id(const tlv_data_t *data, sign_transaction_datas_t *cookie)
{
    PRINTF("get_fee_program_id\n");
    buffer_t buff;
    if (!get_buffer_from_tlv_data(data, &buff, 1, 64)) {
        return false;
    }
    cookie->fee_program_id_length = buff.size;
    memcpy(cookie->fee_program_id, buff.ptr, buff.size);
    return true;
}

static bool get_request(const tlv_data_t *data, sign_transaction_datas_t *cookie)
{
    PRINTF("get_request\n");
    cookie->root_prepared_request.is_root = true;
    extract_prepared_request(&data->value, &cookie->root_prepared_request);
    return true;
}

#define INTENT_TLV_TAGS(X)                                                           \
    X(0x02, TAG_INTENT_VERSION, NULL, ENFORCE_UNIQUE_TAG)                            \
    X(0x40, TAG_INTENT_MAX_BASE_FEE, get_max_base_fee, ENFORCE_UNIQUE_TAG)           \
    X(0x41, TAG_INTENT_MAX_PRIORITY_FEE, get_max_priority_fee, ENFORCE_UNIQUE_TAG)   \
    X(0x42, TAG_INTENT_FEE_FUNCTION_NAME, get_fee_function_name, ENFORCE_UNIQUE_TAG) \
    X(0x43, TAG_INTENT_FEE_PROGRAM_ID, get_fee_program_id, ENFORCE_UNIQUE_TAG)       \
    X(0x44, TAG_INTENT_REQUEST, get_request, ENFORCE_UNIQUE_TAG)

DEFINE_TLV_PARSER(INTENT_TLV_TAGS, NULL, intent_tlv_parser)

static int extract_intent(buffer_t *cdata)
{
    TLV_reception_t received_tags;

    explicit_bzero(&G_context.sign_transaction_datas, sizeof(G_context.sign_transaction_datas));
    if (!intent_tlv_parser(cdata, &G_context.sign_transaction_datas, &received_tags)) {
        return -1;
    }
    return 0;
}

void sign_transaction_init(void)
{
    explicit_bzero(&G_context, sizeof(G_context));
}

int handler_sign_transaction(buffer_t *cdata, uint8_t mode, bool more)
{
    UNUSED(more);
    switch (mode) {
        case SIGN_MODE_ROOT:
            sign_transaction_init();
            // Extract bip32 path
            if (!buffer_read_u8(cdata, &G_context.bip32_path_len)
                || !buffer_read_bip32_path(
                    cdata, G_context.bip32_path, (size_t) G_context.bip32_path_len)) {
                return io_send_sw(SW_WRONG_DATA_LENGTH);
            }
            account_t *a = account_generate_keys(G_context.bip32_path, G_context.bip32_path_len);

            // Extract intent
            cdata->offset += 4;
            cdata->ptr += cdata->offset;
            cdata->size -= cdata->offset;
            cdata->offset = 0;
            extract_intent(cdata);
            print_signature_data(&G_context.sign_transaction_datas);

            sign_prepared_request(a, &G_context.sign_transaction_datas.root_prepared_request);
            return io_send_sw(SW_WRONG_DATA_LENGTH);

            break;

        case SIGN_MODE_NESTED_CALL:
            break;

        case SIGN_MODE_FEES:
            break;

        default:
            return io_send_sw(SW_WRONG_P1P2);
            break;
    }

    return 0;
}