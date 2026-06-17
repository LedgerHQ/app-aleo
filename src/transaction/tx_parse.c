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
#include "ledger_assert.h"
#include "globals.h"

#include "bech32.h"
#include "account.h"

#include "db.h"
#include "tx_types.h"
#include "tx.h"

#define U64_TYPE_LENGTH      (3)
#define U64_VALUE_LENGTH     (8)
#define U128_TYPE_LENGTH     (3)
#define U128_VALUE_LENGTH    (2 * U64_VALUE_LENGTH)
#define ADDRESS_TYPE_LENGTH  (3)
#define ADDRESS_VALUE_LENGTH sizeof(field_t)

static const token_display_info_t aleo_display_info
    = {.type = TOKEN_TYPE_ALEO, .ticker = "ALEO", .decimals = 6};

static int get_u64(input_t *input, bool is_private, uint64_t *value);
static int get_u128(input_t *input, bool is_private, u128_t *value);
static int get_address(input_t *input, bool is_private, char address[ADDRESS_LEN + 1]);

static int parse_aleo_transfer_public(sign_transaction_datas_t *data, tx_t *tx);
static int parse_aleo_transfer_public_to_private(sign_transaction_datas_t *data, tx_t *tx);
static int parse_aleo_transfer_private(sign_transaction_datas_t *data, tx_t *tx);
static int parse_aleo_transfer_private_to_public(sign_transaction_datas_t *data, tx_t *tx);

static int parse_aleo_batch_transfer_private(sign_transaction_datas_t *data, tx_t *tx);
static int parse_aleo_batch_transfer_private_to_public(sign_transaction_datas_t *data, tx_t *tx);

static int parse_token_transfer_public(sign_transaction_datas_t *data, tx_t *tx);
static int parse_token_transfer_public_to_private(sign_transaction_datas_t *data, tx_t *tx);
static int parse_token_transfer_private(sign_transaction_datas_t *data, tx_t *tx);
static int parse_token_transfer_private_to_public(sign_transaction_datas_t *data, tx_t *tx);

static int parse_fee_public(sign_transaction_datas_t *data, tx_t *tx);
static int parse_fee_private(sign_transaction_datas_t *data, tx_t *tx);

static int get_u64(input_t *input, bool is_private, uint64_t *value)
{
    if ((input->type_length != U64_TYPE_LENGTH) || (input->value_length != U64_VALUE_LENGTH)) {
        return -1;
    }
    else if (is_private && (input->type[0] != INPUT_ID_PRIVATE)) {
        return -1;
    }
    else if (!is_private && (input->type[0] != INPUT_ID_PUBLIC)) {
        return -1;
    }
    else if ((input->type[1] != INPUT_VALUE_TYPE_PLAINTEXT_LITERAL)
             || (input->type[2] != PLAINTEXT_TYPE_LITERAL_U64)) {
        return -1;
    }
    *value = 0;
    for (int i = 0; i < (int) U64_VALUE_LENGTH; i++) {
        *value <<= 8;
        *value += input->value[U64_VALUE_LENGTH - 1 - i];
    }

    return 0;
}

static int get_u128(input_t *input, bool is_private, u128_t *value)
{
    if ((input->type_length != U128_TYPE_LENGTH) || (input->value_length != U128_VALUE_LENGTH)) {
        return -1;
    }
    else if (is_private && (input->type[0] != INPUT_ID_PRIVATE)) {
        return -1;
    }
    else if (!is_private && (input->type[0] != INPUT_ID_PUBLIC)) {
        return -1;
    }
    else if ((input->type[1] != INPUT_VALUE_TYPE_PLAINTEXT_LITERAL)
             || (input->type[2] != PLAINTEXT_TYPE_LITERAL_U128)) {
        return -1;
    }
    value->high = 0;
    for (int i = 0; i < (int) U64_VALUE_LENGTH; i++) {
        value->high <<= 8;
        value->high += input->value[U128_VALUE_LENGTH - 1 - i];
    }
    value->low = 0;
    for (int i = 0; i < (int) U64_VALUE_LENGTH; i++) {
        value->low <<= 8;
        value->low += input->value[U64_VALUE_LENGTH - 1 - i];
    }

    return 0;
}

static int get_address(input_t *input, bool is_private, char address[ADDRESS_LEN + 1])
{
    int status = -1;

    if ((input->type_length != ADDRESS_TYPE_LENGTH)
        || (input->value_length != ADDRESS_VALUE_LENGTH)) {
        return -1;
    }
    else if (is_private && (input->type[0] != INPUT_ID_PRIVATE)) {
        return -1;
    }
    else if (!is_private && (input->type[0] != INPUT_ID_PUBLIC)) {
        return -1;
    }
    else if ((input->type[1] != INPUT_VALUE_TYPE_PLAINTEXT_LITERAL)
             || (input->type[2] != PLAINTEXT_TYPE_LITERAL_ADDRESS)) {
        return -1;
    }

    uint8_t data[ADDRESS_LEN + 1];
    size_t  datalen = 0;

    memset(address, 0, ADDRESS_LEN + 1);
    if ((status = bech32_convert_bits(
             data, &datalen, sizeof(data), 5, input->value, ADDRESS_VALUE_LENGTH, 8, 1))
        < 0) {
        return status;
    }
    status = bech32_encode(address, ADDRESS_PREFIX, data, datalen, BECH32_ENCODING_BECH32M);

    return status;
}

