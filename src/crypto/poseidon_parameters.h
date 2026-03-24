#pragma once

#include <stdint.h>  // uint*_t

#include "field.h"

typedef struct {
    uint8_t full_rounds;
    uint8_t partial_rounds;

    uint8_t alpha;

    field_t *ark;
    uint16_t ark_size;
    field_t *mds;
    uint16_t mds_size;

} poseidon_parameters_t;

extern void poseidon_parameters_init(poseidon_parameters_t *parameters, uint8_t rate);
