#pragma once

#include <stddef.h>   // size_t
#include <stdbool.h>  // bool
#include <stdint.h>   // uint*_t

#include "buffer.h"

#include "types.h"

/**
 * Handler for CMD_GET_ADDRESS command. If successfully parse BIP32 path,
 * derive to address and send APDU response.
 *
 * @see G_context.bip32_path, G_context.address
 *
 * @param[in,out] cdata
 *   Command data with BIP32 path.
 * @param[in]     display
 *   Whether to display and confirm address on screen or not.
 *
 * @return zero or positive integer if success, negative integer otherwise.
 *
 */
int handler_get_address(buffer_t *cdata, bool display);