static int parse_aleo_transfer_public(sign_transaction_datas_t *data, tx_t *tx)
{
    int status = get_address(&data->prepared_request.inputs[0], false, tx->transfer.address_to);
    if (status == 0) {
        status = get_u64(&data->prepared_request.inputs[1], false, &tx->transfer.amount.low);
        tx->transfer.amount.high = 0;
    }
    tx->transfer.token_info = PIC(&aleo_display_info);

    return status;
}

static int parse_aleo_transfer_public_to_private(sign_transaction_datas_t *data, tx_t *tx)
{
    int status = get_address(&data->prepared_request.inputs[0], true, tx->transfer.address_to);
    if (status == 0) {
        status = get_u64(&data->prepared_request.inputs[1], false, &tx->transfer.amount.low);
        tx->transfer.amount.high = 0;
    }
    tx->transfer.token_info = PIC(&aleo_display_info);

    return status;
}

static int parse_aleo_transfer_private(sign_transaction_datas_t *data, tx_t *tx)
{
    int status = get_address(&data->prepared_request.inputs[1], true, tx->transfer.address_to);
    if (status == 0) {
        status = get_u64(&data->prepared_request.inputs[2], true, &tx->transfer.amount.low);
        tx->transfer.amount.high = 0;
    }
    tx->transfer.token_info = PIC(&aleo_display_info);

    return status;
}

static int parse_aleo_transfer_private_to_public(sign_transaction_datas_t *data, tx_t *tx)
{
    int status = get_address(&data->prepared_request.inputs[1], false, tx->transfer.address_to);
    if (status == 0) {
        status = get_u64(&data->prepared_request.inputs[2], false, &tx->transfer.amount.low);
        tx->transfer.amount.high = 0;
    }
    tx->transfer.token_info = PIC(&aleo_display_info);

    return status;
}

static int parse_aleo_batch_transfer_private(sign_transaction_datas_t *data, tx_t *tx)
{
    uint8_t inputs_count = data->prepared_request.inputs_count;
    int     status       = get_address(
        &data->prepared_request.inputs[inputs_count - 2], true, tx->transfer.address_to);
    if (status == 0) {
        status = get_u64(
            &data->prepared_request.inputs[inputs_count - 1], true, &tx->transfer.amount.low);
        tx->transfer.amount.high = 0;
    }
    tx->transfer.token_info = PIC(&aleo_display_info);

    return status;
}

static int parse_aleo_batch_transfer_private_to_public(sign_transaction_datas_t *data, tx_t *tx)
{
    uint8_t inputs_count = data->prepared_request.inputs_count;
    int     status       = get_u64(
        &data->prepared_request.inputs[inputs_count - 1], false, &tx->transfer.amount.low);
    tx->transfer.amount.high = 0;
    memset(tx->transfer.address_to, 0, sizeof(tx->transfer.address_to));
    tx->transfer.token_info = PIC(&aleo_display_info);

    return status;
}

static int parse_token_transfer_public(sign_transaction_datas_t *data, tx_t *tx)
{
    int status = db_get_token_display_info(data->prepared_request.program_id,
                                           data->prepared_request.program_id_length,
                                           NULL,
                                           &tx->transfer.token_info);
    if (status < 0) {
        return status;
    }

    if (tx->transfer.token_info->type == TOKEN_TYPE_ARC20) {
        status = get_address(&data->prepared_request.inputs[0], false, tx->transfer.address_to);
        if (status == 0) {
            status = get_u128(&data->prepared_request.inputs[1], false, &tx->transfer.amount);
        }
    }
    else if (tx->transfer.token_info->type == TOKEN_TYPE_ARC22) {
        status = get_address(&data->prepared_request.inputs[0], false, tx->transfer.address_to);
        if (status == 0) {
            status = get_u128(&data->prepared_request.inputs[1], false, &tx->transfer.amount);
        }
    }

    return status;
}

static int parse_token_transfer_public_to_private(sign_transaction_datas_t *data, tx_t *tx)
{
    int status = db_get_token_display_info(data->prepared_request.program_id,
                                           data->prepared_request.program_id_length,
                                           NULL,
                                           &tx->transfer.token_info);
    if (status < 0) {
        return status;
    }

    if (tx->transfer.token_info->type == TOKEN_TYPE_ARC20) {
        status = get_address(&data->prepared_request.inputs[0], true, tx->transfer.address_to);
        if (status == 0) {
            status = get_u128(&data->prepared_request.inputs[1], false, &tx->transfer.amount);
        }
    }
    else if (tx->transfer.token_info->type == TOKEN_TYPE_ARC22) {
        status = get_address(&data->prepared_request.inputs[0], true, tx->transfer.address_to);
        if (status == 0) {
            status = get_u128(&data->prepared_request.inputs[1], false, &tx->transfer.amount);
        }
    }

    return status;
}

