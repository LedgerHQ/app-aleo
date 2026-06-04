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
#include "os_hdkey.h"
#include "sw.h"
#include "globals.h"
#include "group.h"
#include "poseidon.h"
#include "base58.h"
#include "bech32.h"
#include "nbgl_use_case.h"

#include "account.h"

#define HASH_INPUT_MAX_LENGTH (8)

static field_t hash_input[HASH_INPUT_MAX_LENGTH];
static char    text_buffer[32];

const field_t ACCOUNT_SK_SIG_DOMAIN = {
    .big.u64 = {0xc9a73b0068afb54b, 0x95d2050edfd00d2d, 0x30b27b31e4cc8dc3, 0x127ef5e8bbf7590e}
};
const field_t ACCOUNT_R_SIG_DOMAIN = {
    .big.u64 = {0x9a1b12fc7f84af91, 0x935265d4483fc420, 0x69ad7c25c73999f2, 0x0fae3aac5d72c73b}
};
const field_t GRAPH_KEY_DOMAIN = {
    .big.u64 = {0xbe2ebe0e9bbca61b, 0x1cd3f707abca5c71, 0xfd11846a18bef8c1, 0x02c5f9cb610ab8f5}
};

const uint8_t VIEW_KEY_PREFIX[7]     = {14, 138, 223, 204, 247, 224, 122};
const uint8_t PRIVATE_KEY_PREFIX[11] = {127, 134, 189, 116, 210, 221, 210, 137, 145, 18, 253};
const char    ADDRESS_PREFIX[5]      = {'a', 'l', 'e', 'o', 0};

static void display_progression(uint8_t step)
{
    // %2 is an ugly hack to force spinner text update by changing the pointer value
    snprintf(&text_buffer[step % 2], sizeof(text_buffer) - (step % 2), "Loading transaction");
#ifndef FUZZ
    nbgl_useCaseSpinner(&text_buffer[step % 2]);
#endif  // FUZZ
}

static int get_seed(const uint32_t *path, uint8_t path_len, field_t *seed)
{
    uint8_t      seed_bn[BN_LENGTH];
    bigint_256_t seed_big_int;
    bolos_err_t  error = sys_hdkey_derive(HDKEY_DERIVE_MODE_BLS12377_ALEO,
                                         CX_CURVE_BLS12_377_G1,
                                         path,
                                         path_len,
                                         seed_bn,
                                         BN_LENGTH,
                                         NULL,
                                         0,
                                         NULL,
                                         0);
    if (error != SWO_OK) {
        return -1;
    }
    bn_print(seed_bn);

    bn_reverse(seed_bn);
    bn_to_big_int(seed_bn, &seed_big_int);
    field_from_big_int(seed, &seed_big_int);

    return 0;
}

static int private_key_from_seed(const field_t *seed, scalar_t *sk_sig, scalar_t *r_sig)
{
    _Static_assert(HASH_INPUT_MAX_LENGTH >= 4, "hash_input size won't fit");
    int status = -1;

    // Compute sk_sig
    memset(hash_input, 0, sizeof(hash_input));
    memcpy(&hash_input[2], &ACCOUNT_SK_SIG_DOMAIN, sizeof(field_t));
    memcpy(&hash_input[3], seed, sizeof(field_t));
    status = hash_to_scalar_psd2(hash_input, 2 + 2, sk_sig);
    if (status < 0) {
        return -1;
    }
    PRINTF("sk_sig : ");
    scalar_println(sk_sig);

    // Compute r_sig
    memset(hash_input, 0, sizeof(hash_input));
    memcpy(&hash_input[2], &ACCOUNT_R_SIG_DOMAIN, sizeof(field_t));
    memcpy(&hash_input[3], seed, sizeof(field_t));
    status = hash_to_scalar_psd2(hash_input, 2 + 2, r_sig);
    if (status < 0) {
        return -1;
    }
    PRINTF("r_sig : ");
    scalar_println(r_sig);

    return status;
}

static int compute_key_from_private_key(const private_key_t *private_key,
                                        compute_key_t       *compute_key)
{
    int status = -1;

    status = group_g_scalar_multiply(&private_key->sk_sig, &compute_key->pk_sig);
    if (status < 0) {
        return -1;
    }
    PRINTF("pk_sig : ");
    group_println(&compute_key->pk_sig);

    status = group_g_scalar_multiply(&private_key->r_sig, &compute_key->pr_sig);
    if (status < 0) {
        return -1;
    }
    PRINTF("pr_sig : ");
    group_println(&compute_key->pr_sig);

    return status;
}

