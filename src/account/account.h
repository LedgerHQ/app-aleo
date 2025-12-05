#pragma once

#include <stdint.h>   // uint*_t
#include <stddef.h>   // size_t
#include <stdbool.h>  // bool

#include "constants.h"
#include "scalar.h"
#include "group.h"

typedef struct {
    group_t pk_sig;
    group_t pr_sig;

} compute_key_t;

typedef struct {
    scalar_t challenge;
    scalar_t response;
    compute_key_t compute_key;

} account_signature_t;

void account_init(void);
int account_get_address_string(const uint32_t *path, uint8_t path_len, char address[ADDRESS_LEN]);
int account_get_view_key_string(const uint32_t *path, uint8_t path_len, char *view_key);
int account_get_private_key_string(const uint32_t *path, uint8_t path_len, char *private_key);
int account_sign(const uint32_t *path, uint8_t path_len, account_signature_t *signature);
