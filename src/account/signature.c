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
#include "globals.h"
#include "group.h"
#include "poseidon.h"
#include "bhp_512.h"
#include "bhp_1024.h"
#include "bits.h"
#include "field.h"

#include "tx_types.h"
#include "nbgl_use_case.h"
#include "signature.h"

const field_t SERIAL_NUMBER_DOMAIN = {
    .big.u64 = {0xf993d63c8bc0a4ad, 0xe22f8477532c74cc, 0x367fe5e2c9f74b96, 0x0309ea7b80fce820}
};

const field_t ENCRYPTION_DOMAIN = {
    .big.u64 = {0xa2c83ef0b5c793c0, 0xb74e109906bb65ce, 0x3309f1ccb9800822, 0x0a61fb5f71a4d5cf}
};

static field_t hash_input[16];
static field_t message[32];
static size_t  message_length;
static field_t plaintext_fields[4];
static field_t randomizer_fields[4];

static uint8_t bit_buffer[128];
static char    text_buffer[32];

static void add_field_to_message(field_t *field)
{
    memcpy(&message[8 + message_length++], field, sizeof(field_t));
}

static void hash_public_input(prepared_request_t *request, uint8_t input_index)
{
    uint8_t  hash_input_index = 0;
    input_t *input            = &request->inputs[input_index];
    field_t  hash;

    memset(hash_input, 0, sizeof(hash_input));
    memcpy(&hash_input[8 + hash_input_index++], &request->function_id, sizeof(field_t));
    if (input->type[1] == 0) {  // literal
        if ((input->type[2] == PLAINTEXT_TYPE_LITERAL_ADDRESS)
            || (input->type[2] == PLAINTEXT_TYPE_LITERAL_FIELD)) {
            PRINTF("PLAINTEXT_TYPE_LITERAL_ADDRESS/FIELD\n");
            uint16_t bit_length = bits_from_plaintext(
                input->value, &input->type[1], FIELD_MODULUS_BITS, bit_buffer);
            bits_add_single(bit_buffer, bit_length, true);
            bit_length += 1;
            hash_input_index
                += field_from_bits(bit_buffer, bit_length, &hash_input[8 + hash_input_index], 2);
            field_print_array(&hash_input[8 + hash_input_index - 2], 2);
        }
        else if (input->type[2] == PLAINTEXT_TYPE_LITERAL_U64) {
            PRINTF("PLAINTEXT_TYPE_LITERAL_U64\n");
            uint16_t bit_length
                = bits_from_plaintext(input->value, &input->type[1], 64, bit_buffer);
            bits_add_single(bit_buffer, bit_length, true);
            bit_length += 1;
            hash_input_index
                += field_from_bits(bit_buffer, bit_length, &hash_input[8 + hash_input_index], 1);
            field_print_array(&hash_input[8 + hash_input_index - 1], 1);
        }
    }
    memcpy(&hash_input[8 + hash_input_index++], &request->tcm, sizeof(field_t));
    field_from_int(&hash_input[8 + hash_input_index++], input_index);
    hash_psd8(hash_input, hash_input_index, &hash);
    add_field_to_message(&hash);
    PRINTF("hash : ");
    field_println(&hash);
}

