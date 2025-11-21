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
#include "base58.h"
#include "bech32.h"

#include "account.h"

typedef struct {
    field_t seed;
    scalar_t sk_sig;
    scalar_t r_sig;

} private_key_t;

typedef struct {
    private_key_t private_key;
    scalar_t view_key;
    group_t address;

} account_t;

static account_t account;

static field_t hash_input[64];

const field_t ACCOUNT_SK_SIG_DOMAIN = {
    .big.u64 = {0xc9a73b0068afb54b, 0x95d2050edfd00d2d, 0x30b27b31e4cc8dc3, 0x127ef5e8bbf7590e}};
const field_t ACCOUNT_R_SIG_DOMAIN = {
    .big.u64 = {0x9a1b12fc7f84af91, 0x935265d4483fc420, 0x69ad7c25c73999f2, 0x0fae3aac5d72c73b}};
const field_t test_seed = {
    .big.u64 = {0xb4df0e1095ebbb9f, 0x0d46ec2376a4759f, 0x895aa07ec5b99957, 0x061afc10dc073447}};

const uint8_t VIEW_KEY_PREFIX[7] = {14, 138, 223, 204, 247, 224, 122};
const uint8_t PRIVATE_KEY_PREFIX[11] = {127, 134, 189, 116, 210, 221, 210, 137, 145, 18, 253};
const char ADDRESS_PREFIX[5] = {'a', 'l', 'e', 'o', 0};

static int get_seed(const uint32_t *path, uint8_t path_len, field_t *seed) {
    uint8_t seed_bn[64];
    cx_err_t error = os_derive_bip32_with_seed_no_throw(HDW_NORMAL,
                                                        CX_CURVE_256K1,
                                                        path,
                                                        path_len,
                                                        seed_bn,
                                                        NULL,
                                                        NULL,
                                                        0);
    if (error != CX_OK) {
        return -1;
    }

    bigint_256_t seed_big_int;
    bn_to_big_int(seed_bn, &seed_big_int);
    field_from_big_int(seed, &seed_big_int);

    return 0;
}

