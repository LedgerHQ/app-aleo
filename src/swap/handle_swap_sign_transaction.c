#include "swap.h"
#include "swap_error_code_helpers.h"
#include "os.h"
#include "format.h"

#include "sw.h"
#include "globals.h"
#include "tx_types.h"
#include "handle_swap.h"
#include "tokens.h"

#include <stdint.h>

typedef struct swap_validated_s {
    bool     initialized;
    u128_t   amount;
    uint64_t max_fee;
    char     recipient[ADDRESS_LEN + 1];
    char     ticker[MAX_TICKER_SIZE + 1];
    uint8_t  decimals;
} swap_validated_t;

static swap_validated_t G_swap_validated;

bool swap_copy_transaction_parameters(create_transaction_parameters_t *params)
{
    PRINTF("Inside swap_copy_transaction_parameters\n");

    if (!params) {
        return false;
    }

    if (params->destination_address == NULL) {
        PRINTF("Destination address expected\n");
        return false;
    }

    if (strlen(params->destination_address) != ADDRESS_LEN) {
        PRINTF("Destination address wrong length %d - %s\n",
               strlen(params->destination_address),
               params->destination_address);
        return false;
    }

    if (params->amount == NULL) {
        PRINTF("Amount expected\n");
        return false;
    }

    // first copy parameters to stack, and then to global data.
    // We need this "trick" as the input data position can overlap with app globals
    // and also because we want to memset the whole bss segment as it is not done
    // when an app is called as a lib.
    // This is necessary as many part of the code expect bss variables to
    // initialized at 0.
    swap_validated_t swap_validated;
    explicit_bzero(&swap_validated, sizeof(swap_validated));

    memcpy(swap_validated.recipient, params->destination_address, ADDRESS_LEN);
    swap_validated.recipient[ADDRESS_LEN] = '\0';

    PRINTF("Validated recipient: %s\n", swap_validated.recipient);

    // Parse config and save decimals and ticker
    // If there is no coin_configuration, consider that we are doing an ALEO swap
    if (params->coin_configuration == NULL) {
        memcpy(swap_validated.ticker, ALEO_TICKER, sizeof(ALEO_TICKER));
        swap_validated.decimals = ALEO_DECIMALS;
    }
    else {
        if (!swap_parse_config(params->coin_configuration,
                               params->coin_configuration_length,
                               swap_validated.ticker,
                               sizeof(swap_validated.ticker),
                               &swap_validated.decimals)) {
            PRINTF("Fail to parse coin_configuration\n");
            return false;
        }
    }
    PRINTF("Validated ticker: %s, decimals: %d\n", swap_validated.ticker, swap_validated.decimals);

    // Save amount
    if (!swap_str_to_u128(
            (const char *) params->amount, params->amount_length, &swap_validated.amount)) {
        PRINTF("Failed to convert amount to u128_t\n");
        return false;
    }

    // Save fee
    if (!swap_str_to_u64(params->fee_amount, params->fee_amount_length, &swap_validated.max_fee)) {
        PRINTF("Failed to convert fee to uint64_t\n");
        return false;
    }
    PRINTF("Validated max fee: %.*H\n", sizeof(swap_validated.max_fee), &swap_validated.max_fee);

    swap_validated.initialized = true;

    // Full reset the global variables
    os_explicit_zero_BSS_segment();

    // Commit from stack to global data, params becomes tainted but we won't access it anymore
    memcpy(&G_swap_validated, &swap_validated, sizeof(swap_validated));

    return true;
}

bool swap_check_validity(const tx_transfer_t *tx_transfer, const uint64_t max_fee)
{
    PRINTF("Inside swap_check_validity\n");

    if (!tx_transfer) {
        return false;
    }

    if (!G_swap_validated.initialized) {
        PRINTF("Swap structure is not initialized\n");
        send_swap_error_simple(SW_SWAP_FAIL, SWAP_EC_ERROR_GENERIC, SWAP_ERROR_CODE);
        // unreachable
        os_sched_exit(0);
    }

    token_display_info_t *token_info = tx_transfer->token_info;

    if ((strcmp(G_swap_validated.ticker, token_info->ticker) != 0)
        || (G_swap_validated.decimals != token_info->decimals)) {
        PRINTF("Token info does not match\n");
        PRINTF(
            "Validated: %s (decimals: %d)\n", G_swap_validated.ticker, G_swap_validated.decimals);
        PRINTF("Received: %s (decimals: %d)\n", token_info->ticker, token_info->decimals);
        send_swap_error_simple(
            SW_SWAP_FAIL, SWAP_EC_ERROR_WRONG_AMOUNT, SWAP_ERROR_WRONG_TOKEN_INFO);
        // unreachable
        os_sched_exit(0);
    }
    else {
        PRINTF("Token match \n");
    }

    if ((G_swap_validated.amount.low != tx_transfer->amount.low)
        || (G_swap_validated.amount.high != tx_transfer->amount.high)) {
        PRINTF(
            "Amount does not match, promised low : %lld - high : %lld, received low : %lld - high "
            ": %lld\n",
            G_swap_validated.amount.low,
            G_swap_validated.amount.high,
            tx_transfer->amount.low,
            tx_transfer->amount.high);
        send_swap_error_simple(SW_SWAP_FAIL, SWAP_EC_ERROR_WRONG_AMOUNT, SWAP_ERROR_CODE);
        // unreachable
        os_sched_exit(0);
    }
    else {
        PRINTF("Amounts match \n");
    }

    if (G_swap_validated.max_fee < max_fee) {
        PRINTF("Max fee does not match, promised %lld, received %lld\n",
               G_swap_validated.max_fee,
               max_fee);
        send_swap_error_simple(SW_SWAP_FAIL, SWAP_EC_ERROR_WRONG_FEES, SWAP_ERROR_CODE);
        // unreachable
        os_sched_exit(0);
    }
    else {
        PRINTF("Max fee match \n");
    }

    if (strcmp(G_swap_validated.recipient, tx_transfer->address_to) != 0) {
        PRINTF("Destination does not match\n");
        PRINTF("Validated: %s\n", G_swap_validated.recipient);
        PRINTF("Received: %s \n", tx_transfer->address_to);
        send_swap_error_simple(SW_SWAP_FAIL, SWAP_EC_ERROR_WRONG_DESTINATION, SWAP_ERROR_CODE);
        // unreachable
        os_sched_exit(0);
    }
    else {
        PRINTF("Destination is valid\n");
    }

    return true;
}
