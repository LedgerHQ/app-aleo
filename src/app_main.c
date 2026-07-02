/*****************************************************************************
 *   Ledger App Aleo.
 *   (c) 2025 Ledger SAS.
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

#include <stdint.h>  // uint*_t
#include <string.h>  // memset, explicit_bzero

#include "os.h"
#include "ux.h"
#include "swap.h"
#include "swap_error_code_helpers.h"

#include "types.h"
#include "globals.h"
#include "io.h"
#include "sw.h"
#include "menu.h"
#include "nbgl_use_case.h"
#include "dispatcher.h"
#include "account.h"
#include "sign_transaction.h"

global_ctx_t G_context;

const internal_storage_t N_storage_real;

#ifdef TEST_PRIVATE_KEY
static const char *test_private_key = {TEST_PRIVATE_KEY};
#endif  // !TEST_PRIVATE_KEY

static uint64_t time_ms = 0;

/**
 * Handle APDU command received and send back APDU response using handlers.
 */
void app_main(void)
{
    // Length of APDU command received in G_io_apdu_buffer
    int input_len = 0;
    // Structured APDU command
    command_t cmd;

    io_init();
    time_ms = 0;

    // When called in swap context as a library, we don't want to show the menu
    if (!G_called_from_swap) {
        ui_menu_main();
    }

    // Reset context
    explicit_bzero(&G_context, sizeof(G_context));
    sign_transaction_init();

    // Initialize the NVM data if required
    if (N_storage.initialized != 0x01) {
        internal_storage_t storage;
        explicit_bzero(&storage, sizeof(storage));
        storage.initialized = 0x01;
        nvm_write((void *) &N_storage, &storage, sizeof(internal_storage_t));
    }

    for (;;) {
        // Receive command bytes in G_io_apdu_buffer
        if ((input_len = io_recv_command()) < 0) {
            PRINTF("=> io_recv_command failure\n");
            return;
        }

        // Parse APDU command from G_io_apdu_buffer
        if (!apdu_parser(&cmd, G_io_apdu_buffer, input_len)) {
            PRINTF("=> /!\\ BAD LENGTH: %.*H\n", input_len, G_io_apdu_buffer);
            io_send_sw(SWO_WRONG_DATA_LENGTH);
            continue;
        }

        PRINTF("=> CLA=%02X | INS=%02X | P1=%02X | P2=%02X | Lc=%02X | CData=%.*H\n",
               cmd.cla,
               cmd.ins,
               cmd.p1,
               cmd.p2,
               cmd.lc,
               cmd.lc,
               cmd.data);

        // Dispatch structured APDU command to handler
        if (apdu_dispatcher(&cmd) < 0) {
            PRINTF("=> apdu_dispatcher failure\n");
            return;
        }
    }
}

void app_ticker_event_callback(void)
{
    time_ms += 100;
    if (G_context.signing_state > SIGNING_STATE_INTENT) {
        G_context.fees_waiting_time_ms += 100;
        if (G_context.fees_waiting_time_ms > 15 * 1000) {
            G_context.signing_state = SIGNING_STATE_WAIT_INTENT;
            account_erase(&G_context.account);
            r_list_erase();
#ifndef FUZZ
            if (!G_called_from_swap) {
                nbgl_useCaseReviewStatus(STATUS_TYPE_TRANSACTION_REJECTED, ui_menu_main);
            }
            else {
                send_swap_error_simple(
                    SW_SWAP_FAIL, SWAP_EC_ERROR_INTERNAL, SWAP_ERROR_CODE);
                // unreachable
                os_sched_exit(0);
            }
#endif  // FUZZ
        }
    }
    else if ((G_context.signing_state == SIGNING_STATE_WAIT_INTENT)
             && (G_context.r_list_alive_remaining_time_ms >= 100)) {
        G_context.r_list_alive_remaining_time_ms -= 100;
        if (G_context.r_list_alive_remaining_time_ms < 100) {
            G_context.r_list_alive_remaining_time_ms = 0;
            r_list_erase();
            if (!G_called_from_swap) {
                ui_menu_main();
            }
            else {
                send_swap_error_simple(
                    SW_SWAP_FAIL, SWAP_EC_ERROR_INTERNAL, SWAP_ERROR_CODE);
                // unreachable
                os_sched_exit(0);
            }
        }
    }
}
