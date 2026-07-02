/*****************************************************************************
 *   Ledger App Aleo.
 *   (c) 2020 Ledger SAS.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *****************************************************************************/

#include <stdbool.h>  // bool
#include <stdint.h>   // uint*_t
#include <string.h>   // memset

#include "os.h"
#include "swap.h"
#include "glyphs.h"
#include "nbgl_use_case.h"
#include "io.h"
#include "bip32.h"
#include "format.h"

#include "display.h"
#include "constants.h"
#include "globals.h"
#include "sw.h"
#include "validate.h"
#include "types.h"
#include "menu.h"
#include "tokens.h"

// Buffer where the transaction amount string is written
static char g_amount[MAX_AMOUNT_SIZE + 1 + MAX_TICKER_SIZE];
static char g_amount_fees[MAX_AMOUNT_SIZE + 4];  // Ticker is ALEO for fees
static char g_finish_title[27 + 1 + MAX_TICKER_SIZE];
static char g_review_title[29 + 1 + MAX_TICKER_SIZE];

// The flow with the most pairs to display is the token signing flow with amount + dest + token
static nbgl_contentTagValue_t     pairs[3];
static nbgl_contentTagValueList_t pairList;

static void review_transaction(bool confirm)
{
    validate_transaction(confirm);
    if (confirm) {
        if (G_context.nested_call_count) {
            G_context.signing_state = SIGNING_STATE_WAIT_NESTED_CALL;
        }
        else if ((G_context.sign_transaction_datas.max_base_fee != 0)
                 || (G_context.sign_transaction_datas.max_priority_fee != 0)) {
            G_context.fees_waiting_time_ms = 0;
            G_context.signing_state        = SIGNING_STATE_WAIT_FEES;
            r_list_erase();
#ifndef FUZZ
            if (!G_called_from_swap) {
                nbgl_useCaseSpinner("Calculating fees");
            }
#endif  // FUZZ
        }
        else {
            account_erase(&G_context.account);
            r_list_erase();
#ifndef FUZZ
            if (!G_called_from_swap) {
                nbgl_useCaseReviewStatus(STATUS_TYPE_TRANSACTION_SIGNED, ui_menu_main);
            }
#endif  // FUZZ
            G_context.signing_state = SIGNING_STATE_WAIT_INTENT;
        }
    }
    else {
        account_erase(&G_context.account);
        r_list_erase();
#ifndef FUZZ
        if (!G_called_from_swap) {
            nbgl_useCaseReviewStatus(STATUS_TYPE_TRANSACTION_REJECTED, ui_menu_main);
        }
#endif  // FUZZ
    }
}

// Flow used to display a clear-signed transaction
int ui_display_transaction(void)
{
    uint8_t     pair_index              = 0;
    char        amount[MAX_AMOUNT_SIZE] = {0};
    const char *review_subtitle         = NULL;

    if ((G_context.tx.type == TX_ALEO_TRANSFER_PUBLIC)
        || (G_context.tx.type == TX_TOKEN_TRANSFER_PUBLIC)) {
        review_subtitle = "Public transfer";
    }
    else if ((G_context.tx.type == TX_ALEO_TRANSFER_PRIVATE)
             || (G_context.tx.type == TX_TOKEN_TRANSFER_PRIVATE)) {
        review_subtitle = "Private transfer";
    }
    else if ((G_context.tx.type == TX_ALEO_TRANSFER_BATCH_PRIVATE)
             || (G_context.tx.type == TX_TOKEN_TRANSFER_BATCH_PRIVATE)) {
        review_subtitle = "Private batch transfer";
    }
    else if ((G_context.tx.type == TX_ALEO_TRANSFER_PRIVATE_TO_PUBLIC)
             || (G_context.tx.type == TX_TOKEN_TRANSFER_PRIVATE_TO_PUBLIC)) {
        review_subtitle = "Transfer from private to public address";
    }
    else if ((G_context.tx.type == TX_ALEO_TRANSFER_BATCH_PRIVATE_TO_PUBLIC)
             || (G_context.tx.type == TX_TOKEN_TRANSFER_BATCH_PRIVATE_TO_PUBLIC)) {
        review_subtitle = "Batch transfer from private to public address";
    }
    else if ((G_context.tx.type == TX_ALEO_TRANSFER_PUBLIC_TO_PRIVATE)
             || (G_context.tx.type == TX_TOKEN_TRANSFER_PUBLIC_TO_PRIVATE)) {
        review_subtitle = "Transfer from public to private address";
    }
    else {
        return -1;
    }

    // Format amount
    // 50 chars is comfortable for amount formatting
    explicit_bzero(g_amount, sizeof(g_amount));
    if (!format_fpu128(amount,
                       sizeof(amount),
                       G_context.tx.transfer.amount,
                       G_context.tx.transfer.token_info->decimals)) {
        return -1;
    }
    snprintf(g_amount,
             sizeof(g_amount),
             "%.*s %s",
             (int) strlen(amount),
             amount,
             G_context.tx.transfer.token_info->ticker);

    uint64_t max_base_fee     = (uint64_t) G_context.sign_transaction_datas.max_base_fee;
    uint64_t max_priority_fee = (uint64_t) G_context.sign_transaction_datas.max_priority_fee;
    uint64_t total_fees       = max_base_fee + max_priority_fee;
    explicit_bzero(g_amount_fees, sizeof(g_amount_fees));
    if (!format_fpu64(amount, sizeof(amount), total_fees, ALEO_DECIMALS)) {
        return -1;
    }
    snprintf(
        g_amount_fees, sizeof(g_amount_fees), "%.*s " ALEO_TICKER, (int) strlen(amount), amount);

    // Amount
    pairs[pair_index].item  = "Amount";
    pairs[pair_index].value = g_amount;
    pair_index++;

    // To address
    pairs[pair_index].item = "To";
    if (strlen(G_context.tx.transfer.address_to) > 0) {
        pairs[pair_index].value = G_context.tx.transfer.address_to;
    }
    else {
        pairs[pair_index].value = G_context.account.address_str;
    }
    pair_index++;

    // Fees
    pairs[pair_index].item  = "Fees";
    pairs[pair_index].value = g_amount_fees;
    pair_index++;

    // Setup list
    pairList.nbMaxLinesForValue = 0;
    pairList.nbPairs            = pair_index;
    pairList.pairs              = pairs;
    pairList.wrapping           = true;

    snprintf(g_review_title,
             sizeof(g_review_title),
             "Review transaction to send %s?",
             G_context.tx.transfer.token_info->ticker);

    snprintf(g_finish_title,
             sizeof(g_finish_title),
             "Sign transaction to send %s?",
             G_context.tx.transfer.token_info->ticker);

    if (G_called_from_swap) {
        review_transaction(swap_check_validity(&G_context.tx.transfer, total_fees));
        return 0;
    }

#ifdef HAVE_SE_TOUCH
    nbgl_useCaseReview(TYPE_TRANSACTION,
                       &pairList,
                       &ICON_APP_ALEO,
                       g_review_title,
                       review_subtitle,
                       g_finish_title,
                       review_transaction);
#else   // !HAVE_SE_TOUCH
    nbgl_useCaseReview(TYPE_TRANSACTION,
                       &pairList,
                       &ICON_APP_ALEO,
                       g_review_title,
                       review_subtitle,
                       "Sign transaction",
                       review_transaction);
#endif  // HAVE_SE_TOUCH

    return 0;
}
