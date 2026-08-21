#pragma once

#include <stdint.h>  // uint*_t
#include <stddef.h>  // size_t

#include "types.h"

bool format_u128(char *out, size_t out_len, u128_t in);
bool format_fpu128(char *dst, size_t dst_len, const u128_t value, uint8_t decimals);
