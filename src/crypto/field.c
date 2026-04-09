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
#include "ledger_assert.h"

#include "bits.h"

#include "field.h"

const field_t FIELD_ZERO = {
    .big.u64 = {0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000}
};
const field_t FIELD_ONE = {
    .big.u64 = {0x7d1c7ffffffffff3, 0x7257f50f6ffffff2, 0x16d81575512c0fee, 0x0d4bda322bbb9a9d}
};

// BLS12-377
static const fp256_parameters_t field_parameters = {
    .MODULUS_BITS = FIELD_MODULUS_BITS,
    // 8444461749428370424248824938781546531375899335154063827935233455917409239041
    .MODULUS
    = {.big.u64
       = {725501752471715841U, 6461107452199829505U, 6968279316240510977U, 1345280370688173398U}},
    .R
    = {.big.u64
       = {9015221291577245683U, 8239323489949974514U, 1646089257421115374U, 958099254763297437U}},
    .R2
    = {.big.u64
       = {2726216793283724667U, 14712177743343147295U, 12091039717619697043U, 81024008013859129U}},
    .INV = 725501752471715839U};

void field_assign(field_t *a, const field_t *b)
{
    memcpy(a, b, sizeof(field_t));
}

void field_add_assign(field_t *a, const field_t *b)
{
    fp256_add_assign(&field_parameters, a, b);
}

void field_mul_assign(field_t *a, const field_t *b)
{
    fp256_mul_assign(&field_parameters, a, b);
}

void field_inverse_assign(field_t *a)
{
    fp256_inverse_assign(&field_parameters, a);
}

void field_pow_assign(field_t *a, uint8_t alpha)
{
    fp256_pow_assign(&field_parameters, a, alpha);
}

void field_sum_of_products(const field_t *a, const field_t *b, uint8_t length, field_t *r)
{
    fp256_sum_of_products(&field_parameters, a, b, length, r);
}

void field_from_int(field_t *a, uint64_t i)
{
    fp256_from_int(&field_parameters, a, i);
}

void field_to_big_int(const field_t *a, bigint_256_t *bigint)
{
    fp256_to_big_int(&field_parameters, a, bigint);
}

void field_from_big_int(field_t *a, const bigint_256_t *bigint)
{
    fp256_from_big_int(&field_parameters, a, bigint);
}

uint8_t field_from_bits(const uint8_t *input_bits,
                        const uint16_t input_bits_length,
                        field_t       *r,
                        uint8_t        max_field_count)
{
    bigint_256_t s;
    uint8_t      bn[32];
    uint8_t      field_count       = 0;
    uint16_t     input_bits_offset = 0;

    LEDGER_ASSERT(input_bits != NULL, "NULL input_bits");
    LEDGER_ASSERT(r != NULL, "NULL r");

    while ((input_bits_offset < input_bits_length) && (field_count < max_field_count)) {
        memset(bn, 0, sizeof(bn));
        if ((input_bits_length - input_bits_offset) >= (FIELD_MODULUS_BITS - 1)) {
            if (bits_add(
                    input_bits, input_bits_offset, (FIELD_MODULUS_BITS - 1), bn, 0, sizeof(bn) * 8)
                < 0) {
                return 0;
            }
            input_bits_offset += (FIELD_MODULUS_BITS - 1);
        }
        else {
            if (bits_add(input_bits,
                         input_bits_offset,
                         (input_bits_length - input_bits_offset),
                         bn,
                         0,
                         sizeof(bn) * 8)
                < 0) {
                return 0;
            }
            input_bits_offset = input_bits_length;
        }
        bn_reverse(bn);
        bn_to_big_int(bn, &s);
        field_from_big_int(&r[field_count], &s);
        field_count++;
    }

    return field_count;
}

int field_random(field_t *a)
{
    return fp256_random(&field_parameters, a);
}

#ifdef HAVE_PRINTF
void field_print(const field_t *a)
{
    big_int_print(&a->big);
}

void field_println(const field_t *a)
{
    big_int_println(&a->big);
}

void field_print_array(const field_t *array, size_t length)
{
    for (uint16_t index = 0; index < length; index++) {
        PRINTF("[%2d] : ", index);
        big_int_print(&array[index].big);
        PRINTF(" field\n");
    }
}
#endif  // HAVE_PRINTF
