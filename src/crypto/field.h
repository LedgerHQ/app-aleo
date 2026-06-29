#pragma once

#include <stddef.h>  // size_t
#include <stdint.h>  // uint*_t

#include "fp256.h"

#define FIELD_MODULUS_BITS (253)

typedef fp256_t field_t;

extern const field_t FIELD_ZERO;
extern const field_t FIELD_ONE;

void field_assign(field_t *a, const field_t *b);
void field_add_assign(field_t *a, const field_t *b);
void field_mul_assign(field_t *a, const field_t *b);
void field_inverse_assign(field_t *a);
void field_pow_assign(field_t *a, uint8_t alpha);
void field_sum_of_products(const field_t *a, const field_t *b, uint8_t length, field_t *r);
void field_from_int(field_t *a, uint64_t i);

void field_to_big_int(const field_t *a, bigint_256_t *bigint);
void field_from_big_int(field_t *a, const bigint_256_t *bigint);

int field_random(field_t *a);

uint8_t field_from_bits(const uint8_t *input_bits,
                        const uint16_t input_bits_length,
                        field_t       *r,
                        uint8_t        max_field_count);

#ifdef HAVE_PRINTF
void field_print(const field_t *a);
void field_println(const field_t *a);
void field_print_array(const field_t *array, size_t length);
#else  // !HAVE_PRINTF
#define field_print(...)
#define field_println(...)
#define field_print_array(...)
#endif  // !HAVE_PRINTF
