#pragma once

#include <stdint.h>   // uint*_t
#include <stdbool.h>  // bool
#include "types.h"    // bool

typedef struct swap_validated_s {
    bool     initialized;
    u128_t   amount;
    uint64_t max_fee;
    char     account_address[ADDRESS_LEN + 1];
    char     recipient[ADDRESS_LEN + 1];
    char     ticker[MAX_TICKER_SIZE + 1];
    uint8_t  decimals;
} swap_validated_t;

extern swap_validated_t G_swap_validated;

// Check if the Tx to sign have the same parameters as the ones previously validated
bool swap_check_validity(char                 account_address[ADDRESS_LEN + 1],
                         const tx_transfer_t *tx_transfer,
                         const uint64_t       max_fee);
