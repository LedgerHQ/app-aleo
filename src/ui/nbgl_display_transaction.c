/*****************************************************************************
 *   Ledger App Boilerplate.
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
#include <string.h>   // memset

#include "os.h"
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
static char g_amount[30];
static char g_amount_2[30];

// The flow with the most pairs to display is the token signing flow with amount + dest + token
static nbgl_contentTagValue_t     pairs[3];
static nbgl_contentTagValueList_t pairList;

#ifdef HAVE_SE_TOUCH
static nbgl_content_t content[3];
#else   // ! HAVE_SE_TOUCH
static char text_buffer[64];
#endif  // !HAVE_SE_TOUCH

static void review_fees(bool confirm)
{
    // Answer, display a status page and go back to main
    validate_transaction(confirm);
    if (confirm) {
        nbgl_useCaseReviewStatus(STATUS_TYPE_TRANSACTION_SIGNED, ui_menu_main);
    }
    else {
        nbgl_useCaseReviewStatus(STATUS_TYPE_TRANSACTION_REJECTED, ui_menu_main);
    }
    G_context.signing_state = SIGNING_STATE_WAIT_INTENT;
}

static void confirm_cb(int token, uint8_t index, int page)
{
    UNUSED(token);
    UNUSED(index);
    UNUSED(page);
    validate_transaction(true);
    if (G_context.nested_call_count) {
        G_context.signing_state = SIGNING_STATE_WAIT_NESTED_CALL;
    }
    else {
        G_context.fees_waiting_time_ms = 0;
        G_context.signing_state        = SIGNING_STATE_WAIT_FEES;
        nbgl_useCaseSpinner("Calculating fees");
    }
}

static void confirm_reject_cb(void)
{
    nbgl_useCaseReviewStatus(STATUS_TYPE_TRANSACTION_REJECTED, ui_menu_main);
    G_context.signing_state = SIGNING_STATE_WAIT_INTENT;
}

#ifdef HAVE_SE_TOUCH
static void reject_cb(void)
{
    nbgl_useCaseConfirm(
        "Reject transaction?", NULL, "Yes, reject", "Go back to transaction", confirm_reject_cb);
}
#else   // ! HAVE_SE_TOUCH
static void review_tx(bool confirm)
{
    if (confirm) {
        confirm_cb(0, 0, 0);
    }
    else {
        confirm_reject_cb();
    }
}
#endif  // !HAVE_SE_TOUCH

static int display_transfer(void)
{
    uint8_t pair_index                   = 0;
    char    amount[50 + MAX_TICKER_SIZE] = {0};

    const char *review_subtext = NULL;

    if (G_context.tx.transfer.type == TX_TRANSFER_PUBLIC) {
        review_subtext = "Public transfer";
    }
    else if (G_context.tx.transfer.type == TX_TRANSFER_PRIVATE) {
        review_subtext = "Private transfer";
    }
    else if (G_context.tx.transfer.type == TX_TRANSFER_PUBLIC_TO_PRIVATE) {
        review_subtext = "Public to private transfer";
    }
    else if (G_context.tx.transfer.type == TX_TRANSFER_PRIVATE_TO_PUBLIC) {
        review_subtext = "Private to public transfer";
    }
    else {
        return io_send_sw(SWO_INCORRECT_DATA);
    }

    // Format amount
    // 50 chars is comfortable for amount formatting
    explicit_bzero(g_amount, sizeof(g_amount));
    if (!format_fpu64(
            amount, sizeof(amount), G_context.tx.transfer.amount, EXPONENT_SMALLEST_UNIT)) {
        return io_send_sw(SWO_INCORRECT_DATA);
    }
    snprintf(g_amount, sizeof(g_amount), "%.*s ALEO", (int) strlen(amount), amount);

    pairs[pair_index].item  = "Amount";
    pairs[pair_index].value = g_amount;
    pair_index++;

    // Add address
    pairs[pair_index].item  = "To";
    pairs[pair_index].value = G_context.tx.transfer.address_to;
    pair_index++;

    // Setup list
    pairList.nbMaxLinesForValue = 0;
    pairList.nbPairs            = pair_index;
    pairList.pairs              = pairs;
    pairList.wrapping           = true;

#ifdef HAVE_SE_TOUCH
    content[0].type                       = CENTERED_INFO;
    content[0].content.centeredInfo.text1 = "Review transaction to send ALEO";
    content[0].content.centeredInfo.text2 = review_subtext;
    content[0].content.centeredInfo.text3 = "Swipe to review";
    content[0].content.centeredInfo.onTop = false;
    content[0].content.centeredInfo.icon  = &ICON_APP_ALEO;
    content[0].content.centeredInfo.style = LARGE_CASE_GRAY_INFO;
    content[0].contentActionCallback      = NULL;

    content[1].type                       = CENTERED_INFO;
    content[1].content.centeredInfo.text1 = "Review amount and recipient";
    content[1].content.centeredInfo.text2 = "1 of 2";
    content[1].content.centeredInfo.text3 = "Swipe to continue";
    content[1].content.centeredInfo.onTop = false;
    content[1].content.centeredInfo.icon  = NULL;
    content[1].content.centeredInfo.style = LARGE_CASE_GRAY_INFO;
    content[1].contentActionCallback      = NULL;

    content[2].type                                      = TAG_VALUE_CONFIRM;
    content[2].content.tagValueConfirm.tagValueList      = pairList;
    content[2].content.tagValueConfirm.detailsButtonText = NULL;
    content[2].content.tagValueConfirm.detailsButtonIcon = NULL;
    content[2].content.tagValueConfirm.confirmationText  = "Confirm";
    content[2].content.tagValueConfirm.confirmationToken = 50;
    content[2].content.tagValueConfirm.tuneId            = TUNE_TAP_CASUAL;
    content[2].contentActionCallback                     = PIC(confirm_cb);

    nbgl_genericContents_t review_contents
        = {.callbackCallNeeded = false, .contentsList = content, .nbContents = 3};
    nbgl_useCaseGenericReview(&review_contents, "Reject", reject_cb);
#else   // !HAVE_SE_TOUCH
    snprintf(
        text_buffer, sizeof(text_buffer), "Review transaction to send ALEO\n%s", review_subtext);
    nbgl_useCaseReview(TYPE_TRANSACTION,
                       &pairList,
                       &ICON_APP_ALEO,
                       text_buffer,
                       "Review amount and recipient\n1 of 2",
                       "confirm",
                       review_tx);
#endif  // !HAVE_SE_TOUCH

    return 0;
}

static int display_fee(void)
{
    uint8_t     pair_index                   = 0;
    char        amount[50 + MAX_TICKER_SIZE] = {0};
    const char *review_subtext               = NULL;

    if (G_context.tx.transfer.type == TX_TRANSFER_PUBLIC) {
        review_subtext = "Sign transaction to send ALEO?\n(Public transfer)";
    }
    else if (G_context.tx.transfer.type == TX_TRANSFER_PRIVATE) {
        review_subtext = "Sign transaction to send ALEO?\n(Private transfer)";
    }
    else if (G_context.tx.transfer.type == TX_TRANSFER_PUBLIC_TO_PRIVATE) {
        review_subtext = "Sign transaction to send ALEO?\n(Public to private transfer)";
    }
    else if (G_context.tx.transfer.type == TX_TRANSFER_PRIVATE_TO_PUBLIC) {
        review_subtext = "Sign transaction to send ALEO?\n(Private to public transfer)";
    }
    else {
        return io_send_sw(SWO_INCORRECT_DATA);
    }

    explicit_bzero(g_amount, sizeof(g_amount));
    if (!format_fpu64(amount, sizeof(amount), G_context.tx.fee.base_fee, EXPONENT_SMALLEST_UNIT)) {
        return io_send_sw(SWO_INCORRECT_DATA);
    }
    snprintf(g_amount, sizeof(g_amount), "%.*s ALEO", (int) strlen(amount), amount);

    explicit_bzero(g_amount_2, sizeof(g_amount_2));
    if (!format_fpu64(
            amount, sizeof(amount), G_context.tx.fee.priority_fee, EXPONENT_SMALLEST_UNIT)) {
        return io_send_sw(SWO_INCORRECT_DATA);
    }
    snprintf(g_amount_2, sizeof(g_amount_2), "%.*s ALEO", (int) strlen(amount), amount);

    pairs[pair_index].item  = "Base fees";
    pairs[pair_index].value = g_amount;
    pair_index++;

    pairs[pair_index].item  = "Priority fees";
    pairs[pair_index].value = g_amount_2;
    pair_index++;

    // Setup list
    pairList.nbMaxLinesForValue = 0;
    pairList.nbPairs            = pair_index;
    pairList.pairs              = pairs;
    pairList.wrapping           = true;

#ifdef HAVE_SE_TOUCH
    nbgl_useCaseReview(TYPE_TRANSACTION,
                       &pairList,
                       &ICON_APP_ALEO,
                       "Review fees",
                       "2 of 2",
                       review_subtext,
                       review_fees);
#else   // !HAVE_SE_TOUCH
    nbgl_useCaseReview(TYPE_TRANSACTION,
                       &pairList,
                       &ICON_APP_ALEO,
                       "Review fees\n2 of 2",
                       NULL,
                       review_subtext,
                       review_fees);
#endif  // !HAVE_SE_TOUCH

    return 0;
}

static int ui_display_transactions(void)
{
    // TODO check bad state

    if (memcmp(G_context.sign_transaction_datas.fee_program_id,
               "credits.aleo",
               G_context.sign_transaction_datas.fee_program_id_length)) {
        // We currently don't support other token than ALEO
        return -1;
    }

    if (G_context.tx.type == TX_TRANSFER) {
        display_transfer();
    }
    else if (G_context.tx.type == TX_FEE) {
        display_fee();
    }
    else {
        return -1;
    }

    return 0;
}

// Flow used to display a clear-signed transaction
int ui_display_transaction(void)
{
    return ui_display_transactions();
}
