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
    char          address_str[ADDRESS_LEN + 1];
} account_t;

typedef struct {
    scalar_t      challenge;
    scalar_t      response;
    compute_key_t compute_key;
} account_signature_t;

typedef enum {
    R_LIST_MODE_TVK_SEED    = 0x00,
    R_LIST_MODE_TVK_DERIVED = 0x01,
} r_list_mode_e;

typedef struct {
    scalar_t array[R_LIST_MAX_LENGTH];
    size_t   count;
    size_t   index;
} r_list_t;

extern const char ADDRESS_PREFIX[5];

int account_get_address_string(const uint32_t *path,
                               uint8_t         path_len,
                               char            address[ADDRESS_LEN + 1]);
int account_get_view_key_string(const uint32_t *path,
                                uint8_t         path_len,
                                char            viewkey[VIEW_KEY_LEN + 1]);

int account_generate_keys(const uint32_t *path, uint8_t path_len, account_t *account);

void account_erase(account_t *account);

int  r_list_set(account_t *account, uint8_t index);
int  r_list_get(uint8_t index, scalar_t *r);
int  r_list_get_tvk(account_t *account, uint8_t index, field_t *tvk);
void r_list_erase(void);
