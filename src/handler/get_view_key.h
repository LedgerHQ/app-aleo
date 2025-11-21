#pragma once

#include <stddef.h>   // size_t
#include <stdbool.h>  // bool
#include <stdint.h>   // uint*_t

#include "buffer.h"

#include "types.h"

/**
 * Handler for CMD_GET_VIEW_KEY command. If successfully parse BIP32 path,
 * derive to view_key and send APDU response.
 *
 * @see G_context.bip32_path, G_context.view_key
 *
 * @param[in,out] cdata
 *   Command data with BIP32 path.
 *
 * @return zero or positive integer if success, negative integer otherwise.
 *
 */
int handler_get_view_key(buffer_t *cdata);
