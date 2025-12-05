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

#include "bigint_256.h"
#include "fp256.h"

bool fp256_is_valid(const fp256_parameters_t *p, const fp256_t *a) {
    if (big_int_compare(&a->big, &p->MODULUS.big) < 0) {
        return true;
    }
    return false;
}

void fp256_reduce(const fp256_parameters_t *p, fp256_t *a) {
    if (fp256_is_valid(p, a) == false) {
        big_int_sub_noborrow(&a->big, &p->MODULUS.big);
    }
}

void fp256_add_assign(const fp256_parameters_t *p, fp256_t *a, const fp256_t *b) {
    big_int_add_nocarry(&a->big, &b->big);
    fp256_reduce(p, a);
}

void fp256_sub_assign(const fp256_parameters_t *p, fp256_t *a, const fp256_t *b) {
    if (big_int_compare(&a->big, &b->big) < 0) {
        big_int_add_nocarry(&a->big, &p->MODULUS.big);
    }
    big_int_sub_noborrow(&a->big, &b->big);
}

void fp256_mul_assign(const fp256_parameters_t *p, fp256_t *a, const fp256_t *b) {
    uint64_t r[4] = {0, 0, 0, 0};
    uint64_t carry1 = 0;
    uint64_t carry2 = 0;
    uint64_t k[2] = {0, 0};

    // Iteration 0
    r[0] = u64_mac_with_carry(r[0], a->big.u64[0], b->big.u64[0], &carry1);
    u64_mul(r[0], p->INV, k);
    (void) u64_mac_with_carry(r[0], k[1], p->MODULUS.big.u64[0], &carry2);

    r[1] = u64_mac_with_carry(r[1], a->big.u64[1], b->big.u64[0], &carry1);
    r[0] = u64_mac_with_carry(r[1], k[1], p->MODULUS.big.u64[1], &carry2);

    r[2] = u64_mac_with_carry(r[2], a->big.u64[2], b->big.u64[0], &carry1);
    r[1] = u64_mac_with_carry(r[2], k[1], p->MODULUS.big.u64[2], &carry2);

    r[3] = u64_mac_with_carry(r[3], a->big.u64[3], b->big.u64[0], &carry1);
    r[2] = u64_mac_with_carry(r[3], k[1], p->MODULUS.big.u64[3], &carry2);
    r[3] = carry1 + carry2;

    // Iteration 1
    carry1 = 0;
    carry2 = 0;
    r[0] = u64_mac_with_carry(r[0], a->big.u64[0], b->big.u64[1], &carry1);
    u64_mul(r[0], p->INV, k);
    (void) u64_mac_with_carry(r[0], k[1], p->MODULUS.big.u64[0], &carry2);

    r[1] = u64_mac_with_carry(r[1], a->big.u64[1], b->big.u64[1], &carry1);
    r[0] = u64_mac_with_carry(r[1], k[1], p->MODULUS.big.u64[1], &carry2);

    r[2] = u64_mac_with_carry(r[2], a->big.u64[2], b->big.u64[1], &carry1);
    r[1] = u64_mac_with_carry(r[2], k[1], p->MODULUS.big.u64[2], &carry2);

    r[3] = u64_mac_with_carry(r[3], a->big.u64[3], b->big.u64[1], &carry1);
    r[2] = u64_mac_with_carry(r[3], k[1], p->MODULUS.big.u64[3], &carry2);
    r[3] = carry1 + carry2;

    // Iteration 2
    carry1 = 0;
    carry2 = 0;
    r[0] = u64_mac_with_carry(r[0], a->big.u64[0], b->big.u64[2], &carry1);
    u64_mul(r[0], p->INV, k);
    (void) u64_mac_with_carry(r[0], k[1], p->MODULUS.big.u64[0], &carry2);

    r[1] = u64_mac_with_carry(r[1], a->big.u64[1], b->big.u64[2], &carry1);
    r[0] = u64_mac_with_carry(r[1], k[1], p->MODULUS.big.u64[1], &carry2);

    r[2] = u64_mac_with_carry(r[2], a->big.u64[2], b->big.u64[2], &carry1);
    r[1] = u64_mac_with_carry(r[2], k[1], p->MODULUS.big.u64[2], &carry2);

    r[3] = u64_mac_with_carry(r[3], a->big.u64[3], b->big.u64[2], &carry1);
    r[2] = u64_mac_with_carry(r[3], k[1], p->MODULUS.big.u64[3], &carry2);
    r[3] = carry1 + carry2;

    // Iteration 3
    carry1 = 0;
    carry2 = 0;
    r[0] = u64_mac_with_carry(r[0], a->big.u64[0], b->big.u64[3], &carry1);
    u64_mul(r[0], p->INV, k);
    (void) u64_mac_with_carry(r[0], k[1], p->MODULUS.big.u64[0], &carry2);

    r[1] = u64_mac_with_carry(r[1], a->big.u64[1], b->big.u64[3], &carry1);
    r[0] = u64_mac_with_carry(r[1], k[1], p->MODULUS.big.u64[1], &carry2);

    r[2] = u64_mac_with_carry(r[2], a->big.u64[2], b->big.u64[3], &carry1);
    r[1] = u64_mac_with_carry(r[2], k[1], p->MODULUS.big.u64[2], &carry2);

    r[3] = u64_mac_with_carry(r[3], a->big.u64[3], b->big.u64[3], &carry1);
    r[2] = u64_mac_with_carry(r[3], k[1], p->MODULUS.big.u64[3], &carry2);
    r[3] = carry1 + carry2;

    a->big.u64[0] = r[0];
    a->big.u64[1] = r[1];
    a->big.u64[2] = r[2];
    a->big.u64[3] = r[3];
    fp256_reduce(p, a);
}