static int view_key_from_private_and_compute_key(const private_key_t *private_key,
                                                 compute_key_t       *compute_key,
                                                 scalar_t            *view_key)
{
    _Static_assert(HASH_INPUT_MAX_LENGTH >= 6, "hash_input size won't fit");
    int status = -1;

    memset(hash_input, 0, sizeof(hash_input));
    memcpy(&hash_input[4], &compute_key->pk_sig.x, sizeof(field_t));
    memcpy(&hash_input[5], &compute_key->pr_sig.x, sizeof(field_t));
    status = hash_to_scalar_psd4(hash_input, 4 + 2, &compute_key->sk_prf);
    if (status < 0) {
        return -1;
    }
    PRINTF("sk_prf : ");
    scalar_println(&compute_key->sk_prf);

    memcpy(view_key, &compute_key->sk_prf, sizeof(scalar_t));
    scalar_add_assign(view_key, &private_key->sk_sig);
    scalar_add_assign(view_key, &private_key->r_sig);
    PRINTF("view_key : ");
    scalar_println(view_key);

    return status;
}

static int address_from_view_key(const scalar_t *view_key, group_t *address)
{
    int status = -1;

    status = group_g_scalar_multiply(view_key, address);

    PRINTF("address : ");
    group_println(address);

    return status;
}

static int graph_key_from_view_key(const scalar_t *view_key, field_t *graph_key)
{
    _Static_assert(HASH_INPUT_MAX_LENGTH >= 7, "hash_input size won't fit");
    int     status = -1;
    field_t f_view_key;
    scalar_to_field(view_key, &f_view_key);

    memset(hash_input, 0, sizeof(hash_input));
    memcpy(&hash_input[4], &GRAPH_KEY_DOMAIN, sizeof(field_t));
    memcpy(&hash_input[5], &f_view_key, sizeof(field_t));
    memset(&hash_input[6], 0, sizeof(field_t));
    status = hash_psd4(hash_input, 4 + 3, graph_key);
    PRINTF("graph key : ");
    field_println(graph_key);

    return status;
}

int account_get_address_string(const uint32_t *path,
                               uint8_t         path_len,
                               char            address[ADDRESS_LEN + 1])
{
    account_t    account;
    bigint_256_t address_big_int;
    uint8_t      address_bn[BN_LENGTH];
    int          status = -1;

    LEDGER_ASSERT(path != NULL, "NULL path");
    LEDGER_ASSERT(address != NULL, "NULL address");

    if ((status = get_seed(path, path_len, &account.private_key.seed)) < 0) {
        goto end;
    }
    if ((status = private_key_from_seed(
             &account.private_key.seed, &account.private_key.sk_sig, &account.private_key.r_sig))
        < 0) {
        goto end;
    }
    if ((status = compute_key_from_private_key(&account.private_key, &account.compute_key)) < 0) {
        goto end;
    }
    if ((status = view_key_from_private_and_compute_key(
             &account.private_key, &account.compute_key, &account.view_key))
        < 0) {
        goto end;
    }
    if ((status = address_from_view_key(&account.view_key, &account.address)) < 0) {
        goto end;
    }
    if ((status = graph_key_from_view_key(&account.view_key, &account.graph_key)) < 0) {
        goto end;
    }

    field_to_big_int(&account.address.x, &address_big_int);
    big_int_to_bn(&address_big_int, address_bn);

    // Reverse bn
    bn_reverse(address_bn);
    bn_print(address_bn);

    uint8_t data[ADDRESS_LEN + 1];
    size_t  datalen = 0;
    memset(address, 0, ADDRESS_LEN + 1);
    if ((status = bech32_convert_bits(
             data, &datalen, sizeof(data), 5, address_bn, sizeof(address_bn), 8, 1))
        < 0) {
        goto end;
    }
    // bech32_encode writes 63 chars + NUL for Aleo addresses.
    if ((4 + 1 + datalen + 6 + 1) > (ADDRESS_LEN + 1)) {
        status = -1;
        goto end;
    }
    status = bech32_encode(address, ADDRESS_PREFIX, data, datalen, BECH32_ENCODING_BECH32M);
    PRINTF("%s\n", address);

end:
    explicit_bzero(hash_input, sizeof(hash_input));
    explicit_bzero(&account, sizeof(account_t));

    return status;
}