static void private_key_from_seed(const field_t *seed, scalar_t *sk_sig, scalar_t *r_sig) {
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

static void view_key_from_private_key(const scalar_t *sk_sig,
                                      const scalar_t *r_sig,
                                      scalar_t *view_key) {
    group_t pk_sig, pr_sig;
    scalar_t sk_prf;

    group_g_scalar_multiply(sk_sig, &pk_sig);
    PRINTF("pk_sig : ");
    group_println(&pk_sig);

    group_g_scalar_multiply(r_sig, &pr_sig);
    PRINTF("pr_sig : ");
    group_println(&pr_sig);

    memset(hash_input, 0, sizeof(hash_input));
    memcpy(&hash_input[4], &pk_sig.x, sizeof(field_t));
    memcpy(&hash_input[5], &pr_sig.x, sizeof(field_t));
    hash_to_scalar_psd4(hash_input, 2, &sk_prf);
    PRINTF("sk_prf : ");
    scalar_println(&sk_prf);

    memcpy(view_key, &sk_prf, sizeof(scalar_t));
    scalar_add_assign(view_key, sk_sig);
    scalar_add_assign(view_key, r_sig);
    PRINTF("view_key : ");
    scalar_println(view_key);
}

static void address_from_view_key(const scalar_t *view_key, group_t *address) {
    group_g_scalar_multiply(view_key, address);

    PRINTF("address : ");
    group_println(address);
}

int account_get_address_string(const uint32_t *path, uint8_t path_len, char address[ADDRESS_LEN]) {
    bigint_256_t address_big_int;
    uint8_t address_bn[32];
    int status = get_seed(path, path_len, &account.private_key.seed);

    if (status != 0) {
        return status;
    }

    memcpy(&account.private_key.seed, &test_seed, sizeof(field_t));  // TEST

    private_key_from_seed(&account.private_key.seed,
                          &account.private_key.sk_sig,
                          &account.private_key.r_sig);
    view_key_from_private_key(&account.private_key.sk_sig,
                              &account.private_key.r_sig,
                              &account.view_key);
    address_from_view_key(&account.view_key, &account.address);

    field_to_big_int(&account.address.x, &address_big_int);
    big_int_to_bn(&address_big_int, address_bn);

    // Reverse bn
    for (size_t i = 0; i < sizeof(address_bn) / 2; i++) {
        uint8_t item = address_bn[i];
        address_bn[i] = address_bn[sizeof(address_bn) - 1 - i];
        address_bn[sizeof(address_bn) - 1 - i] = item;
    }

    for (size_t i = 0; i < sizeof(address_bn); i++) {
        PRINTF("%02x ", address_bn[i]);
    }
    PRINTF("\n");

    uint8_t data[64];
    size_t datalen = 0;
    bech32_convert_bits(data, &datalen, 5, address_bn, 32, 8, 1);
    bech32_encode(address, ADDRESS_PREFIX, data, datalen, BECH32_ENCODING_BECH32M);
    PRINTF("%s\n", address);

    return status;
}

int account_get_view_key_string(const uint32_t *path, uint8_t path_len, char *viewkey) {
    bigint_256_t view_key_big_int;
    uint8_t view_key_bn[32];
    uint8_t base_58_input[MAX_ENC_INPUT_SIZE];
    int status = get_seed(path, path_len, &account.private_key.seed);

    if (status != 0) {
        return status;
    }

    memcpy(&account.private_key.seed, &test_seed, sizeof(field_t));  // TEST

    private_key_from_seed(&account.private_key.seed,
                          &account.private_key.sk_sig,
                          &account.private_key.r_sig);
    view_key_from_private_key(&account.private_key.sk_sig,
                              &account.private_key.r_sig,
                              &account.view_key);

    scalar_to_big_int(&account.view_key, &view_key_big_int);
    big_int_to_bn(&view_key_big_int, view_key_bn);
    memset(base_58_input, 0, sizeof(base_58_input));
    memcpy(base_58_input, VIEW_KEY_PREFIX, sizeof(VIEW_KEY_PREFIX));
    for (size_t i = 0; i < sizeof(view_key_bn); i++) {
        base_58_input[sizeof(VIEW_KEY_PREFIX) + i] = view_key_bn[sizeof(view_key_bn) - 1 - i];
    }
    base58_encode(base_58_input, sizeof(VIEW_KEY_PREFIX) + sizeof(view_key_bn), viewkey, 64);
    PRINTF("%s\n", viewkey);

    return status;
}

int account_get_private_key_string(const uint32_t *path, uint8_t path_len, char *private_key) {
    bigint_256_t seed_big_int;
    uint8_t seed_bn[32];
    uint8_t base_58_input[MAX_ENC_INPUT_SIZE];
    int status = get_seed(path, path_len, &account.private_key.seed);

    if (status != 0) {
        return status;
    }

    memcpy(&account.private_key.seed, &test_seed, sizeof(field_t));  // TEST

    field_to_big_int(&account.private_key.seed, &seed_big_int);
    big_int_to_bn(&seed_big_int, seed_bn);
    memset(base_58_input, 0, sizeof(base_58_input));
    memcpy(base_58_input, PRIVATE_KEY_PREFIX, sizeof(PRIVATE_KEY_PREFIX));
    for (size_t i = 0; i < sizeof(seed_bn); i++) {
        base_58_input[sizeof(PRIVATE_KEY_PREFIX) + i] = seed_bn[sizeof(seed_bn) - 1 - i];
    }
    base58_encode(base_58_input, sizeof(PRIVATE_KEY_PREFIX) + sizeof(seed_bn), private_key, 64);
    PRINTF("%s\n", private_key);

    return status;
}

void account_init(void) {
    memset(&account, 0, sizeof(account));
}