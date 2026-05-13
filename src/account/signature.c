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

#include <stdint.h>   // uint*_t
#include <stddef.h>   // size_t
#include <stdbool.h>  // bool

#include "os.h"
#include "ledger_assert.h"
#include "globals.h"
#include "group.h"
#include "poseidon.h"
#include "bhp_1024.h"
#include "bits.h"
#include "field.h"

#include "tx_types.h"
#include "nbgl_use_case.h"
#include "signature.h"

#define HASH_INPUT_MAX_LENGTH     (16)
#define MESSAGE_MAX_LENGTH        (64)
#define PLAINTEXT_FIELDS_MAX_SIZE (4)
#define BIT_BUFFER_MAX_LENGTH     (128)

const field_t SERIAL_NUMBER_DOMAIN = {
    .big.u64 = {0xf993d63c8bc0a4ad, 0xe22f8477532c74cc, 0x367fe5e2c9f74b96, 0x0309ea7b80fce820}
};

const field_t ENCRYPTION_DOMAIN = {
    .big.u64 = {0xa2c83ef0b5c793c0, 0xb74e109906bb65ce, 0x3309f1ccb9800822, 0x0a61fb5f71a4d5cf}
};

static field_t hash_input[HASH_INPUT_MAX_LENGTH];
static field_t message[MESSAGE_MAX_LENGTH];
static size_t  message_length;
static field_t plaintext_fields[PLAINTEXT_FIELDS_MAX_SIZE];
static field_t randomizer_fields[PLAINTEXT_FIELDS_MAX_SIZE];

static uint8_t bit_buffer[BIT_BUFFER_MAX_LENGTH];
static char    text_buffer[32];

static int add_field_to_message(field_t *field)
{
    if (message_length >= MESSAGE_MAX_LENGTH) {
        return -1;
    }
    memcpy(&message[message_length++], field, sizeof(field_t));
    return 0;
}

static int hash_public_input(prepared_request_t *request, uint8_t input_index)
{
    _Static_assert(HASH_INPUT_MAX_LENGTH >= 13, "hash_input size won't fit");
    int      status           = 0;
    uint8_t  hash_input_index = 8;
    input_t *input            = &request->inputs[input_index];
    field_t  hash;

    memset(hash_input, 0, sizeof(hash_input));
    memcpy(&hash_input[hash_input_index++], &request->function_id, sizeof(field_t));
    if (input->type[1] == INPUT_VALUE_TYPE_PLAINTEXT) {
        if ((input->type[2] == PLAINTEXT_TYPE_LITERAL_ADDRESS)
            || (input->type[2] == PLAINTEXT_TYPE_LITERAL_FIELD)) {
            PRINTF("PLAINTEXT_TYPE_LITERAL_ADDRESS/FIELD\n");
            int bit_length = bits_from_plaintext(input->value,
                                                 &input->type[1],
                                                 FIELD_MODULUS_BITS,
                                                 bit_buffer,
                                                 BIT_BUFFER_MAX_LENGTH * 8);
            if (bit_length < 0) {
                return bit_length;
            }
            if (bit_length >= BIT_BUFFER_MAX_LENGTH * 8) {
                return -1;
            }
            bits_add_single(bit_buffer, (uint16_t) bit_length, true);
            bit_length += 1;
            hash_input_index += field_from_bits(
                bit_buffer, (uint16_t) bit_length, &hash_input[hash_input_index], 2);
            field_print_array(&hash_input[hash_input_index - 2], 2);
        }
        else if (input->type[2] == PLAINTEXT_TYPE_LITERAL_U64) {
            PRINTF("PLAINTEXT_TYPE_LITERAL_U64\n");
            int bit_length = bits_from_plaintext(
                input->value, &input->type[1], 64, bit_buffer, BIT_BUFFER_MAX_LENGTH * 8);
            if (bit_length < 0) {
                return bit_length;
            }
            if (bit_length >= BIT_BUFFER_MAX_LENGTH * 8) {
                return -1;
            }
            bits_add_single(bit_buffer, (uint16_t) bit_length, true);
            bit_length += 1;
            hash_input_index += field_from_bits(
                bit_buffer, (uint16_t) bit_length, &hash_input[hash_input_index], 1);
            field_print_array(&hash_input[hash_input_index - 1], 1);
        }
        else {
            PRINTF("Public plaintext type unsupported (%d)\n", input->type[2]);
            return -1;
        }
    }
    else {
        PRINTF("Public input value type unsupported (%d)\n", input->type[1]);
        return -1;
    }
    memcpy(&hash_input[hash_input_index++], &request->tcm, sizeof(field_t));
    field_from_int(&hash_input[hash_input_index++], input_index);
    if ((status = hash_psd8(hash_input, hash_input_index, &hash)) < 0) {
        return status;
    }
    PRINTF("hash : ");
    field_println(&hash);

    return add_field_to_message(&hash);
}

