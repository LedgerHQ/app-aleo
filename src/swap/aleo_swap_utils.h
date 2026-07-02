#pragma once

#include <stdint.h>   // uint*_t
#include <stdbool.h>  // bool

#include "types.h"

bool swap_str_to_u128(const char *in, size_t in_len, u128_t *out);
