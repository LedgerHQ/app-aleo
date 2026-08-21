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

#include <stdint.h>
#include <stdbool.h>

#include "buffer.h"
#include "io.h"
#include "ledger_assert.h"

#include "dispatcher.h"
#include "constants.h"
#include "globals.h"
#include "types.h"
#include "sw.h"
#include "get_version.h"
#include "get_app_name.h"
#include "get_address.h"
#include "get_view_key.h"
#include "sign_transaction.h"
#include "account.h"
#include "send_response.h"

int apdu_dispatcher(const command_t *cmd)
{
    LEDGER_ASSERT(cmd != NULL, "NULL cmd");

    if (cmd->cla != CLA) {
        return io_send_sw(SWO_INVALID_CLA);
    }

    buffer_t buf = {0};

    switch (cmd->ins) {
        case CMD_GET_VERSION:
            if (cmd->p1 != P1_GET_WITHOUT_CONFIRMATION || cmd->p2 != P2_BEGIN) {
                return io_send_sw(SWO_INCORRECT_P1_P2);
            }

            return handler_get_version();

        case CMD_GET_APP_NAME:
            if (cmd->p1 != P1_GET_WITHOUT_CONFIRMATION || cmd->p2 != P2_BEGIN) {
                return io_send_sw(SWO_INCORRECT_P1_P2);
            }

            return handler_get_app_name();

        case CMD_GET_ADDRESS:
            if (cmd->p1 > P1_GET_WITH_CONFIRMATION || cmd->p2 != P2_BEGIN) {
                return io_send_sw(SWO_INCORRECT_P1_P2);
            }

            if (!cmd->data) {
                return io_send_sw(SWO_WRONG_DATA_LENGTH);
            }

            buf.ptr    = cmd->data;
            buf.size   = cmd->lc;
            buf.offset = 0;

            return handler_get_address(&buf, (bool) cmd->p1);

        case CMD_GET_VIEW_KEY:
            if (cmd->p1 != P1_GET_WITH_CONFIRMATION || cmd->p2 != P2_BEGIN) {
                return io_send_sw(SWO_INCORRECT_P1_P2);
            }

            if (!cmd->data) {
                return io_send_sw(SWO_WRONG_DATA_LENGTH);
            }

            buf.ptr    = cmd->data;
            buf.size   = cmd->lc;
            buf.offset = 0;

            return handler_get_view_key(&buf);

        case CMD_SIGN_TRANSACTION:
            if ((cmd->p1 > P1_SIGN_MODE_FEE) || (cmd->p2 != P2_BEGIN && cmd->p2 != P2_CONTINUE)) {
                return io_send_sw(SWO_INCORRECT_P1_P2);
            }

            if (!cmd->data) {
                return io_send_sw(SWO_WRONG_DATA_LENGTH);
            }

            buf.ptr    = cmd->data;
            buf.size   = cmd->lc;
            buf.offset = 0;

            return handler_sign_transaction(&buf, cmd->p1, (bool) cmd->p2 == P2_CONTINUE);

        case CMD_GET_TVK:
            if ((cmd->p1 != P1_GET_TVK_SEED) && (cmd->p1 != P1_GET_TVK_DERIVED)) {
                return io_send_sw(SWO_INCORRECT_P1_P2);
            }

            if (!cmd->data) {
                return io_send_sw(SWO_WRONG_DATA_LENGTH);
            }

            buf.ptr    = cmd->data;
            buf.size   = cmd->lc;
            buf.offset = 0;

            return handler_get_tvk(&buf, cmd->p1);

        default:
            return io_send_sw(SWO_INVALID_INS);
    }
}
