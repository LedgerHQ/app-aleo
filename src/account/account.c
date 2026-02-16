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
//#include "os_hdkey.h"
#include "globals.h"
#include "group.h"
#include "poseidon.h"
#include "base58.h"
#include "bech32.h"

#include "account.h"

static field_t hash_input[64];

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

static void seed_from_private_key_string(const char *private_key_string, field_t *seed)
{
    uint8_t      base_58_output[MAX_ENC_INPUT_SIZE];
    uint8_t      seed_bn[32];
    bigint_256_t seed_big_int;

    base58_decode(private_key_string, PRIVATE_KEY_LEN, base_58_output, sizeof(base_58_output));

    for (size_t i = 0; i < sizeof(seed_bn); i++) {
        seed_bn[sizeof(seed_bn) - 1 - i] = base_58_output[sizeof(PRIVATE_KEY_PREFIX) + i];
    }
    bn_to_big_int(seed_bn, &seed_big_int);
    field_from_big_int(seed, &seed_big_int);
}

static int get_seed(const uint32_t *path, uint8_t path_len, field_t *seed)
{
#if 1
    // TODO : Temporary code start
    uint32_t account_number = path[path_len - 1];
    PRINTF("Get seed path from account %d\n", account_number);
    if ((account_number < 4) && (N_storage.private_keys[account_number * PRIVATE_KEY_LEN] == 'A')) {
        seed_from_private_key_string(
            (const char *) &N_storage.private_keys[account_number * PRIVATE_KEY_LEN], seed);
        return 0;
    }
    // TODO : Temporary code end

#else
    uint8_t      seed_bn[32];
    bigint_256_t seed_big_int;
    cx_err_t     error = sys_hdkey_derive(HDKEY_DERIVE_MODE_BLS12377_ALEO,
                                      CX_CURVE_BLS12_377_G1,
                                      path,
                                      path_len,
                                      seed_bn,
                                      32,
                                      NULL,
                                      0,
                                      NULL,
                                      0);
    if (error != CX_OK) {
        return -1;
    }

    bn_reverse(seed_bn);
    bn_to_big_int(seed_bn, &seed_big_int);
    field_from_big_int(seed, &seed_big_int);
#endif

    return 0;
}

static void private_key_from_seed(const field_t *seed, scalar_t *sk_sig, scalar_t *r_sig)
{
    // Compute sk_sig
    memset(hash_input, 0, sizeof(hash_input));
    memcpy(&hash_input[2], &ACCOUNT_SK_SIG_DOMAIN, sizeof(field_t));
    memcpy(&hash_input[3], seed, sizeof(field_t));
    hash_to_scalar_psd2(hash_input, 2, sk_sig);
    PRINTF("sk_sig : ");
    scalar_println(sk_sig);

    // Compute r_sig
    memset(hash_input, 0, sizeof(hash_input));
    memcpy(&hash_input[2], &ACCOUNT_R_SIG_DOMAIN, sizeof(field_t));
    memcpy(&hash_input[3], seed, sizeof(field_t));
    hash_to_scalar_psd2(hash_input, 2, r_sig);
    PRINTF("r_sig : ");
    scalar_println(r_sig);
}

static void compute_key_from_private_key(const private_key_t *private_key,
                                         compute_key_t       *compute_key)
{
    group_g_scalar_multiply(&private_key->sk_sig, &compute_key->pk_sig);
    PRINTF("pk_sig : ");
    group_println(&compute_key->pk_sig);

    group_g_scalar_multiply(&private_key->r_sig, &compute_key->pr_sig);
    PRINTF("pr_sig : ");
    group_println(&compute_key->pr_sig);
}

static void view_key_from_private_and_compute_key(const private_key_t *private_key,
                                                  compute_key_t       *compute_key,
                                                  scalar_t            *view_key)
{
    memset(hash_input, 0, sizeof(hash_input));
    memcpy(&hash_input[4], &compute_key->pk_sig.x, sizeof(field_t));
    memcpy(&hash_input[5], &compute_key->pr_sig.x, sizeof(field_t));
    hash_to_scalar_psd4(hash_input, 2, &compute_key->sk_prf);
    PRINTF("sk_prf : ");
    scalar_println(&compute_key->sk_prf);

    memcpy(view_key, &compute_key->sk_prf, sizeof(scalar_t));
    scalar_add_assign(view_key, &private_key->sk_sig);
    scalar_add_assign(view_key, &private_key->r_sig);
    PRINTF("view_key : ");
    scalar_println(view_key);
}

