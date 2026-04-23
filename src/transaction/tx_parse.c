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

#include "db_program_function.h"
#include "tx_types.h"
#include "tx.h"

#define U64_TYPE_LENGTH      (3)
#define U64_VALUE_LENGTH     sizeof(uint64_t)
#define ADDRESS_TYPE_LENGTH  (3)
#define ADDRESS_VALUE_LENGTH sizeof(field_t)

typedef struct {
    const char *string;
    uint8_t     string_length;
    tx_type_e   tx_type;
    uint8_t     input_count;
    int (*parse_fn)(sign_transaction_datas_t *, tx_t *);
} program_infos_t;

static int get_u64(input_t *input, bool is_private, uint64_t *value);
static int get_address(input_t *input, bool is_private, char address[ADDRESS_LEN + 1]);

static int parse_aleo_transfer_public(sign_transaction_datas_t *data, tx_t *tx);
static int parse_aleo_transfer_private(sign_transaction_datas_t *data, tx_t *tx);
static int parse_aleo_batch_transfer_private(sign_transaction_datas_t *data, tx_t *tx);
static int parse_aleo_transfer_public_to_private(sign_transaction_datas_t *data, tx_t *tx);
static int parse_aleo_transfer_private_to_public(sign_transaction_datas_t *data, tx_t *tx);
static int parse_fee_public(sign_transaction_datas_t *data, tx_t *tx);
static int parse_fee_private(sign_transaction_datas_t *data, tx_t *tx);

static int get_u64(input_t *input, bool is_private, uint64_t *value)
{
    int status = 0;

    if ((input->type_length != U64_TYPE_LENGTH) || (input->value_length != U64_VALUE_LENGTH)) {
        return -1;
    }
    else if (is_private && (input->type[0] != INPUT_ID_PRIVATE)) {
        return -1;
    }
    else if (!is_private && (input->type[0] != INPUT_ID_PUBLIC)) {
        return -1;
    }
    else if ((input->type[1] != INPUT_VALUE_TYPE_PLAINTEXT)
             || (input->type[2] != PLAINTEXT_TYPE_LITERAL_U64)) {
        return -1;
    }
    *value = 0;
    for (int i = 0; i < (int) U64_VALUE_LENGTH; i++) {
        *value <<= 8;
        *value += input->value[U64_VALUE_LENGTH - 1 - i];
    }

    return status;
}

static int get_address(input_t *input, bool is_private, char address[ADDRESS_LEN + 1])
{
    int status = 0;

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
    else if ((input->type[1] != INPUT_VALUE_TYPE_PLAINTEXT)
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
        status = get_u64(&data->prepared_request.inputs[1], false, &tx->transfer.amount);
    }

    return status;
}

static int parse_aleo_transfer_private(sign_transaction_datas_t *data, tx_t *tx)
{
    int status = get_address(&data->prepared_request.inputs[1], true, tx->transfer.address_to);
    if (status == 0) {
        status = get_u64(&data->prepared_request.inputs[2], true, &tx->transfer.amount);
    }

    return status;
}

static int parse_aleo_batch_transfer_private(sign_transaction_datas_t *data, tx_t *tx)
{
    uint8_t inputs_count = data->prepared_request.inputs_count;
    int     status       = get_address(
        &data->prepared_request.inputs[inputs_count - 2], true, tx->transfer.address_to);
    if (status == 0) {
        status
            = get_u64(&data->prepared_request.inputs[inputs_count - 1], true, &tx->transfer.amount);
    }

    return status;
}

static int parse_aleo_transfer_public_to_private(sign_transaction_datas_t *data, tx_t *tx)
{
    int status = get_address(&data->prepared_request.inputs[0], true, tx->transfer.address_to);
    if (status == 0) {
        status = get_u64(&data->prepared_request.inputs[1], false, &tx->transfer.amount);
    }

    return status;
}

static int parse_aleo_transfer_private_to_public(sign_transaction_datas_t *data, tx_t *tx)
{
    int status = get_address(&data->prepared_request.inputs[1], false, tx->transfer.address_to);
    if (status == 0) {
        status = get_u64(&data->prepared_request.inputs[2], false, &tx->transfer.amount);
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

    size_t                 index         = 0;
    size_t                 program_index = 0;
    function_parameters_t *functions     = NULL;

    if (!data->prepared_request.program_id) {
        return -1;
    }
    if (!data->prepared_request.function_name) {
        return -1;
    }

    // Find program
    for (index = 0; index < NB_OF_PROGRAMS; index++) {
        if (data->prepared_request.program_id_length
            != strlen(PIC(program_parameters[index].program_id))) {
            continue;
        }
        if (memcmp(data->prepared_request.program_id,
                   PIC(program_parameters[index].program_id),
                   data->prepared_request.program_id_length)) {
            continue;
        }
        break;
    }

    if (index >= NB_OF_PROGRAMS) {
        return -1;
    }

    // Find program's function
    program_index = index;
    functions     = PIC(program_parameters[program_index].functions);

    for (index = 0; index < program_parameters[program_index].nb_of_functions; index++) {
        if (data->prepared_request.function_name_length != strlen(PIC(functions[index].string))) {
            continue;
        }
        if (memcmp(data->prepared_request.function_name,
                   PIC(functions[index].string),
                   data->prepared_request.function_name_length)) {
            continue;
        }
        break;
    }

    if (index >= program_parameters[program_index].nb_of_functions) {
        return -1;
    }

    if (functions[index].input_count != data->prepared_request.inputs_count) {
        return -1;
    }

    tx->type = functions[index].tx_type;

    switch (tx->type) {
        case TX_ALEO_TRANSFER_PUBLIC:
            return parse_aleo_transfer_public(data, tx);

        case TX_ALEO_TRANSFER_PRIVATE:
            return parse_aleo_transfer_private(data, tx);

        case TX_ALEO_TRANSFER_BATCH_PRIVATE:
            return parse_aleo_batch_transfer_private(data, tx);

        case TX_ALEO_TRANSFER_PUBLIC_TO_PRIVATE:
            return parse_aleo_transfer_public_to_private(data, tx);

        case TX_ALEO_TRANSFER_PRIVATE_TO_PUBLIC:
            return parse_aleo_transfer_private_to_public(data, tx);

        case TX_FEE_PUBLIC:
            return parse_fee_public(data, tx);

        case TX_FEE_PRIVATE:
            return parse_fee_private(data, tx);

        default:
            break;
    }

    return -1;
}
