#pragma once

#include <stdint.h>   // uint*_t
#include <stddef.h>   // size_t
#include <stdbool.h>  // bool

#include "constants.h"
#include "field.h"
#include "scalar.h"
#include "group.h"

typedef struct {
    group_t  pk_sig;
    group_t  pr_sig;
    scalar_t sk_prf;

} compute_key_t;

typedef struct {
    field_t  seed;
    scalar_t sk_sig;
    scalar_t r_sig;

} private_key_t;

typedef struct {
    private_key_t private_key;
    compute_key_t compute_key;
    scalar_t      view_key;
    group_t       address;
    field_t       graph_key;

} account_t;

typedef struct {
    scalar_t      challenge;
    scalar_t      response;
    compute_key_t compute_key;

} account_signature_t;

extern const char ADDRESS_PREFIX[5];

int  account_get_address_string(const uint32_t *path, uint8_t path_len, char address[ADDRESS_LEN]);
int  account_get_view_key_string(const uint32_t *path, uint8_t path_len, char *view_key);

int account_generate_keys(const uint32_t *path, uint8_t path_len, account_t *account);

#ifdef ENABLE_PRIVATE_KEY_MANAGEMENT
int account_get_private_key_string(const uint32_t *path, uint8_t path_len, char *private_key);
#endif  // ENABLE_PRIVATE_KEY_MANAGEMENT
