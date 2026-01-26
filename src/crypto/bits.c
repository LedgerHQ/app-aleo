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

void bits_add_single(uint8_t *input, uint16_t input_bit_offset, bool bit)
{
    if (bit) {
        input[(input_bit_offset) / 8] |= (1 << ((input_bit_offset) % 8));
    }
    else {
        input[(input_bit_offset) / 8] &= ~(1 << ((input_bit_offset) % 8));
    }
}

void bits_add(uint8_t *input, uint16_t input_bit_offset, uint8_t *data, uint16_t data_bit_length)
{
    for (size_t i = 0; i < data_bit_length; i++) {
        bits_add_single(input, input_bit_offset + i, ((data[i / 8]) & (1 << (i % 8))));
    }
}

void bits_print(uint8_t *input, uint16_t input_bit_length)
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

void bits_println(uint8_t *input, uint16_t input_bit_length)
{
    bits_print(input, input_bit_length);
    PRINTF("\n");
}
