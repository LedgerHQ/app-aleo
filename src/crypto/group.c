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
    .x.big.u64 = {0x3fe2a0af33ac1b52, 0x028939946bfa31f3, 0x1d65dfa053616662, 0x120f836f8253d8c2},
    .y.big.u64 = {0xe824937af1cbef27, 0x8b98045b63a9531e, 0xc1ca47ef204e5d8f, 0x0e990e7d019b9712},
};

int group_add_assign(group_t *a, const group_t *b)
{
    cx_err_t     error;
    cx_ecpoint_t point_a;
    cx_ecpoint_t point_b;
    cx_ecpoint_t point_r;
    uint8_t      bn_x[BN_LENGTH];
    uint8_t      bn_y[BN_LENGTH];
    bigint_256_t s;

    LEDGER_ASSERT(a != NULL, "NULL a");
    LEDGER_ASSERT(b != NULL, "NULL b");

    if (cx_bn_lock(BN_LENGTH, 0) != CX_OK) {
        return -1;
    }

    field_to_big_int(&a->x, &s);
    big_int_to_bn(&s, bn_x);
    field_to_big_int(&a->y, &s);
    big_int_to_bn(&s, bn_y);
    if ((error = cx_ecpoint_alloc(&point_a, CX_CURVE_EdBLS12)) != CX_OK) {
        goto end;
    }
    if ((error = cx_ecpoint_init(&point_a, bn_x, sizeof(bn_x), bn_y, sizeof(bn_y))) != CX_OK) {
        goto end;
    }

    field_to_big_int(&b->x, &s);
    big_int_to_bn(&s, bn_x);
    field_to_big_int(&b->y, &s);
    big_int_to_bn(&s, bn_y);
    if ((error = cx_ecpoint_alloc(&point_b, CX_CURVE_EdBLS12)) != CX_OK) {
        goto end;
    }
    if ((error = cx_ecpoint_init(&point_b, bn_x, sizeof(bn_x), bn_y, sizeof(bn_y))) != CX_OK) {
        goto end;
    }

    if ((error = cx_ecpoint_alloc(&point_r, CX_CURVE_EdBLS12)) != CX_OK) {
        goto end;
    }
    if ((error = cx_ecpoint_add(&point_r, &point_a, &point_b)) != CX_OK) {
        goto end;
    }

    if ((error = cx_ecpoint_export(&point_r, bn_x, sizeof(bn_x), bn_y, sizeof(bn_y))) != CX_OK) {
        goto end;
    }
    bn_to_big_int(bn_x, &s);
    field_from_big_int(&a->x, &s);
    bn_to_big_int(bn_y, &s);
    field_from_big_int(&a->y, &s);

    if ((error = cx_ecpoint_destroy(&point_a)) != CX_OK) {
        goto end;
    }
    if ((error = cx_ecpoint_destroy(&point_b)) != CX_OK) {
        goto end;
    }
    if ((error = cx_ecpoint_destroy(&point_r)) != CX_OK) {
        goto end;
    }

end:
    cx_bn_unlock();
    if (error != CX_OK) {
        return -1;
    }
    return 0;
}

int group_scalar_multiply(const group_t *a, const scalar_t *b, group_t *r)
{
    cx_err_t     error;
    cx_ecpoint_t point;
    uint8_t      bn_x[BN_LENGTH];
    uint8_t      bn_y[BN_LENGTH];
    uint8_t      bn_scalar[BN_LENGTH];
    bigint_256_t s;

    LEDGER_ASSERT(a != NULL, "NULL a");
    LEDGER_ASSERT(b != NULL, "NULL b");
    LEDGER_ASSERT(r != NULL, "NULL r");

    field_to_big_int(&a->x, &s);
    big_int_to_bn(&s, bn_x);
    field_to_big_int(&a->y, &s);
    big_int_to_bn(&s, bn_y);

    scalar_to_big_int(b, &s);
    big_int_to_bn(&s, bn_scalar);

    if (cx_bn_lock(BN_LENGTH, 0) != CX_OK) {
        return -1;
    }

    if ((error = cx_ecpoint_alloc(&point, CX_CURVE_EdBLS12)) != CX_OK) {
        goto end;
    }
    if ((error = cx_ecpoint_init(&point, bn_x, sizeof(bn_x), bn_y, sizeof(bn_y))) != CX_OK) {
        goto end;
    }

    if ((error = cx_ecpoint_scalarmul(&point, bn_scalar, sizeof(bn_scalar))) != CX_OK) {
        goto end;
    }
    if ((error = cx_ecpoint_export(&point, bn_x, sizeof(bn_x), bn_y, sizeof(bn_y))) != CX_OK) {
        goto end;
    }

    bn_to_big_int(bn_x, &s);
    field_from_big_int(&r->x, &s);
    bn_to_big_int(bn_y, &s);
    field_from_big_int(&r->y, &s);

    if ((error = cx_ecpoint_destroy(&point)) != CX_OK) {
        goto end;
    }

end:
    if (cx_bn_unlock() != CX_OK) {
        return -1;
    }
    if (error != CX_OK) {
        return -1;
    }
    return 0;
}

int group_g_scalar_multiply(const scalar_t *b, group_t *r)
{
    return group_scalar_multiply(&GROUP_G, b, r);
}

#ifdef HAVE_PRINTF
void group_print(const group_t *a)
{
    PRINTF("(x:");
    field_print(&a->x);
    PRINTF(", y:");
    field_print(&a->y);
}

void group_println(const group_t *a)
{
    group_print(a);
    PRINTF("\n");
}

void group_print_array(const group_t *array, size_t length)
{
    for (size_t index = 0; index < length; index++) {
        PRINTF("[%2d] : ", index);
        group_println(&array[index]);
    }
}
#endif  // HAVE_PRINTF