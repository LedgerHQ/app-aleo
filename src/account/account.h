#pragma once

#include <stdint.h>   // uint*_t
#include <stddef.h>   // size_t
#include <stdbool.h>  // bool

#include "constants.h"

void account_init(void);
int account_get_address_string(const uint32_t *path, uint8_t path_len, char address[ADDRESS_LEN]);
int account_get_view_key_string(const uint32_t *path, uint8_t path_len, char *view_key);
int account_get_private_key_string(const uint32_t *path, uint8_t path_len, char *private_key);
