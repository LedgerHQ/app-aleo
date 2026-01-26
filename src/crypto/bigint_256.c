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

#include "bigint_256.h"
#include "write.h"
#include "read.h"

void u64_print(uint64_t val)
{
    UNUSED(val);
    PRINTF("%08x", (uint32_t) (val >> 32));
    PRINTF("%08x ", (uint32_t) (val >> 0));
}

uint64_t u64_add_carry(uint64_t *c, uint64_t a, uint64_t b)
{
    uint32_t bn_a[2];
    uint32_t bn_b[2];
    uint64_t r = 0;

    bn_a[1] = (uint32_t) (a >> 32);
    bn_a[0] = (uint32_t) (a >> 0);
    bn_b[1] = (uint32_t) (b >> 32);
    bn_b[0] = (uint32_t) (b >> 0);

    r       = (uint64_t) bn_a[0] + (uint64_t) bn_b[0] + *c;
    bn_a[0] = (uint32_t) r;
    r >>= 32;
    r       = (uint64_t) bn_a[1] + (uint64_t) bn_b[1] + r;
    bn_a[1] = (uint32_t) r;
    r >>= 32;
    *c = r;

    r = bn_a[1];
    r <<= 32;
    r += bn_a[0];

    return r;
}

uint64_t u64_sub_borrow(uint64_t *c, uint64_t a, uint64_t b)
{
    uint64_t val = 0;

    if (*c) {
        if (a > b) {
            val = (a - b) - *c;
            *c  = 0;
        }
        else {
            val = (b - a) + *c;
            val = (0xffffffffffffffff - val) + 1;
        }
    }
    else {
        if (a >= b) {
            val = a - b;
        }
        else {
            val = b - a;
            val = (0xffffffffffffffff - val) + 1;
            *c  = 1;
        }
    }

    return val;
}

void u64_mul(uint64_t a, uint64_t b, uint64_t *r)
{
    uint64_t carry = 0;
    uint32_t bn_a[2];
    uint32_t bn_b[2];
    uint64_t val = 0;

    r[0]    = 0;
    r[1]    = 0;
    bn_a[0] = (uint32_t) (a >> 32);
    bn_a[1] = (uint32_t) (a >> 0);
    bn_b[0] = (uint32_t) (b >> 32);
    bn_b[1] = (uint32_t) (b >> 0);

    val = (uint64_t) bn_a[0] * (uint64_t) bn_b[0];
    r[0] += val;

    val = (uint64_t) bn_a[1] * (uint64_t) bn_b[1];
    r[1] += val;

    val   = (uint64_t) bn_a[0] * (uint64_t) bn_b[1];
    carry = 0;
    r[1]  = u64_add_carry(&carry, r[1], (uint64_t) (val << 32));
    r[0] += (uint32_t) (val >> 32) + carry;

    val   = (uint64_t) bn_a[1] * (uint64_t) bn_b[0];
    carry = 0;
    r[1]  = u64_add_carry(&carry, r[1], (uint64_t) (val << 32));
    r[0] += (uint32_t) (val >> 32) + carry;
}

uint64_t u64_mac_with_carry(uint64_t a, uint64_t b, uint64_t c, uint64_t *carry)
{
    // a + (b*c) + carry
    uint64_t cr = 0;
    uint64_t p[2];

    u64_mul(b, c, p);

    cr   = 0;
    p[1] = u64_add_carry(&cr, p[1], a);
    p[0] += cr;

    cr   = 0;
    p[1] = u64_add_carry(&cr, p[1], *carry);
    p[0] += cr;

    *carry = p[0];

    return p[1];
}

void big_int_print(const bigint_256_t *a)
{
    for (uint8_t i = 0; i < 4; i++) {
        u64_print(a->u64[i]);
    }
}

void big_int_println(const bigint_256_t *a)
{
    big_int_print(a);
    PRINTF("\n");
}

void big_int_from_int(bigint_256_t *a, const uint64_t i)
{
    a->u64[0] = i;
    a->u64[1] = 0;
    a->u64[2] = 0;
    a->u64[3] = 0;
}

bool big_int_is_zero(const bigint_256_t *a)
{
    if (!a->u64[0] && !a->u64[1] && !a->u64[2] && !a->u64[3]) {
        return true;
    }
    return false;
}

bool big_int_add_nocarry(bigint_256_t *a, const bigint_256_t *b)
{
    uint64_t carry = 0;

    for (uint8_t i = 0; i < 4; i++) {
        a->u64[i] = u64_add_carry(&carry, a->u64[i], b->u64[i]);
    }

    return (carry != 0);
}

bool big_int_sub_noborrow(bigint_256_t *a, const bigint_256_t *b)
{
    uint64_t borrow = 0;

    for (uint8_t i = 0; i < 4; i++) {
        a->u64[i] = u64_sub_borrow(&borrow, a->u64[i], b->u64[i]);
    }

    return (borrow != 0);
}

int big_int_compare(const bigint_256_t *a, const bigint_256_t *b)
{
    for (uint8_t i = 0; i < 4; i++) {
        if (a->u64[3 - i] > b->u64[3 - i]) {
            return 1;
        }
        else if (a->u64[3 - i] < b->u64[3 - i]) {
            return -1;
        }
    }
    return 0;
}

void big_int_to_le(const bigint_256_t *a)
{
    PRINTF("To LE : ");
    for (size_t i = 0; i < 256; i++) {
        uint64_t val  = a->u64[i / 64];
        uint64_t mask = ((uint64_t) 1) << (i % 64);
        UNUSED(val);
        if (val & mask) {
            PRINTF("true, ");
        }
        else {
            PRINTF("false, ");
        }
    }
    PRINTF("\n");
}

void big_int_bit_reduce(bigint_256_t *a, uint32_t nb_bits)
{
    for (size_t i = nb_bits; i < 256; i++) {
        uint64_t mask = ((uint64_t) 1) << (i % 64);
        a->u64[i / 64] &= ~mask;
    }
}

void bn_print(uint8_t *bn)
{
    UNUSED(bn);
    for (size_t i = 0; i < 32; i++) {
        PRINTF("%02x", bn[i]);
    }
    PRINTF("\n");
}

void big_int_to_bn(const bigint_256_t *a, uint8_t *bn)
{
    memset(bn, 0, 32);
    for (size_t i = 0; i < 4; i++) {
        uint64_t val = a->u64[i];
        for (size_t j = 0; j < 8; j++) {
            bn[8 * (3 - i) + 7 - j] = (uint8_t) (val);
            val >>= 8;
        }
    }
}

void bn_to_big_int(const uint8_t *bn, bigint_256_t *a)
{
    memset(a, 0, sizeof(bigint_256_t));
    for (size_t i = 0; i < 32; i++) {
        uint64_t val = bn[31 - i];
        val <<= 8 * (i % 8);
        a->u64[i / 8] += val;
    }
}

void big_int_random(bigint_256_t *a, const bigint_256_t *modulus)
{
    uint8_t bn[32];
    cx_bn_t cx_bn_modulus;
    cx_bn_t cx_bn_r;

    big_int_to_bn(modulus, bn);

    (void) cx_bn_lock(32, 0);
    (void) cx_bn_alloc_init(&cx_bn_modulus, sizeof(bn), bn, sizeof(bn));
    (void) cx_bn_alloc(&cx_bn_r, sizeof(bn));
    (void) cx_bn_rng(cx_bn_r, cx_bn_modulus);
    (void) cx_bn_export(cx_bn_r, bn, sizeof(bn));
    bn_to_big_int(bn, a);
    (void) cx_bn_unlock();
}
