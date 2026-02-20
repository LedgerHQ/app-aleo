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

#include <stdbool.h>  // bool

#include "crypto_helpers.h"

#include "validate.h"
#include "menu.h"
#include "sw.h"
#include "globals.h"
#include "send_response.h"
#include "signature.h"

void validate_address(bool choice)
{
    if (choice) {
        helper_send_response_get_address();
    }
    else {
        io_send_sw(SWO_PERMISSION_DENIED);
    }
}

void validate_view_key(bool choice)
{
    if (choice) {
        helper_send_response_get_view_key();
    }
    else {
        io_send_sw(SWO_PERMISSION_DENIED);
    }
}

void validate_transaction(bool choice)
{
    if (choice) {
        sign_prepared_request(&G_context.account,
                              &G_context.sign_transaction_datas.prepared_request);

        helper_send_response_sign_transaction();
    }
    else {
        io_send_sw(SWO_PERMISSION_DENIED);
    }
}