static void hash_private_input(prepared_request_t *request, uint8_t input_index)
{
    uint8_t  hash_input_index = 0;
    input_t *input            = &request->inputs[input_index];
    uint8_t  num_randomizers  = 0;
    field_t  input_view_key;
    field_t  hash;

    // Compute the input view key as `Hash(function ID || tvk || index)
    memset(hash_input, 0, sizeof(hash_input));
    memcpy(&hash_input[4 + hash_input_index++], &request->function_id, sizeof(field_t));
    memcpy(&hash_input[4 + hash_input_index++], &request->tvk, sizeof(field_t));
    field_from_int(&hash_input[4 + hash_input_index++], input_index);
    hash_psd4(hash_input, hash_input_index, &input_view_key);
    PRINTF("input_view_key : ");
    field_println(&input_view_key);

    if (input->type[1] == 0) {  // literal
        if ((input->type[2] == PLAINTEXT_TYPE_LITERAL_ADDRESS)
            || (input->type[2] == PLAINTEXT_TYPE_LITERAL_FIELD)) {
            PRINTF("PLAINTEXT_TYPE_LITERAL_ADDRESS/FIELD\n");
            uint16_t bit_length = bits_from_plaintext(
                input->value, &input->type[1], FIELD_MODULUS_BITS, bit_buffer);
            bits_add_single(bit_buffer, bit_length, true);
            bit_length += 1;
            num_randomizers = field_from_bits(bit_buffer, bit_length, plaintext_fields, 4);
        }
        else if (input->type[2] == PLAINTEXT_TYPE_LITERAL_U64) {
            PRINTF("PLAINTEXT_TYPE_LITERAL_U64\n");
            uint16_t bit_length
                = bits_from_plaintext(input->value, &input->type[1], 64, bit_buffer);
            bits_add_single(bit_buffer, bit_length, true);
            bit_length += 1;
            num_randomizers = field_from_bits(bit_buffer, bit_length, plaintext_fields, 4);
        }
        PRINTF("plaintext_fields : \n");
        field_print_array(plaintext_fields, num_randomizers);
    }

    // Compute randomizers
    hash_input_index = 0;
    memset(hash_input, 0, sizeof(hash_input));
    memcpy(&hash_input[8 + hash_input_index++], &ENCRYPTION_DOMAIN, sizeof(field_t));
    memcpy(&hash_input[8 + hash_input_index++], &input_view_key, sizeof(field_t));
    hash_many_psd8(hash_input, hash_input_index, randomizer_fields, num_randomizers);
    PRINTF("randomizer_fields : \n");
    field_print_array(randomizer_fields, num_randomizers);

    // Compute the ciphertext
    memset(hash_input, 0, sizeof(hash_input));
    for (hash_input_index = 0; hash_input_index < num_randomizers; hash_input_index++) {
        memcpy(&hash_input[8 + hash_input_index],
               &plaintext_fields[hash_input_index],
               sizeof(field_t));
        field_add_assign(&hash_input[8 + hash_input_index], &randomizer_fields[hash_input_index]);
    }
    PRINTF("cyphertext_fields : \n");
    field_print_array(&hash_input[8], num_randomizers);

    // Hash the ciphertext to a field element
    hash_psd8(hash_input, hash_input_index, &hash);
    add_field_to_message(&hash);
    PRINTF("hash : ");
    field_println(&hash);
}

static void hash_record_input(account_t *account, prepared_request_t *request, uint8_t input_index)
{
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
    add_field_to_message(&h.x);
    PRINTF("h : ");
    group_println(&h);

    // Compute `h_r` as `r * h`
    group_scalar_multiply(&h, &request->r, &h_r);
    add_field_to_message(&h_r.x);
    PRINTF("h_r : ");
    group_println(&h_r);

    // Compute `gamma` as `sk_sig * h
    group_scalar_multiply(
        &h, &account->private_key.sk_sig, &request->gammas[request->gammas_count]);
    add_field_to_message(&request->gammas[request->gammas_count].x);
    PRINTF("gamma : ");
    group_println(&request->gammas[request->gammas_count]);
    request->gammas_count++;

    // Compute the tag
    memset(hash_input, 0, sizeof(hash_input));
    memcpy(&hash_input[2], &account->graph_key, sizeof(field_t));
    memcpy(&hash_input[3], &commitment, sizeof(field_t));
    hash_psd2(hash_input, 2, &tag);
    add_field_to_message(&tag);
    PRINTF("tag : ");
    field_println(&tag);
}

static void prepare_inputs(account_t *account, prepared_request_t *request)
{
    uint8_t input_index = 0;
    for (input_index = 0; input_index < request->inputs_count; input_index++) {
        input_t *input = &request->inputs[input_index];
        switch (input->type[0]) {
            case INPUT_ID_CONSTANT:
                break;

            case INPUT_ID_PUBLIC:
                hash_public_input(request, input_index);
                break;

            case INPUT_ID_PRIVATE:
                hash_private_input(request, input_index);
                break;

            case INPUT_ID_RECORD:
                hash_record_input(account, request, input_index);
                break;

            default:
                break;
        }
    }
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
        text = "Prepare Tx";
        current_step += step + ((1 + G_context.nested_call_offset) * 5);
    }
    else {
        text = "Prepare Tx";
    }
    // %2 is an ugly hack to force spinner text update by changing the pointer value
    snprintf(&text_buffer[step % 2],
             sizeof(text_buffer) - 1,
             "%s %d/%d",
             text,
             current_step,
             total_step);
    nbgl_useCaseSpinner(&text_buffer[step % 2]);
}

