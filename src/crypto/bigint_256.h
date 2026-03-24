#pragma once

#include <stdint.h>  // uint*_t

typedef struct {
    uint64_t u64[4];
} bigint_256_t;

void     u64_print(uint64_t val);
uint64_t u64_add_carry(uint64_t *c, uint64_t a, uint64_t b);
uint64_t u64_sub_borrow(uint64_t *c, uint64_t a, uint64_t b);
void     u64_mul(uint64_t a, uint64_t b, uint64_t *r);
uint64_t u64_mac_with_carry(uint64_t a, uint64_t b, uint64_t c, uint64_t *carry);

void big_int_from_u64(bigint_256_t *a, const uint64_t i);
bool big_int_is_zero(const bigint_256_t *a);
bool big_int_is_even(const bigint_256_t *a);
bool big_int_add_nocarry(bigint_256_t *a, const bigint_256_t *b);
bool big_int_sub_noborrow(bigint_256_t *a, const bigint_256_t *b);
int  big_int_compare(const bigint_256_t *a, const bigint_256_t *b);

void big_int_bit_reduce(bigint_256_t *a, uint32_t nb_bits);

void big_int_to_le(const bigint_256_t *a);

void big_int_to_bn(const bigint_256_t *a, uint8_t *bn);
void bn_to_big_int(const uint8_t *bn, bigint_256_t *a);
void bn_reverse(uint8_t *bn);

int big_int_random(bigint_256_t *a, const bigint_256_t *modulus);

#ifdef HAVE_PRINTF
void big_int_print(const bigint_256_t *a);
void big_int_println(const bigint_256_t *a);
void bn_print(uint8_t *bn);
#else   // !HAVE_PRINTF
#define big_int_print(...)
#define big_int_println(...)
#define bn_print(...)
#endif  // !HAVE_PRINTF