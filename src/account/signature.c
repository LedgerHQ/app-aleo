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
#include "group.h"
#include "poseidon.h"
#include "bhp_512.h"
#include "bhp_1024.h"
#include "bits.h"

#include "signature.h"

typedef enum {
    INPUT_ID_CONSTANT        = 0,
    INPUT_ID_PUBLIC          = 1,
    INPUT_ID_PRIVATE         = 2,
    INPUT_ID_RECORD          = 3,
    INPUT_ID_EXTERNAL_RECORD = 4,
    INPUT_ID_FUTURE          = 5,
} input_id_e;

typedef enum {
    INPUT_VALUE_TYPE_PLAINTEXT = 0,
    INPUT_VALUE_TYPE_RECORD    = 1,
    INPUT_VALUE_TYPE_FUTURE    = 2,
} input_value_type_e;

typedef enum {
    PLAINTEXT_TYPE_LITERAL_ADDRESS = 0,
    PLAINTEXT_TYPE_LITERAL_U64     = 12,
} plaintext_type_literal_e;

const field_t SERIAL_NUMBER_DOMAIN = {
    .big.u64 = {0xf993d63c8bc0a4ad, 0xe22f8477532c74cc, 0x367fe5e2c9f74b96, 0x0309ea7b80fce820}
};

const function_id_datas_t test_function_id = {
    .network_id         = 0,
    .program_id_name    = "credits",
    .program_id_network = "aleo",
    .function_name      = "transfer_public",
};

static field_t hash_input[8];
static field_t message[20];
static size_t  message_length;

static uint8_t bit_buffer[128];

static void plaintext_to_bits(const uint8_t *plaintext,
                              uint16_t       bit_length,
                              const uint8_t *plaintext_type,
                              uint8_t       *out)
{
    uint16_t out_offset = 0;
    if (plaintext_type[1] == 0x00) {
        // Literal
        out_offset = 2;  // add [false, false]
        out_offset += 8;
        uint8_t data = 0xfd;
        bits_add(out, out_offset, &data, 8);
        out_offset += 16;
        bits_add(out, out_offset, plaintext, bit_length);
        out_offset += bit_length;
        bits_add_single(out, out_offset, true);
        bits_println(out, out_offset);
    }
    else {
    }
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
                if (input->type[1] == 0) {  // literal
                    if (input->type[2] == PLAINTEXT_TYPE_LITERAL_ADDRESS) {
                        memset(bit_buffer, 0, sizeof(bit_buffer));
                        plaintext_to_bits(
                            input->value, FIELD_MODULUS_BITS, &input->type[1], bit_buffer);
                    }
                    else if (input->type[2] == PLAINTEXT_TYPE_LITERAL_U64) {
                    }
                }
                break;

            case INPUT_ID_PRIVATE:
                break;

            default:
                break;
        }
    }
}

int sign_prepared_request(account_t *account, prepared_request_t *request)
{
    scalar_t r;
    group_t  g_r;
    field_t  tvk;
    field_t  tcm;
    field_t  scm;
    field_t *is_root;
    field_t  function_id;
    group_t  g_temp;

    field_t nonce = {
        .big.u64
        = {0xad290fa7dd257fb2, 0x385bf35014630b35, 0x2bc4b958ce75c306, 0x00d4936b27f225b6}
    };
    field_t program_checksum = {
        .big.u64
        = {0x66df0cd12ba73fdd, 0xf52d96b094afd437, 0x794127a6e766061f, 0x0aa4d60793a54ad5}
    };

    // Compute a `r` as `HashToScalar(sk_sig || nonce)`. Note: This is the transition secret key
    // `tsk`.
    memset(hash_input, 0, sizeof(hash_input));
    memcpy(&hash_input[4], &SERIAL_NUMBER_DOMAIN, sizeof(field_t));
    scalar_to_field(&account->private_key.sk_sig, &hash_input[5]);
    memcpy(&hash_input[6], &nonce, sizeof(field_t));
    hash_to_scalar_psd4(hash_input, 3, &r);
    PRINTF("r : ");
    scalar_println(&r);

    // Compute `g_r` as `r * G`. Note: This is the transition public key `tpk`.
    group_g_scalar_multiply(&r, &g_r);
    PRINTF("g_r : ");
    group_println(&g_r);

    // Compute the transition view key `tvk` as `r * signer`.
    group_scalar_multiply(&account->address, &r, &g_temp);
    memcpy(&tvk, &g_temp.x, sizeof(field_t));
    PRINTF("tvk : ");
    field_println(&tvk);

    // Compute the transition commitment `tcm` as `Hash(tvk)`.
    memset(hash_input, 0, sizeof(hash_input));
    memcpy(&hash_input[2], &tvk, sizeof(field_t));
    hash_psd2(hash_input, 1, &tcm);
    PRINTF("tcm : ");
    field_println(&tcm);

    // Compute the signer commitment `scm` as `Hash(signer || root_tvk)`.
    memset(hash_input, 0, sizeof(hash_input));
    memcpy(&hash_input[2], &account->address.x, sizeof(field_t));
    memcpy(&hash_input[3], &tvk, sizeof(field_t));
    hash_psd2(hash_input, 2, &scm);
    PRINTF("scm : ");
    field_println(&scm);

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

    (void) bhp_1024_hash_function_id(&function_id_datas, &function_id);
    PRINTF("function_id : ");
    field_println(&function_id);

    // Construct the hash input as `(r * G, pk_sig, pr_sig, signer, [tvk, tcm, function ID, is_root,
    // program checksum?, input IDs])`.
    message_length = 0;
    memset(message, 0, sizeof(message));
    memcpy(&message[message_length++], &g_r.x, sizeof(field_t));
    memcpy(&message[message_length++], &account->compute_key.pk_sig.x, sizeof(field_t));
    memcpy(&message[message_length++], &account->compute_key.pr_sig.x, sizeof(field_t));
    memcpy(&message[message_length++], &account->address.x, sizeof(field_t));
    memcpy(&message[message_length++], &tvk, sizeof(field_t));
    memcpy(&message[message_length++], &tcm, sizeof(field_t));
    memcpy(&message[message_length++], &function_id, sizeof(field_t));
    memcpy(&message[message_length++], is_root, sizeof(field_t));
    memcpy(&message[message_length++], &program_checksum, sizeof(field_t));
    PRINTF("message : \n");
    field_print_array(message, message_length);

    // Prepare the inputs.
    prepare_inputs(request);

    field_t to_address = {
        .big.u64
        = {0x780d698b3e8082b0, 0xea572fa0576c0267, 0xcf71384adea62486, 0x0f0be051689e66e6}
    };
    bigint_256_t s;
    field_to_big_int(&to_address, &s);
    big_int_println(&s);

    return 0;
}

