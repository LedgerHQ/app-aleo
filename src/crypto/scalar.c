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

#include "scalar.h"

// BLS12-377
static const fp256_parameters_t scalar_parameters = {
    .MODULUS_BITS = SCALAR_MODULUS_BITS,
 // 2111115437357092606062206234695386632838870926408408195193685246394721360383
    .MODULUS = {.big.u64 = 13356249993388743167U,
                5950279507993463550U,  10965441865914903552U,
                336320092672043349U},
    .R       = {.big.u64 = 16632263305389933622U,
                10726299895124897348U, 16608693673010411502U,
                285459069419210737U},
    .R2      = {.big.u64 = 3987543627614508126U,
                17742427666091596403U, 14557327917022607905U,
                322810149704226881U},
    .INV     = 9659935179256617473U
};

void scalar_to_big_int(const scalar_t *a, bigint_256_t *bigint)
{
    fp256_to_big_int(&scalar_parameters, a, bigint);
}

void scalar_from_big_int(scalar_t *a, const bigint_256_t *bigint)
{
    fp256_from_big_int(&scalar_parameters, a, bigint);
}

void scalar_from_field_lossy(scalar_t *a, const field_t *f)
{
    bigint_256_t bigint;
    field_to_big_int(f, &bigint);
    big_int_bit_reduce(&bigint, scalar_parameters.MODULUS_BITS - 1);
    scalar_from_big_int(a, &bigint);
}

void scalar_to_field(const scalar_t *a, field_t *f)
{
    bigint_256_t bigint;
    scalar_to_big_int(a, &bigint);
    field_from_big_int(f, &bigint);
}

void scalar_add_assign(scalar_t *a, const scalar_t *b)
{
    fp256_add_assign(&scalar_parameters, a, b);
}

void scalar_sub_assign(scalar_t *a, const scalar_t *b)
{
    fp256_sub_assign(&scalar_parameters, a, b);
}

void scalar_mul_assign(scalar_t *a, const scalar_t *b)
{
    fp256_mul_assign(&scalar_parameters, a, b);
}

int scalar_random(scalar_t *a)
{
    return fp256_random(&scalar_parameters, a);
}

#ifdef HAVE_PRINTF
void scalar_print(const scalar_t *a)
{
    big_int_print(&a->big);
}

void scalar_println(const scalar_t *a)
{
    big_int_println(&a->big);
}

void scalar_print_array(const scalar_t *array, size_t length)
{
    for (uint16_t index = 0; index < length; index++) {
        PRINTF("[%2d] : ", index);
        big_int_print(&array[index].big);
        PRINTF(" scalar\n");
    }
}
#endif  // HAVE_PRINTF