void fp256_pow_assign(const fp256_parameters_t *p, fp256_t *a, uint8_t alpha) {
    fp256_t r;

    memcpy(&r, a, sizeof(fp256_t));
    for (uint8_t i = 0; i < (alpha - 1); i++) {
        fp256_mul_assign(p, a, &r);
    }
}

void fp256_sum_of_products(const fp256_parameters_t *p,
                           const fp256_t *a,
                           const fp256_t *b,
                           uint8_t length,
                           fp256_t *r) {
    uint64_t carry = 0;
    uint64_t u0 = 0, u1 = 0, u2 = 0, u3 = 0;

    for (uint8_t j = 0; j < 4; j++) {
        uint64_t t0 = u0, t1 = u1, t2 = u2, t3 = u3, t4 = 0;
        for (uint8_t i = 0; i < length; i++) {
            carry = 0;
            t0 = u64_mac_with_carry(t0, a[i].big.u64[j], b[i].big.u64[0], &carry);
            t1 = u64_mac_with_carry(t1, a[i].big.u64[j], b[i].big.u64[1], &carry);
            t2 = u64_mac_with_carry(t2, a[i].big.u64[j], b[i].big.u64[2], &carry);
            t3 = u64_mac_with_carry(t3, a[i].big.u64[j], b[i].big.u64[3], &carry);
            t4 += carry;
        }

        carry = 0;
        uint64_t k[2] = {0, 0};
        u64_mul(t0, p->INV, k);
        (void) u64_mac_with_carry(t0, k[1], p->MODULUS.big.u64[0], &carry);
        u0 = u64_mac_with_carry(t1, k[1], p->MODULUS.big.u64[1], &carry);
        u1 = u64_mac_with_carry(t2, k[1], p->MODULUS.big.u64[2], &carry);
        u2 = u64_mac_with_carry(t3, k[1], p->MODULUS.big.u64[3], &carry);
        u3 = t4 + carry;
    }
    r->big.u64[0] = u0;
    r->big.u64[1] = u1;
    r->big.u64[2] = u2;
    r->big.u64[3] = u3;
    fp256_reduce(p, r);
}

void fp256_from_big_int(const fp256_parameters_t *p, fp256_t *a, const bigint_256_t *bigint) {
    memcpy(&a->big, bigint, sizeof(bigint_256_t));
    if (big_int_is_zero(bigint) == false) {
        fp256_mul_assign(p, a, &p->R2);
    }
}

void fp256_from_int(const fp256_parameters_t *p, fp256_t *a, uint64_t i) {
    bigint_256_t big;

    big_int_from_int(&big, i);
    fp256_from_big_int(p, a, &big);
}

