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

#include <stddef.h>  // size_t
#include <stdint.h>  // uint*_t
#include <string.h>  // memmove

#include "buffer.h"

#include "send_response.h"
#include "constants.h"
#include "globals.h"
#include "sw.h"

int helper_send_response_get_address(void) {
    uint8_t resp[1 + ADDRESS_LEN] = {0};
    size_t offset = 0;

    resp[offset++] = ADDRESS_LEN;
    memmove(resp + offset, G_context.address, ADDRESS_LEN);
    offset += ADDRESS_LEN;

    return io_send_response_pointer(resp, offset, SW_OK);
}

int helper_send_response_get_view_key(void)
{
    uint8_t resp[1 + VIEW_KEY_LEN] = {0};
    size_t offset = 0;

    resp[offset++] = VIEW_KEY_LEN;
    memmove(resp + offset, G_context.view_key, VIEW_KEY_LEN);
    offset += VIEW_KEY_LEN;

    return io_send_response_pointer(resp, offset, SW_OK);
}

int helper_send_response_get_private_key(void)
{
    uint8_t resp[1 + PRIVATE_KEY_LEN] = {0};
    size_t offset = 0;

    resp[offset++] = PRIVATE_KEY_LEN;
    memmove(resp + offset, G_context.private_key, PRIVATE_KEY_LEN);
    offset += PRIVATE_KEY_LEN;

    return io_send_response_pointer(resp, offset, SW_OK);
}
