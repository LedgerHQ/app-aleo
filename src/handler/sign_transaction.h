#pragma once

#include <stddef.h>   // size_t
#include <stdbool.h>  // bool
#include <stdint.h>   // uint*_t

#include "buffer.h"

#include "types.h"

/**
 * Handler for SIGN_TRANSACTION command.
 *
 * @see ?
 *
 * @param[in,out] cdata
 *   Command data with BIP32 path.
 *
 * @return zero or positive integer if success, negative integer otherwise.
 *
 */
int handler_sign_transaction(buffer_t *cdata, uint8_t mode, bool next_chunk);
int handler_get_tvk(buffer_t *cdata, uint8_t mode);

void sign_transaction_init(void);
