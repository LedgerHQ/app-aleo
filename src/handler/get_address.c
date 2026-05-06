/*****************************************************************************
 *   Ledger App ALEO.
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

#include <stdint.h>   // uint*_t
#include <stdbool.h>  // bool
#include <stddef.h>   // size_t
#include <string.h>   // memset, explicit_bzero

#include "os.h"
#include "cx.h"
#include "ledger_assert.h"
#include "io.h"
#include "buffer.h"
#include "crypto_helpers.h"

#include "globals.h"
#include "types.h"
#include "sw.h"
#include "display.h"
#include "send_response.h"
#include "account.h"

int handler_get_address(buffer_t *cdata, bool display)
{
    LEDGER_ASSERT(cdata != NULL, "NULL cdata");

    explicit_bzero(&G_context, sizeof(G_context));
    G_context.req_type = CONFIRM_ADDRESS;
    G_context.state    = STATE_NONE;

    if (!buffer_read_u8(cdata, &G_context.bip32_path_len)
        || !buffer_read_bip32_path(
            cdata, G_context.bip32_path, (size_t) G_context.bip32_path_len)) {
        return io_send_sw(SWO_WRONG_DATA_LENGTH);
    }

    int status = account_get_address_string(
        G_context.bip32_path, G_context.bip32_path_len, G_context.address);

    if (status < 0) {
        return io_send_sw(SW_DISPLAY_BIP32_PATH_FAIL);
    }

    if (display) {
        return ui_display_address();
    }

    return helper_send_response_get_address();
}
