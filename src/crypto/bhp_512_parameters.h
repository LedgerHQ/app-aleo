#pragma once

#include <stdint.h>  // uint*_t

#include "scalar.h"
#include "group.h"

#define BHP_512_NUM_WINDOWS (6)
#define BHP_512_WINDOW_SIZE (43)

// lookup
extern const bn_group_t bhp_512_lookup_base_bn[BHP_512_NUM_WINDOWS*BHP_512_WINDOW_SIZE*8];
extern const bn_group_t bhp_512_lookup_base_light_bn[90*8];

// random
extern const bn_group_t bhp_512_random_base_bn[SCALAR_MODULUS_BITS];
