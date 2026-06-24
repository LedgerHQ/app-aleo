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

#include "format_u128.h"

bool format_u128(char *out, size_t out_len, u128_t in)
{
    size_t i = 0;

    uint64_t high = in.high;
    uint64_t low  = in.low;

    if (!out || (out_len < 2)) {
        return false;
    }

    if (!high && !low) {
        out[0] = '0';
        out[1] = '\0';
        return true;
    }

    while (high || low) {
        uint64_t mid   = ((high % 10) << 32) | (low >> 32);
        uint64_t inter = ((mid % 10) << 32) | (low & 0xFFFFFFFF);

        out[i++] = (char) ('0' + (inter % 10));

        if (i >= out_len - 1) {
            return false;
        }
        high /= 10;
        low = ((mid / 10) << 32) | (inter / 10);
    }
    out[i] = '\0';

    // Reverse the buffer to get the correct digit order
    for (size_t j = 0; j < i / 2; j++) {
        char tmp       = out[j];
        out[j]         = out[i - 1 - j];
        out[i - 1 - j] = tmp;
    }

    return true;
}

bool format_fpu128(char *dst, size_t dst_len, const u128_t value, uint8_t decimals)
{
    // U128 max is 340282366920938463463374607431768211455 (39 digits)
    char buffer[40] = {0};

    if (!format_u128(buffer, sizeof(buffer), value)) {
        return false;
    }

    size_t digits = strlen(buffer);

    if (decimals == 0) {
        if (dst_len <= digits) {
            return false;
        }
        strncpy(dst, buffer, dst_len);
        return true;
    }

    if (digits <= decimals) {
        // Result is "0.000...XXX"
        if (dst_len <= (2 + decimals)) {
            return false;
        }
        *dst++ = '0';
        *dst++ = '.';
        for (size_t j = 0; j < decimals - digits; j++, dst++) {
            *dst = '0';
        }
        dst_len -= 2 + decimals - digits;
        strncpy(dst, buffer, dst_len);
    }
    else {
        // Result is "XXX.YYY"
        if (dst_len <= (digits + 1)) {
            return false;
        }
        const size_t shift = digits - decimals;
        memmove(dst, buffer, shift);
        dst[shift] = '.';
        strncpy(dst + shift + 1, buffer + shift, decimals);
    }

    return true;
}