int sign_prepared_request(account_t *account, prepared_request_t *request)
{
    field_t *is_root;
    group_t  g_temp;

    field_t nonce;
    field_random(&nonce);
    PRINTF("nonce : ");
    field_println(&nonce);

    display_progression(1);

    // Compute a `r` as `HashToScalar(sk_sig || nonce)`. Note: This is the transition secret key
    // `tsk`.
    memset(hash_input, 0, sizeof(hash_input));
    memcpy(&hash_input[4], &SERIAL_NUMBER_DOMAIN, sizeof(field_t));
    scalar_to_field(&account->private_key.sk_sig, &hash_input[5]);
    memcpy(&hash_input[6], &nonce, sizeof(field_t));
    hash_to_scalar_psd4(hash_input, 3, &request->r);
    PRINTF("r : ");
    scalar_println(&request->r);
    display_progression(2);

    // Compute `g_r` as `r * G`. Note: This is the transition public key `tpk`.
    group_g_scalar_multiply(&request->r, &request->tpk);
    PRINTF("tpk : ");
    group_println(&request->tpk);

    // Compute the transition view key `tvk` as `r * signer`.
    group_scalar_multiply(&account->address, &request->r, &g_temp);
    memcpy(&request->tvk, &g_temp.x, sizeof(field_t));
    PRINTF("tvk : ");
    field_println(&request->tvk);

    // Compute the transition commitment `tcm` as `Hash(tvk)`.
    memset(hash_input, 0, sizeof(hash_input));
    memcpy(&hash_input[2], &request->tvk, sizeof(field_t));
    hash_psd2(hash_input, 1, &request->tcm);
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
    function_id_datas_t function_id_datas;
    memset(&function_id_datas, 0, sizeof(function_id_datas));
    function_id_datas.network_id = request->network_id;
    uint8_t is_name              = 1;
    uint8_t offset               = 0;
    for (size_t i = 0; i < request->program_id_length; i++) {
        if (request->program_id[i] != '.') {
            if (is_name) {
                function_id_datas.program_id_name[offset++] = request->program_id[i];
            }
            else {
                function_id_datas.program_id_network[offset++] = request->program_id[i];
            }
        }
        else {
            is_name = 0;
            offset  = 0;
        }
    }
    memcpy(function_id_datas.function_name, request->function_name, request->function_name_length);

    (void) bhp_1024_hash_function_id(&function_id_datas, &request->function_id);
    PRINTF("function_id : ");
    field_println(&request->function_id);

    // Construct the hash input as `(r * G, pk_sig, pr_sig, signer, [tvk, tcm, function ID, is_root,
    // program checksum?, input IDs])`.
    message_length = 0;
    memset(message, 0, sizeof(message));
    add_field_to_message(&request->tpk.x);
    add_field_to_message(&account->compute_key.pk_sig.x);
    add_field_to_message(&account->compute_key.pr_sig.x);
    add_field_to_message(&account->address.x);
    add_field_to_message(&request->tvk);
    add_field_to_message(&request->tcm);
    add_field_to_message(&request->function_id);
    add_field_to_message(is_root);
    if (request->program_checksum) {
        bigint_256_t s;
        field_t      program_checksum;
        bn_reverse(request->program_checksum);
        bn_to_big_int(request->program_checksum, &s);
        field_from_big_int(&program_checksum, &s);
        add_field_to_message(&program_checksum);
        PRINTF("program_checksum : ");
        field_println(&program_checksum);
    }

    display_progression(3);
    // Prepare the inputs.
    prepare_inputs(account, request);
    PRINTF("message : \n");
    field_print_array(message, 8 + message_length);

    display_progression(4);
    // Compute challenge
    hash_to_scalar_psd8(message, message_length, &request->challenge);
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

    return 0;
}
