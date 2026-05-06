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
#include "write.h"
#include "read.h"

void u64_print(uint64_t val)
{
    UNUSED(val);
    PRINTF("%08x", (uint32_t) (val >> 32));
    PRINTF("%08x ", (uint32_t) (val >> 0));
}

static uint64_t u64_add_carry(uint64_t *c, uint64_t a, uint64_t b)
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

static uint64_t u64_sub_borrow(uint64_t *c, uint64_t a, uint64_t b)
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

    LEDGER_ASSERT(r != NULL, "NULL r");

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

    LEDGER_ASSERT(carry != NULL, "NULL carry");

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

void big_int_from_u64(bigint_256_t *a, const uint64_t i)
{
    LEDGER_ASSERT(a != NULL, "NULL a");

    a->u64[0] = i;
    a->u64[1] = 0;
    a->u64[2] = 0;
    a->u64[3] = 0;
}

bool big_int_is_zero(const bigint_256_t *a)
{
    LEDGER_ASSERT(a != NULL, "NULL a");

    if (!a->u64[0] && !a->u64[1] && !a->u64[2] && !a->u64[3]) {
        return true;
    }
    return false;
}

bool big_int_is_even(const bigint_256_t *a)
{
    LEDGER_ASSERT(a != NULL, "NULL a");

    if (a->u64[0] & 1) {
        return false;
    }

    return true;
}

bool big_int_add_nocarry(bigint_256_t *a, const bigint_256_t *b)
{
    uint64_t carry = 0;

    LEDGER_ASSERT(a != NULL, "NULL a");
    LEDGER_ASSERT(b != NULL, "NULL b");

    for (uint8_t i = 0; i < 4; i++) {
        a->u64[i] = u64_add_carry(&carry, a->u64[i], b->u64[i]);
    }

    return (carry != 0);
}

bool big_int_sub_noborrow(bigint_256_t *a, const bigint_256_t *b)
{
    uint64_t borrow = 0;

    LEDGER_ASSERT(a != NULL, "NULL a");
    LEDGER_ASSERT(b != NULL, "NULL b");

    for (uint8_t i = 0; i < 4; i++) {
        a->u64[i] = u64_sub_borrow(&borrow, a->u64[i], b->u64[i]);
    }

    return (borrow != 0);
}

int big_int_compare(const bigint_256_t *a, const bigint_256_t *b)
{
    LEDGER_ASSERT(a != NULL, "NULL a");
    LEDGER_ASSERT(b != NULL, "NULL b");

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

void big_int_bit_reduce(bigint_256_t *a, uint32_t nb_bits)
{
    LEDGER_ASSERT(a != NULL, "NULL a");

    for (size_t i = nb_bits; i < 256; i++) {
        uint64_t mask = ((uint64_t) 1) << (i % 64);
        a->u64[i / 64] &= ~mask;
    }
}

void big_int_to_bn(const bigint_256_t *a, uint8_t *bn)
{
    LEDGER_ASSERT(a != NULL, "NULL a");
    LEDGER_ASSERT(bn != NULL, "NULL bn");

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
    LEDGER_ASSERT(a != NULL, "NULL a");
    LEDGER_ASSERT(bn != NULL, "NULL bn");

    memset(a, 0, sizeof(bigint_256_t));
    for (size_t i = 0; i < 32; i++) {
        uint64_t val = bn[31 - i];
        val <<= 8 * (i % 8);
        a->u64[i / 8] += val;
    }
}

void bn_reverse(uint8_t *bn)
{
    LEDGER_ASSERT(bn != NULL, "NULL bn");

    for (size_t i = 0; i < (32 + 1) / 2; i++) {
        uint8_t inter  = bn[i];
        bn[i]          = bn[32 - 1 - i];
        bn[32 - 1 - i] = inter;
    }
}

int big_int_random(bigint_256_t *a, const bigint_256_t *modulus)
{
    cx_err_t error;
    cx_bn_t  cx_bn_modulus;
    cx_bn_t  cx_bn_r;
    uint8_t  bn[32];

    LEDGER_ASSERT(a != NULL, "NULL a");
    LEDGER_ASSERT(modulus != NULL, "NULL modulus");

    big_int_to_bn(modulus, bn);

    if (cx_bn_lock(32, 0) != CX_OK) {
        return -1;
    }
    if ((error = cx_bn_alloc_init(&cx_bn_modulus, sizeof(bn), bn, sizeof(bn))) != CX_OK) {
        goto end;
    }
    if ((error = cx_bn_alloc(&cx_bn_r, sizeof(bn))) != CX_OK) {
        goto end;
    }
    if ((error = cx_bn_rng(cx_bn_r, cx_bn_modulus)) != CX_OK) {
        goto end;
    }
    if ((error = cx_bn_export(cx_bn_r, bn, sizeof(bn))) != CX_OK) {
        goto end;
    }
    bn_to_big_int(bn, a);

end:
    if (cx_bn_unlock() != CX_OK) {
        return -1;
    }
    if (error != CX_OK) {
        return -1;
    }
    return 0;
}

#ifdef HAVE_PRINTF
void big_int_print(const bigint_256_t *a)
{
    LEDGER_ASSERT(a != NULL, "NULL a");

    for (uint8_t i = 0; i < 4; i++) {
        u64_print(a->u64[i]);
    }
}

void big_int_println(const bigint_256_t *a)
{
    LEDGER_ASSERT(a != NULL, "NULL a");

    big_int_print(a);
    PRINTF("\n");
}

void bn_print(uint8_t *bn)
{
    LEDGER_ASSERT(bn != NULL, "NULL bn");

    for (size_t i = 0; i < 32; i++) {
        PRINTF("%02x", bn[i]);
    }
    PRINTF("\n");
}
#endif  // HAVE_PRINTF