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

#include "group.h"

const group_t GROUP_G = {
    .x.big.u64 = {0x639e26f80ea7d4eb, 0x542dca7954d3e108, 0x3645718a1b4b2c83, 0x0127d355882d2e71},
    .y.big.u64 = {0xa405794451b5876e, 0x6f703124e2038870, 0xbe7fc364a828594c, 0x0a39ebf50185f537},
};

void group_g_scalar_multiply(const scalar_t *b, group_t *r) {
    cx_ecpoint_t point;
    uint8_t bn_x[32];
    uint8_t bn_y[32];
    uint8_t bn_scalar[32];
    bigint_256_t s;

    scalar_to_big_int(b, &s);

	big_int_println(&s);

    big_int_to_bn(&GROUP_G.x.big, bn_x);
    big_int_to_bn(&GROUP_G.y.big, bn_y);
    big_int_to_bn(&s, bn_scalar);

    (void) cx_bn_lock(32, 0);
    (void) cx_ecpoint_alloc(&point, CX_CURVE_EdBLS12);
    (void) cx_ecpoint_init(&point, bn_x, 32, bn_y, 32);

    (void) cx_ecpoint_scalarmul(&point, bn_scalar, 32);
    (void) cx_ecpoint_export(&point, bn_x, 32, bn_y, 32);

    bn_to_big_int(bn_x, &s);
    field_from_big_int(&r->x, &s);
    bn_to_big_int(bn_y, &s);
    field_from_big_int(&r->y, &s);

    (void) cx_ecpoint_destroy(&point);
    (void) cx_bn_unlock();
}

void group_print(const group_t *a) {
    PRINTF("(x:");
    field_print(&a->x);
    PRINTF(", y:");
    field_print(&a->y);
}

void group_println(const group_t *a) {
    group_print(a);
    PRINTF("\n");
}

void group_print_array(const group_t *array, size_t length) {
    for (size_t index = 0; index < length; index++) {
        PRINTF("[%2d] : ", index);
        group_println(&array[index]);
    }
}