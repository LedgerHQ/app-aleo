#pragma once

#include <stdint.h>   // uint*_t
#include <stdbool.h>  // bool
#include "types.h"    // bool

/* Check if the Tx to sign have the same parameters as the ones previously validated */
bool swap_check_validity(const tx_transfer_t *tx_transfer, const uint64_t max_fee);