static void address_from_view_key(const scalar_t *view_key, group_t *address)
{
    group_g_scalar_multiply(view_key, address);

    PRINTF("address : ");
    group_println(address);
}

static void graph_key_from_view_key(const scalar_t *view_key, field_t *graph_key)
{
    field_t f_view_key;
    scalar_to_field(view_key, &f_view_key);

    memset(hash_input, 0, sizeof(hash_input));
    memcpy(&hash_input[4], &GRAPH_KEY_DOMAIN, sizeof(field_t));
    memcpy(&hash_input[5], &f_view_key, sizeof(field_t));
    memset(&hash_input[6], 0, sizeof(field_t));
    hash_psd4(hash_input, 3, graph_key);
    PRINTF("graph key : ");
    field_println(graph_key);
}

int account_get_address_string(const uint32_t *path, uint8_t path_len, char address[ADDRESS_LEN])
{
    account_t    account;
    bigint_256_t address_big_int;
    uint8_t      address_bn[32];
    int          status = get_seed(path, path_len, &account.private_key.seed);

    if (status != 0) {
        return status;
    }

    private_key_from_seed(
        &account.private_key.seed, &account.private_key.sk_sig, &account.private_key.r_sig);
    compute_key_from_private_key(&account.private_key, &account.compute_key);
    view_key_from_private_and_compute_key(
        &account.private_key, &account.compute_key, &account.view_key);
    address_from_view_key(&account.view_key, &account.address);
    graph_key_from_view_key(&account.view_key, &account.graph_key);

    field_to_big_int(&account.address.x, &address_big_int);
    big_int_to_bn(&address_big_int, address_bn);

    // Reverse bn
    for (size_t i = 0; i < sizeof(address_bn) / 2; i++) {
        uint8_t item                           = address_bn[i];
        address_bn[i]                          = address_bn[sizeof(address_bn) - 1 - i];
        address_bn[sizeof(address_bn) - 1 - i] = item;
    }

    for (size_t i = 0; i < sizeof(address_bn); i++) {
        PRINTF("%02x ", address_bn[i]);
    }
    PRINTF("\n");

    uint8_t data[64];
    size_t  datalen = 0;
    bech32_convert_bits(data, &datalen, 5, address_bn, 32, 8, 1);
    bech32_encode(address, ADDRESS_PREFIX, data, datalen, BECH32_ENCODING_BECH32M);
    PRINTF("%s\n", address);

    explicit_bzero(&account, sizeof(account));

    return status;
}

int account_get_view_key_string(const uint32_t *path, uint8_t path_len, char *viewkey)
{
    account_t    account;
    bigint_256_t view_key_big_int;
    uint8_t      view_key_bn[32];
    uint8_t      base_58_input[MAX_ENC_INPUT_SIZE];
    int          status = get_seed(path, path_len, &account.private_key.seed);

    if (status != 0) {
        return status;
    }

    private_key_from_seed(
        &account.private_key.seed, &account.private_key.sk_sig, &account.private_key.r_sig);
    compute_key_from_private_key(&account.private_key, &account.compute_key);
    view_key_from_private_and_compute_key(
        &account.private_key, &account.compute_key, &account.view_key);

    scalar_to_big_int(&account.view_key, &view_key_big_int);
    big_int_to_bn(&view_key_big_int, view_key_bn);
    memset(base_58_input, 0, sizeof(base_58_input));
    memcpy(base_58_input, VIEW_KEY_PREFIX, sizeof(VIEW_KEY_PREFIX));
    for (size_t i = 0; i < sizeof(view_key_bn); i++) {
        base_58_input[sizeof(VIEW_KEY_PREFIX) + i] = view_key_bn[sizeof(view_key_bn) - 1 - i];
    }
    base58_encode(base_58_input, sizeof(VIEW_KEY_PREFIX) + sizeof(view_key_bn), viewkey, 64);
    PRINTF("%s\n", viewkey);

    explicit_bzero(&account, sizeof(account));

    return status;
}