void fp256_to_big_int(const fp256_parameters_t *p, const fp256_t *a, bigint_256_t *bigint) {
    uint64_t carry = 0;
    uint64_t k[2] = {0, 0};

    memcpy(bigint, &a->big, sizeof(bigint_256_t));

    u64_mul(bigint->u64[0], p->INV, k);
    bigint->u64[0] = u64_mac_with_carry(bigint->u64[0], k[1], p->MODULUS.big.u64[0], &carry);
    bigint->u64[1] = u64_mac_with_carry(bigint->u64[1], k[1], p->MODULUS.big.u64[1], &carry);
    bigint->u64[2] = u64_mac_with_carry(bigint->u64[2], k[1], p->MODULUS.big.u64[2], &carry);
    bigint->u64[3] = u64_mac_with_carry(bigint->u64[3], k[1], p->MODULUS.big.u64[3], &carry);
    bigint->u64[0] = carry;

    u64_mul(bigint->u64[1], p->INV, k);
    bigint->u64[1] = u64_mac_with_carry(bigint->u64[1], k[1], p->MODULUS.big.u64[0], &carry);
    bigint->u64[2] = u64_mac_with_carry(bigint->u64[2], k[1], p->MODULUS.big.u64[1], &carry);
    bigint->u64[3] = u64_mac_with_carry(bigint->u64[3], k[1], p->MODULUS.big.u64[2], &carry);
    bigint->u64[0] = u64_mac_with_carry(bigint->u64[0], k[1], p->MODULUS.big.u64[3], &carry);
    bigint->u64[1] = carry;

    u64_mul(bigint->u64[2], p->INV, k);
    bigint->u64[2] = u64_mac_with_carry(bigint->u64[2], k[1], p->MODULUS.big.u64[0], &carry);
    bigint->u64[3] = u64_mac_with_carry(bigint->u64[3], k[1], p->MODULUS.big.u64[1], &carry);
    bigint->u64[0] = u64_mac_with_carry(bigint->u64[0], k[1], p->MODULUS.big.u64[2], &carry);
    bigint->u64[1] = u64_mac_with_carry(bigint->u64[1], k[1], p->MODULUS.big.u64[3], &carry);
    bigint->u64[2] = carry;

    u64_mul(bigint->u64[3], p->INV, k);
    bigint->u64[3] = u64_mac_with_carry(bigint->u64[3], k[1], p->MODULUS.big.u64[0], &carry);
    bigint->u64[0] = u64_mac_with_carry(bigint->u64[0], k[1], p->MODULUS.big.u64[1], &carry);
    bigint->u64[1] = u64_mac_with_carry(bigint->u64[1], k[1], p->MODULUS.big.u64[2], &carry);
    bigint->u64[2] = u64_mac_with_carry(bigint->u64[2], k[1], p->MODULUS.big.u64[3], &carry);
    bigint->u64[3] = carry;
}

void fp256_random(const fp256_parameters_t *p, fp256_t *a) {
    uint8_t bn[32];
    cx_bn_t cx_bn_modulus;
    cx_bn_t cx_bn_r;

    big_int_to_bn(&p->MODULUS.big, bn);

    (void) cx_bn_lock(32, 0);
    (void) cx_bn_alloc_init(&cx_bn_modulus, sizeof(bn), bn, sizeof(bn));
    (void) cx_bn_alloc(&cx_bn_r, sizeof(bn));
	(void) cx_bn_rng(cx_bn_r, cx_bn_modulus);
    (void) cx_bn_export(cx_bn_r, bn, sizeof(bn));
    bn_to_big_int(bn, &a->big);
    (void) cx_bn_unlock();
}

void fp256_print(fp256_t *array, uint16_t length) {
    for (uint16_t index = 0; index < length; index++) {
        PRINTF("[%2d] : ", index);
        big_int_print(&array[index].big);
    }
}

void fp256_bg_print(const fp256_parameters_t *p, fp256_t *array, uint16_t length) {
    for (uint16_t index = 0; index < length; index++) {
        PRINTF("[%2d] : ", index);
        fp256_t b;
        fp256_to_big_int(p, &array[index], &b.big);
        big_int_print(&b.big);
    }
}
