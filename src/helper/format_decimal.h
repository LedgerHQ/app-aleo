#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

// Maximum input byte length supported by format_decimal_le. 32 covers u128
// (16) and field (32). Anything larger is rejected.
#define FORMAT_DECIMAL_MAX_BYTES (32)

// Format a little-endian unsigned integer of arbitrary byte length as a
// NUL-terminated decimal string.
//
// dst       — output buffer.
// dst_len   — capacity of dst (must include room for NUL).
// bytes     — input bytes in little-endian order.
// byte_len  — number of input bytes, in [1, FORMAT_DECIMAL_MAX_BYTES].
//
// Returns true on success, false if any argument is invalid or dst_len is
// too small to hold the result (in which case dst is left untouched).
bool format_decimal_le(char *dst, size_t dst_len, const uint8_t *bytes, size_t byte_len);
