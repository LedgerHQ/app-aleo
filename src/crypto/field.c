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

#include "field.h"

// BLS12-377
static const fp256_parameters_t field_parameters = {
    .MODULUS_BITS = 253,
    // 8444461749428370424248824938781546531375899335154063827935233455917409239041
    .MODULUS = {.big.u64 = 725501752471715841U,
                6461107452199829505U,
                6968279316240510977U,
                1345280370688173398U},
    .R = {.big.u64 = 9015221291577245683U,
          8239323489949974514U,
          1646089257421115374U,
          958099254763297437U},
    .R2 = {.big.u64 = 2726216793283724667U,
           14712177743343147295U,
           12091039717619697043U,
           81024008013859129U},
    .INV = 725501752471715839U};

void field_assign(field_t *a, const field_t *b) {
    memcpy(a, b, sizeof(field_t));
}

void field_add_assign(field_t *a, const field_t *b) {
    fp256_add_assign(&field_parameters, a, b);
}

void field_mul_assign(field_t *a, const field_t *b) {
    fp256_mul_assign(&field_parameters, a, b);
}

void field_pow_assign(field_t *a, uint8_t alpha) {
    fp256_pow_assign(&field_parameters, a, alpha);
}

void field_sum_of_products(const field_t *a, const field_t *b, uint8_t length, field_t *r) {
    fp256_sum_of_products(&field_parameters, a, b, length, r);
}

void field_from_int(field_t *a, uint64_t i) {
    fp256_from_int(&field_parameters, a, i);
}

void field_to_big_int(const field_t *a, bigint_256_t *bigint) {
    fp256_to_big_int(&field_parameters, a, bigint);
}

void field_from_big_int(field_t *a, const bigint_256_t *bigint) {
    fp256_from_big_int(&field_parameters, a, bigint);
}

void field_print(const field_t *a) {
    big_int_print(&a->big);
}

void field_println(const field_t *a) {
    big_int_println(&a->big);
}

void field_print_array(const field_t *array, size_t length) {
    for (uint16_t index = 0; index < length; index++) {
        PRINTF("[%2d] : ", index);
        big_int_print(&array[index].big);
        PRINTF(" field\n");
    }
}