int account_get_private_key_string(const uint32_t *path, uint8_t path_len, char *private_key)
{
    account_t    account;
    bigint_256_t seed_big_int;
    uint8_t      seed_bn[32];
    uint8_t      base_58_input[MAX_ENC_INPUT_SIZE];
    int          status = get_seed(path, path_len, &account.private_key.seed);

    if (status != 0) {
        return status;
    }

    field_to_big_int(&account.private_key.seed, &seed_big_int);
    big_int_to_bn(&seed_big_int, seed_bn);
    memset(base_58_input, 0, sizeof(base_58_input));
    memcpy(base_58_input, PRIVATE_KEY_PREFIX, sizeof(PRIVATE_KEY_PREFIX));
    for (size_t i = 0; i < sizeof(seed_bn); i++) {
        base_58_input[sizeof(PRIVATE_KEY_PREFIX) + i] = seed_bn[sizeof(seed_bn) - 1 - i];
    }
    base58_encode(base_58_input, sizeof(PRIVATE_KEY_PREFIX) + sizeof(seed_bn), private_key, 64);
    PRINTF("%s\n", private_key);

    field_t seed;
    seed_from_private_key_string(private_key, &seed);
    field_println(&seed);

    explicit_bzero(&account, sizeof(account));

    return status;
}

int account_sign(const uint32_t *path, uint8_t path_len, account_signature_t *signature)
{
    account_t account;
    int       status = get_seed(path, path_len, &account.private_key.seed);

    if (status != 0) {
        return status;
    }

    // Prepare keys
    private_key_from_seed(
        &account.private_key.seed, &account.private_key.sk_sig, &account.private_key.r_sig);
    compute_key_from_private_key(&account.private_key, &account.compute_key);
    view_key_from_private_and_compute_key(
        &account.private_key, &account.compute_key, &account.view_key);
    address_from_view_key(&account.view_key, &account.address);

    // Fill signature compute key
    memcpy(&signature->compute_key, &account.compute_key, sizeof(compute_key_t));

    // Compute challenge
    scalar_t nonce;
    group_t  g_r;

    scalar_random(&nonce);
    PRINTF("nonce : ");
    scalar_println(&nonce);

    group_g_scalar_multiply(&nonce, &g_r);
    PRINTF("g_r : ");
    group_println(&g_r);

    memset(hash_input, 0, sizeof(hash_input));
    memcpy(&hash_input[8], &g_r.x, sizeof(field_t));
    memcpy(&hash_input[9], &account.compute_key.pk_sig.x, sizeof(field_t));
    memcpy(&hash_input[10], &account.compute_key.pr_sig.x, sizeof(field_t));
    memcpy(&hash_input[11], &account.address.x, sizeof(field_t));
    for (size_t i = 0; i < 32; i++) {
        // memcpy(&hash_input[12 + i], &test_message[i], sizeof(field_t));
    }
    hash_to_scalar_psd8(hash_input, 4 + 32, &signature->challenge);
    PRINTF("challenge : ");
    scalar_println(&signature->challenge);

    // Compute response
    scalar_t s_res;
    memcpy(&s_res, &signature->challenge, sizeof(scalar_t));
    scalar_mul_assign(&s_res, &account.private_key.sk_sig);

    memcpy(&signature->response, &nonce, sizeof(scalar_t));
    scalar_sub_assign(&signature->response, &s_res);
    PRINTF("response : ");
    scalar_println(&signature->response);

    explicit_bzero(&account, sizeof(account));

    return status;
}

void account_init(void) {}

int account_generate_keys(const uint32_t *path, uint8_t path_len, account_t *account)
{
    (void) get_seed(path, path_len, &account->private_key.seed);
    private_key_from_seed(
        &account->private_key.seed, &account->private_key.sk_sig, &account->private_key.r_sig);
    compute_key_from_private_key(&account->private_key, &account->compute_key);
    view_key_from_private_and_compute_key(
        &account->private_key, &account->compute_key, &account->view_key);
    address_from_view_key(&account->view_key, &account->address);
    graph_key_from_view_key(&account->view_key, &account->graph_key);

    return 0;
}
