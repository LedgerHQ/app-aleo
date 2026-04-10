#pragma once

#include <stddef.h>  // size_t
#include <stdint.h>  // uint*_t
#include "buffer.h"

int io_legacy_apdu_tx(const unsigned char *buffer, unsigned short length);