static int parse_token_transfer_private(sign_transaction_datas_t *data, tx_t *tx)
{
    int status = db_get_token_display_info(data->prepared_request.program_id,
                                           data->prepared_request.program_id_length,
                                           NULL,
                                           &tx->transfer.token_info);
    if (status < 0) {
        return status;
    }

    if (tx->transfer.token_info->type == TOKEN_TYPE_ARC20) {
        status = get_address(&data->prepared_request.inputs[1], true, tx->transfer.address_to);
        if (status == 0) {
            status = get_u128(&data->prepared_request.inputs[2], true, &tx->transfer.amount);
        }
    }
    else if (tx->transfer.token_info->type == TOKEN_TYPE_ARC22) {
        status = get_address(&data->prepared_request.inputs[0], true, tx->transfer.address_to);
        if (status == 0) {
            status = get_u128(&data->prepared_request.inputs[1], true, &tx->transfer.amount);
        }
    }

    return status;
}

static int parse_token_transfer_private_to_public(sign_transaction_datas_t *data, tx_t *tx)
{
    int status = db_get_token_display_info(data->prepared_request.program_id,
                                           data->prepared_request.program_id_length,
                                           NULL,
                                           &tx->transfer.token_info);
    if (status < 0) {
        return status;
    }

    if (tx->transfer.token_info->type == TOKEN_TYPE_ARC20) {
        status = get_address(&data->prepared_request.inputs[1], false, tx->transfer.address_to);
        if (status == 0) {
            status = get_u128(&data->prepared_request.inputs[2], false, &tx->transfer.amount);
        }
    }
    else if (tx->transfer.token_info->type == TOKEN_TYPE_ARC22) {
        status = get_address(&data->prepared_request.inputs[0], false, tx->transfer.address_to);
        if (status == 0) {
            status = get_u128(&data->prepared_request.inputs[1], false, &tx->transfer.amount);
        }
    }

    return status;
}

static int parse_fee_public(sign_transaction_datas_t *data, tx_t *tx)
{
    int status = get_u64(&data->prepared_request.inputs[0], false, &tx->fee.base_fee);
    if (status == 0) {
        status = get_u64(&data->prepared_request.inputs[1], false, &tx->fee.priority_fee);
    }

    return status;
}

static int parse_fee_private(sign_transaction_datas_t *data, tx_t *tx)
{
    int status = get_u64(&data->prepared_request.inputs[1], false, &tx->fee.base_fee);
    if (status == 0) {
        status = get_u64(&data->prepared_request.inputs[2], false, &tx->fee.priority_fee);
    }

    return status;
}

int tx_parse(sign_transaction_datas_t *data, tx_t *tx)
{
    LEDGER_ASSERT(data != NULL, "NULL data");
    LEDGER_ASSERT(tx != NULL, "NULL tx");

    function_parameters_t *function_parameters = NULL;

    explicit_bzero(tx, sizeof(tx_t));

    if (!data->prepared_request.program_id) {
        return -1;
    }
    if (!data->prepared_request.function_name) {
        return -1;
    }

    int status = db_get_function_parameters(data->prepared_request.program_id,
                                            data->prepared_request.program_id_length,
                                            data->prepared_request.function_name,
                                            data->prepared_request.function_name_length,
                                            &function_parameters);

    if (status < 0) {
        return status;
    }

    if (function_parameters->input_count != data->prepared_request.inputs_count) {
        return -1;
    }

    tx->type = function_parameters->tx_type;

    switch (tx->type) {
        case TX_ALEO_TRANSFER_PUBLIC:
            return parse_aleo_transfer_public(data, tx);

        case TX_ALEO_TRANSFER_PUBLIC_TO_PRIVATE:
            return parse_aleo_transfer_public_to_private(data, tx);

        case TX_ALEO_TRANSFER_PRIVATE:
            return parse_aleo_transfer_private(data, tx);

        case TX_ALEO_TRANSFER_PRIVATE_TO_PUBLIC:
            return parse_aleo_transfer_private_to_public(data, tx);

        case TX_ALEO_TRANSFER_BATCH_PRIVATE:
            return parse_aleo_batch_transfer_private(data, tx);

        case TX_ALEO_TRANSFER_BATCH_PRIVATE_TO_PUBLIC:
            return parse_aleo_batch_transfer_private_to_public(data, tx);

        case TX_TOKEN_TRANSFER_PUBLIC:
            return parse_token_transfer_public(data, tx);

        case TX_TOKEN_TRANSFER_PUBLIC_TO_PRIVATE:
            return parse_token_transfer_public_to_private(data, tx);

        case TX_TOKEN_TRANSFER_PRIVATE:
            return parse_token_transfer_private(data, tx);

        case TX_TOKEN_TRANSFER_PRIVATE_TO_PUBLIC:
            return parse_token_transfer_private_to_public(data, tx);

        case TX_FEE_PUBLIC:
            return parse_fee_public(data, tx);

        case TX_FEE_PRIVATE:
            return parse_fee_private(data, tx);

        default:
            break;
    }

    return -1;
}
