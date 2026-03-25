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
#include "globals.h"

#include "bech32.h"
#include "account.h"
#include "tx_types.h"
#include "tx.h"

typedef struct {
    const char *string;
    uint8_t     string_length;
    tx_type_e   tx_type;
    uint8_t     input_count;
    int (*parse_fn)(sign_transaction_datas_t *, tx_t *);
} program_infos_t;

static int get_u64(input_t *input, bool is_private, uint64_t *value);
static int get_address(input_t *input, bool is_private, char address[ADDRESS_LEN + 1]);

static int parse_transfer_public(sign_transaction_datas_t *data, tx_t *tx);
static int parse_transfer_private(sign_transaction_datas_t *data, tx_t *tx);
static int parse_transfer_public_to_private(sign_transaction_datas_t *data, tx_t *tx);
static int parse_transfer_private_to_public(sign_transaction_datas_t *data, tx_t *tx);
static int parse_fee_public(sign_transaction_datas_t *data, tx_t *tx);
static int parse_fee_private(sign_transaction_datas_t *data, tx_t *tx);

static const program_infos_t program_infos[6] = {
    {
     .string        = "transfer_public",
     .string_length = 15,
     .tx_type       = TX_TRANSFER,
     .input_count   = 2,
     .parse_fn      = parse_transfer_public,
     },
    {
     .string        = "transfer_private",
     .string_length = 16,
     .tx_type       = TX_TRANSFER,
     .input_count   = 3,
     .parse_fn      = parse_transfer_private,
     },
    {
     .string        = "transfer_public_to_private",
     .string_length = 26,
     .tx_type       = TX_TRANSFER,
     .input_count   = 2,
     .parse_fn      = parse_transfer_public_to_private,
     },
    {
     .string        = "transfer_private_to_public",
     .string_length = 26,
     .tx_type       = TX_TRANSFER,
     .input_count   = 3,
     .parse_fn      = parse_transfer_private_to_public,
     },
    {
     .string        = "fee_public",
     .string_length = 10,
     .tx_type       = TX_FEE,
     .input_count   = 3,
     .parse_fn      = parse_fee_public,
     },
    {
     .string        = "fee_private",
     .string_length = 11,
     .tx_type       = TX_FEE,
     .input_count   = 4,
     .parse_fn      = parse_fee_private,
     },
};

static int get_u64(input_t *input, bool is_private, uint64_t *value)
{
    int status = 0;

    if ((input->type_length != 3) || (input->value_length != 8)) {
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
    for (int i = 0; i < 8; i++) {
        *value <<= 8;
        *value += input->value[7 - i];
    }

    return status;
}

static int get_address(input_t *input, bool is_private, char address[ADDRESS_LEN + 1])
{
    int status = 0;

    if ((input->type_length != 3) || (input->value_length != 32)) {
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

    uint8_t data[64];
    size_t  datalen = 0;

    memset(address, 0, ADDRESS_LEN + 1);
    if ((status = bech32_convert_bits(data, &datalen, sizeof(data), 5, input->value, 32, 8, 1))
        < 0) {
        return status;
    }
    status = bech32_encode(address, ADDRESS_PREFIX, data, datalen, BECH32_ENCODING_BECH32M);

    return status;
}

static int parse_transfer_public(sign_transaction_datas_t *data, tx_t *tx)
{
    PRINTF("parse_transfer_public\n");
    tx->transfer.type = TX_TRANSFER_PUBLIC;
    int status = get_address(&data->prepared_request.inputs[0], false, tx->transfer.address_to);
    if (status == 0) {
        status = get_u64(&data->prepared_request.inputs[1], false, &tx->transfer.amount);
    }

    return status;
}

static int parse_transfer_private(sign_transaction_datas_t *data, tx_t *tx)
{
    tx->transfer.type = TX_TRANSFER_PRIVATE;
    int status = get_address(&data->prepared_request.inputs[1], true, tx->transfer.address_to);
    if (status == 0) {
        status = get_u64(&data->prepared_request.inputs[2], true, &tx->transfer.amount);
    }

    return status;
}

static int parse_transfer_public_to_private(sign_transaction_datas_t *data, tx_t *tx)
{
    tx->transfer.type = TX_TRANSFER_PUBLIC_TO_PRIVATE;
    int status = get_address(&data->prepared_request.inputs[0], true, tx->transfer.address_to);
    if (status == 0) {
        status = get_u64(&data->prepared_request.inputs[1], false, &tx->transfer.amount);
    }

    return status;
}

static int parse_transfer_private_to_public(sign_transaction_datas_t *data, tx_t *tx)
{
    tx->transfer.type = TX_TRANSFER_PRIVATE_TO_PUBLIC;
    int status = get_address(&data->prepared_request.inputs[1], false, tx->transfer.address_to);
    if (status == 0) {
        status = get_u64(&data->prepared_request.inputs[2], false, &tx->transfer.amount);
    }

    return status;
}

static int parse_fee_public(sign_transaction_datas_t *data, tx_t *tx)
{
    tx->fee.type = TX_FEE_PUBLIC;
    int status   = get_u64(&data->prepared_request.inputs[0], false, &tx->fee.base_fee);
    if (status == 0) {
        status = get_u64(&data->prepared_request.inputs[1], false, &tx->fee.priority_fee);
    }

    return status;
}

static int parse_fee_private(sign_transaction_datas_t *data, tx_t *tx)
{
    tx->fee.type = TX_FEE_PRIVATE;
    int status   = get_u64(&data->prepared_request.inputs[1], false, &tx->fee.base_fee);
    if (status == 0) {
        status = get_u64(&data->prepared_request.inputs[2], false, &tx->fee.priority_fee);
    }

    return status;
}

int tx_parse(sign_transaction_datas_t *data, tx_t *tx)
{
    if (memcmp(data->prepared_request.program_id,
               "credits.aleo",
               data->prepared_request.program_id_length)) {
        // Unknown transaction
        return -1;
    }
    tx->type = TX_UNKNOWN;
    for (uint8_t i = 0; i < 6; i++) {
        if (data->prepared_request.function_name_length != program_infos[i].string_length) {
            continue;
        }

        if (!memcmp(data->prepared_request.function_name,
                    PIC(program_infos[i].string),
                    data->prepared_request.function_name_length)) {
            tx->type = program_infos[i].tx_type;
            if (program_infos[i].input_count != data->prepared_request.inputs_count) {
                return -1;
            }
            int (*parse_fn)(sign_transaction_datas_t *, tx_t *) = PIC(program_infos[i].parse_fn);
            if (parse_fn(data, tx) < 0) {
                return -1;
            }
            break;
        }
    }

    if (tx->type == TX_UNKNOWN) {
        return -1;
    }

    return 0;
}
