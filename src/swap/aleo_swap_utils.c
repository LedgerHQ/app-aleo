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
#include <string.h>

#include "aleo_swap_utils.h"

bool swap_str_to_u128(const char *in, size_t in_len, u128_t *out)
{
    if (!in || !out || in_len == 0 || in_len > 16) {
        return false;
    }

    uint64_t high = 0;
    uint64_t low  = 0;

    for (size_t i = 0; i < in_len; i++) {
        high = (high << 8) | (low >> 56);
        low  = (low << 8) | (uint64_t) in[i];
    }

    out->low  = low;
    out->high = high;

    return true;
}