static int hash_private_input(prepared_request_t *request, uint8_t input_index)
{
    int      status           = 0;
    size_t   hash_input_index = 0;
    input_t *input            = &request->inputs[input_index];
    uint8_t  num_randomizers  = 0;
    field_t  input_view_key;
    field_t  hash;

    // Compute the input view key as `Hash(function ID || tvk || index)
    _Static_assert(HASH_INPUT_MAX_LENGTH >= 7, "hash_input size won't fit");
    memset(hash_input, 0, sizeof(hash_input));
    memcpy(&hash_input[4], &request->function_id, sizeof(field_t));
    memcpy(&hash_input[5], &request->tvk, sizeof(field_t));
    field_from_int(&hash_input[6], input_index);
    if ((status = hash_psd4(hash_input, 4 + 3, &input_view_key)) < 0) {
        return status;
    }
    PRINTF("input_view_key : ");
    field_println(&input_view_key);

    if (input->type[1] == INPUT_VALUE_TYPE_PLAINTEXT) {
        if ((input->type[2] == PLAINTEXT_TYPE_LITERAL_ADDRESS)
            || (input->type[2] == PLAINTEXT_TYPE_LITERAL_FIELD)) {
            PRINTF("PLAINTEXT_TYPE_LITERAL_ADDRESS/FIELD\n");
            int bit_length = bits_from_plaintext(input->value,
                                                 &input->type[1],
                                                 FIELD_MODULUS_BITS,
                                                 bit_buffer,
                                                 BIT_BUFFER_MAX_LENGTH * 8);
            if (bit_length < 0) {
                return bit_length;
            }
            if (bit_length >= BIT_BUFFER_MAX_LENGTH * 8) {
                return -1;
            }
            bits_add_single(bit_buffer, (uint16_t) bit_length, true);
            bit_length += 1;
            num_randomizers = field_from_bits(
                bit_buffer, (uint16_t) bit_length, plaintext_fields, PLAINTEXT_FIELDS_MAX_SIZE);
        }
        else if (input->type[2] == PLAINTEXT_TYPE_LITERAL_U64) {
            PRINTF("PLAINTEXT_TYPE_LITERAL_U64\n");
            int bit_length = bits_from_plaintext(
                input->value, &input->type[1], 64, bit_buffer, BIT_BUFFER_MAX_LENGTH * 8);
            if (bit_length < 0) {
                return bit_length;
            }
            if (bit_length >= BIT_BUFFER_MAX_LENGTH * 8) {
                return -1;
            }
            bits_add_single(bit_buffer, (uint16_t) bit_length, true);
            bit_length += 1;
            num_randomizers = field_from_bits(
                bit_buffer, (uint16_t) bit_length, plaintext_fields, PLAINTEXT_FIELDS_MAX_SIZE);
        }
        else {
            PRINTF("Private plaintext type unsupported (%d)\n", input->type[2]);
            return -1;
        }
        PRINTF("plaintext_fields : \n");
        field_print_array(plaintext_fields, num_randomizers);
    }
    else {
        PRINTF("Private input value type unsupported (%d)\n", input->type[1]);
        return -1;
    }

    // Compute randomizers
    _Static_assert(HASH_INPUT_MAX_LENGTH >= 10, "hash_input size won't fit");
    memset(hash_input, 0, sizeof(hash_input));
    memcpy(&hash_input[8], &ENCRYPTION_DOMAIN, sizeof(field_t));
    memcpy(&hash_input[9], &input_view_key, sizeof(field_t));
    if ((status = hash_many_psd8(hash_input, 10, randomizer_fields, num_randomizers)) < 0) {
        return status;
    }
    PRINTF("randomizer_fields : \n");
    field_print_array(randomizer_fields, num_randomizers);

    // Compute the ciphertext
    _Static_assert(HASH_INPUT_MAX_LENGTH >= (8 + PLAINTEXT_FIELDS_MAX_SIZE),
                   "hash_input size won't fit");
    memset(hash_input, 0, sizeof(hash_input));
    for (hash_input_index = 8; hash_input_index < 8 + num_randomizers; hash_input_index++) {
        memcpy(&hash_input[hash_input_index],
               &plaintext_fields[hash_input_index - 8],
               sizeof(field_t));
        field_add_assign(&hash_input[hash_input_index], &randomizer_fields[hash_input_index - 8]);
    }
    PRINTF("cyphertext_fields : \n");
    field_print_array(&hash_input[8], num_randomizers);

    // Hash the ciphertext to a field element
    if ((status = hash_psd8(hash_input, hash_input_index, &hash)) < 0) {
        return status;
    }
    PRINTF("hash : ");
    field_println(&hash);

    return add_field_to_message(&hash);
}

