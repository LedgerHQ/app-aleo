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

static field_t hash_input[16];
static field_t message[20];
static size_t  message_length;

static uint8_t bit_buffer[128];
static char    text_buffer[32];

static void hash_public_input(prepared_request_t *request, uint8_t input_index, field_t *hash)
{
    uint8_t  hash_input_index = 0;
    input_t *input            = &request->inputs[input_index];

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
            PRINTF("\n");
        }
    }
    memcpy(&hash_input[8 + hash_input_index++], &request->tcm, sizeof(field_t));
    field_from_int(&hash_input[8 + hash_input_index++], input_index);
    hash_psd8(hash_input, hash_input_index, hash);
}

static void prepare_inputs(prepared_request_t *request)
{
    uint8_t input_index = 0;
    for (input_index = 0; input_index < request->inputs_count; input_index++) {
        input_t *input = &request->inputs[input_index];
        switch (input->type[0]) {
            case INPUT_ID_CONSTANT:
                break;

            case INPUT_ID_PUBLIC:
                hash_public_input(request, input_index, &message[8 + message_length]);
                message_length++;
                break;

            case INPUT_ID_PRIVATE:
                break;

            case INPUT_ID_RECORD:
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

    field_t nonce = {
        .big.u64
        = {0xb4df0e1095ebbb9f, 0x0d46ec2376a4759f, 0x895aa07ec5b99957, 0x061afc10dc073447}
    };

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
    memcpy(&message[8 + message_length++], &request->tpk.x, sizeof(field_t));
    memcpy(&message[8 + message_length++], &account->compute_key.pk_sig.x, sizeof(field_t));
    memcpy(&message[8 + message_length++], &account->compute_key.pr_sig.x, sizeof(field_t));
    memcpy(&message[8 + message_length++], &account->address.x, sizeof(field_t));
    memcpy(&message[8 + message_length++], &request->tvk, sizeof(field_t));
    memcpy(&message[8 + message_length++], &request->tcm, sizeof(field_t));
    memcpy(&message[8 + message_length++], &request->function_id, sizeof(field_t));
    memcpy(&message[8 + message_length++], is_root, sizeof(field_t));
    if (request->program_checksum) {
        bigint_256_t s;
        bn_to_big_int(request->program_checksum, &s);
        field_from_big_int(&message[8 + message_length], &s);
        PRINTF("program_checksum : ");
        field_println(&message[8 + message_length]);
        message_length++;
    }

    display_progression(3);
    // Prepare the inputs.
    prepare_inputs(request);
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