/*void signature_test(void)
{
    field_t nonce = {
        .big.u64
        = {0xad290fa7dd257fb2, 0x385bf35014630b35, 0x2bc4b958ce75c306, 0x00d4936b27f225b6}
    };
    field_t program_checksum = {
        .big.u64
        = {0x66df0cd12ba73fdd, 0xf52d96b094afd437, 0x794127a6e766061f, 0x0aa4d60793a54ad5}
    };

    PRINTF("Nonce ");
    field_println(&nonce);

    account_t a;
    // account_generate_keys(&a);
    PRINTF("\n");

    memset(hash_input, 0, sizeof(hash_input));
    memcpy(&hash_input[4], &SERIAL_NUMBER_DOMAIN, sizeof(field_t));
    scalar_to_field(&a.private_key.sk_sig, &hash_input[5]);
    memcpy(&hash_input[6], &nonce, sizeof(field_t));

    scalar_t r;
    hash_to_scalar_psd4(hash_input, 3, &r);
    PRINTF("r : ");
    scalar_println(&r);

    group_t g_r;
    group_g_scalar_multiply(&r, &g_r);
    PRINTF("g_r : ");
    group_println(&g_r);

    PRINTF("signer : ");
    group_println(&a.address);

    field_t tvk;
    group_t g_temp;
    group_scalar_multiply(&a.address, &r, &g_temp);
    memcpy(&tvk, &g_temp.x, sizeof(field_t));
    PRINTF("tvk : ");
    field_println(&tvk);

    field_t tcm;
    memset(hash_input, 0, sizeof(hash_input));
    memcpy(&hash_input[2], &tvk, sizeof(field_t));
    hash_psd2(hash_input, 1, &tcm);
    PRINTF("tcm : ");
    field_println(&tcm);

    field_t scm;
    memset(hash_input, 0, sizeof(hash_input));
    memcpy(&hash_input[2], &a.address.x, sizeof(field_t));
    memcpy(&hash_input[3], &tvk, sizeof(field_t));
    hash_psd2(hash_input, 2, &scm);
    PRINTF("scm : ");
    field_println(&scm);

    field_t function_id;
    int     res = bhp_1024_hash_function_id(&test_function_id, &function_id);
    PRINTF("function_id : ");
    field_println(&function_id);

    message_length = 0;
    memset(message, 0, sizeof(message));
    memcpy(&message[message_length++], &g_r.x, sizeof(field_t));
    memcpy(&message[message_length++], &a.compute_key.pk_sig.x, sizeof(field_t));
    memcpy(&message[message_length++], &a.compute_key.pr_sig.x, sizeof(field_t));
    memcpy(&message[message_length++], &a.address.x, sizeof(field_t));
    memcpy(&message[message_length++], &tvk, sizeof(field_t));
    memcpy(&message[message_length++], &tcm, sizeof(field_t));
    memcpy(&message[message_length++], &function_id, sizeof(field_t));
    memcpy(&message[message_length++], &FIELD_ONE, sizeof(field_t));
    memcpy(&message[message_length++], &program_checksum, sizeof(field_t));

    PRINTF("message : \n");
    field_print_array(message, message_length);

    field_t to_address = {
        .big.u64
        = {0x780d698b3e8082b0, 0xea572fa0576c0267, 0xcf71384adea62486, 0x0f0be051689e66e6}
    };
    bigint_256_t s;
    field_to_big_int(&to_address, &s);
    big_int_println(&s);

}*/