static int hash_record_input(account_t *account, prepared_request_t *request, uint8_t input_index)
{
    int          status = 0;
    bigint_256_t s;
    input_t     *input = &request->inputs[input_index];
    field_t      commitment;
    group_t      h;
    group_t      h_r;
    field_t      tag;

    // Extract 'commitment'
    bn_reverse(input->value);
    bn_to_big_int(input->value, &s);
    field_from_big_int(&commitment, &s);
    PRINTF("commitment : ");
    field_println(&commitment);

    // Extract 'h' x coordinate
    bn_reverse(&input->value[32]);
    bn_to_big_int(&input->value[32], &s);
    field_from_big_int(&h.x, &s);

    // Extract 'h' y coordinate
    bn_reverse(&input->value[64]);
    bn_to_big_int(&input->value[64], &s);
    field_from_big_int(&h.y, &s);
    if ((status = add_field_to_message(&h.x)) < 0) {
        return status;
    }
    PRINTF("h : ");
    group_println(&h);

    // Compute `h_r` as `r * h`
    if ((status = group_scalar_multiply(&h, &request->r, &h_r)) < 0) {
        return status;
    }
    if ((status = add_field_to_message(&h_r.x)) < 0) {
        return status;
    }
    PRINTF("h_r : ");
    group_println(&h_r);

    // Compute `gamma` as `sk_sig * h
    if ((status = group_scalar_multiply(
             &h, &account->private_key.sk_sig, &request->gammas[request->gammas_count]))
        < 0) {
        return status;
    }
    if ((status = add_field_to_message(&request->gammas[request->gammas_count].x)) < 0) {
        return status;
    }
    PRINTF("gamma : ");
    group_println(&request->gammas[request->gammas_count]);
    request->gammas_count++;

    // Compute the tag
    _Static_assert(HASH_INPUT_MAX_LENGTH >= 4, "hash_input size won't fit");
    memset(hash_input, 0, sizeof(hash_input));
    memcpy(&hash_input[2], &account->graph_key, sizeof(field_t));
    memcpy(&hash_input[3], &commitment, sizeof(field_t));
    if ((status = hash_psd2(hash_input, 2 + 2, &tag)) < 0) {
        return status;
    }
    PRINTF("tag : ");
    field_println(&tag);
    return add_field_to_message(&tag);
}

