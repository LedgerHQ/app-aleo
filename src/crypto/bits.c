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

#include <stdint.h>   // uint*_t
#include <stddef.h>   // size_t
#include <stdbool.h>  // bool
#include <string.h>   // memmove

#include "os.h"
#include "cx.h"

#include "bits.h"

void bits_add_single(uint8_t *output, uint16_t output_bit_offset, bool bit)
{
    if (bit) {
        output[(output_bit_offset) / 8] |= (1 << ((output_bit_offset) % 8));
    }
    else {
        output[(output_bit_offset) / 8] &= ~(1 << ((output_bit_offset) % 8));
    }
}

void bits_add(const uint8_t *input,
              uint16_t       input_bit_offset,
              uint16_t       input_length,
              uint8_t       *output,
              uint16_t       output_offset)
{
    for (size_t i = 0; i < input_length; i++) {
        bool val = ((input[(input_bit_offset+i) / 8]) & (1 << ((input_bit_offset+i) % 8)));
        bits_add_single(output, output_offset + i, val);
    }
}

uint16_t bits_from_plaintext(const uint8_t *plaintext,
                             const uint8_t *plaintext_type,
                             uint16_t       plaintext_bit_length,
                             uint8_t       *out)
{
    uint16_t out_offset = 0;
    if (plaintext_type[0] == 0x00) {
        // Literal

        // Variant bits
        bits_add_single(out, out_offset, false);
        bits_add_single(out, out_offset + 1, false);
        out_offset = 2;

        // Variant bits
        bits_add(&plaintext_type[1], 0, 8, out, out_offset);
        out_offset += 8;

        // Size in bits
        uint8_t data = (uint8_t) plaintext_bit_length;
        bits_add(&data, 0, 8, out, out_offset);
        out_offset += 8;
        data = (uint8_t) (plaintext_bit_length >> 8);
        bits_add(&data, 0, 8, out, out_offset);
        out_offset += 8;

        // Value
        bits_add((uint8_t *) plaintext, 0, plaintext_bit_length, out, out_offset);
        out_offset += plaintext_bit_length;
    }
    else {
    }
    return out_offset;
}

void bits_print(const uint8_t *input, uint16_t input_bit_length)
{
    for (size_t i = 0; i < input_bit_length; i++) {
        if ((input[i / 8]) & (1 << (i % 8))) {
            PRINTF("true, ");
        }
        else {
            PRINTF("false, ");
        }
    }
}

void bits_println(const uint8_t *input, uint16_t input_bit_length)
{
    bits_print(input, input_bit_length);
    PRINTF("\n");
}
