#pragma once

#include <stdint.h>  // uint*_t

#include "field.h"

extern void bhp_512_hash_commitment(const field_t *commitment, field_t *hash);