int account_get_view_key_string(const uint32_t *path,
                                uint8_t         path_len,
                                char            viewkey[VIEW_KEY_LEN + 1])
{
    account_t    account;
    bigint_256_t view_key_big_int;
    uint8_t      view_key_bn[BN_LENGTH];
    uint8_t      base_58_input[MAX_ENC_INPUT_SIZE];
    int          status = -1;

    LEDGER_ASSERT(path != NULL, "NULL path");
    LEDGER_ASSERT(viewkey != NULL, "NULL viewkey");

    if ((status = get_seed(path, path_len, &account.private_key.seed)) < 0) {
        goto end;
    }

    if ((status = private_key_from_seed(
             &account.private_key.seed, &account.private_key.sk_sig, &account.private_key.r_sig))
        < 0) {
        goto end;
    }
    if ((status = compute_key_from_private_key(&account.private_key, &account.compute_key)) < 0) {
        goto end;
    }
    if ((status = view_key_from_private_and_compute_key(
             &account.private_key, &account.compute_key, &account.view_key))
        < 0) {
        goto end;
    }

    scalar_to_big_int(&account.view_key, &view_key_big_int);
    big_int_to_bn(&view_key_big_int, view_key_bn);
    memset(base_58_input, 0, sizeof(base_58_input));
    memcpy(base_58_input, VIEW_KEY_PREFIX, sizeof(VIEW_KEY_PREFIX));
    for (size_t i = 0; i < sizeof(view_key_bn); i++) {
        base_58_input[sizeof(VIEW_KEY_PREFIX) + i] = view_key_bn[sizeof(view_key_bn) - 1 - i];
    }
    memset(viewkey, 0, VIEW_KEY_LEN + 1);
    int out_len = base58_encode(
        base_58_input, sizeof(VIEW_KEY_PREFIX) + sizeof(view_key_bn), viewkey, VIEW_KEY_LEN + 1);
    if (out_len < 0 || out_len > VIEW_KEY_LEN) {
        status = -1;
        goto end;
    }
    PRINTF("%s\n", viewkey);

end:
    explicit_bzero(hash_input, sizeof(hash_input));
    explicit_bzero(&account, sizeof(account_t));

    return status;
}

int account_generate_keys(const uint32_t *path, uint8_t path_len, account_t *account)
{
    int          status = -1;
    bigint_256_t address_big_int;
    uint8_t      address_bn[BN_LENGTH];

    LEDGER_ASSERT(path != NULL, "NULL path");
    LEDGER_ASSERT(account != NULL, "NULL account");

    display_progression(0);
    if ((status = get_seed(path, path_len, &account->private_key.seed)) < 0) {
        goto error;
    }
    if ((status = private_key_from_seed(
             &account->private_key.seed, &account->private_key.sk_sig, &account->private_key.r_sig))
        < 0) {
        goto error;
    }
    if ((status = compute_key_from_private_key(&account->private_key, &account->compute_key)) < 0) {
        goto error;
    }
    display_progression(1);
    if ((status = view_key_from_private_and_compute_key(
             &account->private_key, &account->compute_key, &account->view_key))
        < 0) {
        goto error;
    }
    if ((status = address_from_view_key(&account->view_key, &account->address)) < 0) {
        goto error;
    }
    if ((status = graph_key_from_view_key(&account->view_key, &account->graph_key)) < 0) {
        goto error;
    }

    field_to_big_int(&account->address.x, &address_big_int);
    big_int_to_bn(&address_big_int, address_bn);

    // Reverse bn
    bn_reverse(address_bn);
    bn_print(address_bn);

    uint8_t data[ADDRESS_LEN + 1];
    size_t  datalen = 0;
    memset(account->address_str, 0, ADDRESS_LEN + 1);
    if ((status = bech32_convert_bits(
             data, &datalen, sizeof(data), 5, address_bn, sizeof(address_bn), 8, 1))
        < 0) {
        goto error;
    }
    if ((status = bech32_encode(
             account->address_str, ADDRESS_PREFIX, data, datalen, BECH32_ENCODING_BECH32M))
        < 0) {
        goto error;
    }

    return 0;

error:
    explicit_bzero(hash_input, sizeof(hash_input));
    explicit_bzero(account, sizeof(account_t));
    return status;
}

void account_erase(account_t *account)
{
    explicit_bzero(account, sizeof(account_t));
}