static int hash_external_record_input(prepared_request_t *request, uint8_t input_index)
{
    int          status           = 0;
    size_t       hash_input_index = 8;
    input_t     *input            = &request->inputs[input_index];
    bigint_256_t s;
    field_t      hash;

    if ((input->value_length == 0) || (input->value_length % 32 != 0)) {
        return -1;
    }
    uint8_t num_fields = input->value_length / 32;
    if (num_fields
        > (HASH_INPUT_MAX_LENGTH - 11)) { /* 11 = 8 capacity + function_id + tvk + index */
        return -1;
    }

    _Static_assert(HASH_INPUT_MAX_LENGTH >= 14, "hash_input size won't fit for external record");
    memset(hash_input, 0, sizeof(hash_input));

    /* Preimage: [function_id || record_fields... || tvk || index] */
    memcpy(&hash_input[hash_input_index++], &request->function_id, sizeof(field_t));

    for (uint8_t i = 0; i < num_fields; i++) {
        bn_reverse(&input->value[i * 32]);
        bn_to_big_int(&input->value[i * 32], &s);
        field_from_big_int(&hash_input[hash_input_index++], &s);
    }

    memcpy(&hash_input[hash_input_index++], &request->tvk, sizeof(field_t));
    field_from_int(&hash_input[hash_input_index++], input_index);

    if ((status = hash_psd8(hash_input, hash_input_index, &hash)) < 0) {
        return status;
    }
    PRINTF("external_record_hash : ");
    field_println(&hash);

    return add_field_to_message(&hash);
}

static int prepare_inputs(account_t *account, prepared_request_t *request)
{
    int     status      = 0;
    uint8_t input_index = 0;

    for (input_index = 0; input_index < request->inputs_count; input_index++) {
        input_t *input = &request->inputs[input_index];
        switch (input->type[0]) {
            case INPUT_ID_CONSTANT:
                PRINTF("Input constant not supported\n");
                status = -1;
                break;

            case INPUT_ID_PUBLIC:
                status = hash_public_input(request, input_index);
                break;

            case INPUT_ID_PRIVATE:
                status = hash_private_input(request, input_index);
                break;

            case INPUT_ID_RECORD:
                status = hash_record_input(account, request, input_index);
                break;

            case INPUT_ID_EXTERNAL_RECORD:
                status = hash_external_record_input(request, input_index);
                break;

            default:
                status = -1;
                break;
        }
        if (status < 0) {
            return status;
        }
    }

    return status;
}

static void display_progression(uint8_t step)
{
    const char *text         = NULL;
    uint8_t     current_step = step;
    uint8_t     total_step   = (1 + G_context.nested_call_count) * 5;

    if (G_context.signing_state == SIGNING_STATE_FEES) {
        text = "Signing transaction";
    }
    else if (G_context.signing_state == SIGNING_STATE_NESTED_CALL) {
        text         = "Prepare Tx";
        current_step = step + ((1 + G_context.nested_call_offset) * 5);
    }
    else {
        text = "Prepare Tx";
    }
    // %2 is an ugly hack to force spinner text update by changing the pointer value
    snprintf(&text_buffer[step % 2],
             sizeof(text_buffer) - (step % 2),
             "%s %d/%d",
             text,
             current_step,
             total_step);
#ifndef FUZZ
    nbgl_useCaseSpinner(&text_buffer[step % 2]);
#endif  // FUZZ
}

