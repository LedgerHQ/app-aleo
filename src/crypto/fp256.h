#pragma once

#include <stdint.h>  // uint*_t

#include "bigint_256.h"

typedef struct {
    bigint_256_t big;
} fp256_t;

typedef struct {
    uint32_t MODULUS_BITS;
    fp256_t  MODULUS;
    fp256_t  R;
    fp256_t  R2;
    uint64_t INV;
} fp256_parameters_t;

void fp256_add_assign(const fp256_parameters_t *p, fp256_t *a, const fp256_t *b);
void fp256_sub_assign(const fp256_parameters_t *p, fp256_t *a, const fp256_t *b);
void fp256_mul_assign(const fp256_parameters_t *p, fp256_t *a, const fp256_t *b);
void fp256_inverse_assign(const fp256_parameters_t *p, fp256_t *a);

void fp256_from_big_int(const fp256_parameters_t *p, fp256_t *a, const bigint_256_t *bigint);
void fp256_to_big_int(const fp256_parameters_t *p, const fp256_t *a, bigint_256_t *bigint);
void fp256_pow_assign(const fp256_parameters_t *p, fp256_t *a, uint8_t alpha);
void fp256_sum_of_products(const fp256_parameters_t *p,
                           const fp256_t            *a,
                           const fp256_t            *b,
                           uint8_t                   length,
                           fp256_t                  *r);

void fp256_bg_print(const fp256_parameters_t *p, fp256_t *array, uint16_t length);

void fp256_from_int(const fp256_parameters_t *p, fp256_t *a, uint64_t i);

void fp256_random(const fp256_parameters_t *p, fp256_t *a);

void fp256_print(fp256_t *array, uint16_t length);