int sign_prepared_request(account_t *account, prepared_request_t *request)
{
    int      status = 0;
    field_t *is_root;
    group_t  g_temp;
    field_t  nonce;

    LEDGER_ASSERT(account != NULL, "NULL account");
    LEDGER_ASSERT(request != NULL, "NULL request");

    if ((status = field_random(&nonce)) < 0) {
        goto end;
    }
    PRINTF("nonce : ");
    field_println(&nonce);

    display_progression(1);

    if (request->r_hint) {
        // Use preprocessed 'r' from the request
        bigint_256_t s;
        bn_reverse(request->r_hint);
        bn_to_big_int(request->r_hint, &s);
        scalar_from_big_int(&request->r, &s);
        PRINTF("r_hint : ");
        scalar_println(&request->r);
    }
    else {
        // Compute a `r` as `HashToScalar(sk_sig || nonce)`. Note: This is the transition secret key
        // `tsk`.
        _Static_assert(HASH_INPUT_MAX_LENGTH >= 7, "hash_input size won't fit");
        memset(hash_input, 0, sizeof(hash_input));
        memcpy(&hash_input[4], &SERIAL_NUMBER_DOMAIN, sizeof(field_t));
        scalar_to_field(&account->private_key.sk_sig, &hash_input[5]);
        memcpy(&hash_input[6], &nonce, sizeof(field_t));
        if ((status = hash_to_scalar_psd4(hash_input, 4 + 3, &request->r)) < 0) {
            goto end;
        }
        PRINTF("r : ");
        scalar_println(&request->r);
    }
    display_progression(2);

    // Compute `g_r` as `r * G`. Note: This is the transition public key `tpk`.
    if ((status = group_g_scalar_multiply(&request->r, &request->tpk)) < 0) {
        goto end;
    }
    PRINTF("tpk : ");
    group_println(&request->tpk);

    // Compute the transition view key `tvk` as `r * signer`.
    if ((status = group_scalar_multiply(&account->address, &request->r, &g_temp)) < 0) {
        goto end;
    }
    memcpy(&request->tvk, &g_temp.x, sizeof(field_t));
    PRINTF("tvk : ");
    field_println(&request->tvk);

    // Compute the transition commitment `tcm` as `Hash(tvk)`.
    _Static_assert(HASH_INPUT_MAX_LENGTH >= 3, "hash_input size won't fit");
    memset(hash_input, 0, sizeof(hash_input));
    memcpy(&hash_input[2], &request->tvk, sizeof(field_t));
    if ((status = hash_psd2(hash_input, 2 + 1, &request->tcm)) < 0) {
        goto end;
    }
    PRINTF("tcm : ");
    field_println(&request->tcm);

    // Compute 'is_root' as a field element.
    if (request->is_root) {
        is_root = (field_t *) &FIELD_ONE;
    }
    else {
        is_root = (field_t *) &FIELD_ZERO;
    }

    // Compute the function ID.
    if ((status = bhp_1024_hash_function_id(request)) < 0) {
        goto end;
    }
    PRINTF("function_id : ");
    field_println(&request->function_id);

    // Construct the hash input as `(r * G, pk_sig, pr_sig, signer, [tvk, tcm, function ID, is_root,
    // program checksum?, input IDs])`.
    _Static_assert(MESSAGE_MAX_LENGTH >= 16, "message size won't fit");
    message_length = 8;
    memset(message, 0, sizeof(message));
    if ((status = add_field_to_message(&request->tpk.x)) < 0) {
        goto end;
    }
    if ((status = add_field_to_message(&account->compute_key.pk_sig.x)) < 0) {
        goto end;
    }
    if ((status = add_field_to_message(&account->compute_key.pr_sig.x)) < 0) {
        goto end;
    }
    if ((status = add_field_to_message(&account->address.x)) < 0) {
        goto end;
    }
    if ((status = add_field_to_message(&request->tvk)) < 0) {
        goto end;
    }
    if ((status = add_field_to_message(&request->tcm)) < 0) {
        goto end;
    }
    if ((status = add_field_to_message(&request->function_id)) < 0) {
        goto end;
    }
    if ((status = add_field_to_message(is_root)) < 0) {
        goto end;
    }
    if (request->program_checksum) {
        bigint_256_t s;
        field_t      program_checksum;
        bn_reverse(request->program_checksum);
        bn_to_big_int(request->program_checksum, &s);
        field_from_big_int(&program_checksum, &s);
        if ((status = add_field_to_message(&program_checksum)) < 0) {
            goto end;
        }
        PRINTF("program_checksum : ");
        field_println(&program_checksum);
    }

    display_progression(3);
    // Prepare the inputs.
    if ((status = prepare_inputs(account, request)) < 0) {
        goto end;
    }
    PRINTF("message : \n");
    field_print_array(message, message_length);

    display_progression(4);
    // Compute challenge
    if ((status = hash_to_scalar_psd8(message, message_length, &request->challenge)) < 0) {
        goto end;
    }
    PRINTF("challenge : ");
    scalar_println(&request->challenge);

    // Compute response
    scalar_t s_res;
    memcpy(&s_res, &request->challenge, sizeof(scalar_t));
    scalar_mul_assign(&s_res, &account->private_key.sk_sig);
    memcpy(&request->response, &request->r, sizeof(scalar_t));
    scalar_sub_assign(&request->response, &s_res);
    PRINTF("response : ");
    scalar_println(&request->response);
    display_progression(5);

end:
    explicit_bzero(hash_input, sizeof(hash_input));
    explicit_bzero(message, sizeof(message));

    return status;
}
