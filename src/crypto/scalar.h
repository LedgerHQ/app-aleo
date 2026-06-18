#pragma once

#include <stdint.h>  // uint*_t

#include "fp256.h"
#include "field.h"

#define SCALAR_MODULUS_BITS (251)

typedef fp256_t scalar_t;

extern const field_t SCALAR_ZERO;

void scalar_from_field_lossy(scalar_t *a, const field_t *f);
void scalar_to_field(const scalar_t *a, field_t *f);

void scalar_to_big_int(const scalar_t *a, bigint_256_t *bigint);
void scalar_from_big_int(scalar_t *a, const bigint_256_t *bigint);

void scalar_add_assign(scalar_t *a, const scalar_t *b);
void scalar_sub_assign(scalar_t *a, const scalar_t *b);
void scalar_mul_assign(scalar_t *a, const scalar_t *b);

int scalar_random(scalar_t *a);

#ifdef HAVE_PRINTF
void scalar_print(const scalar_t *a);
void scalar_println(const scalar_t *a);
void scalar_print_array(const scalar_t *array, size_t length);
#else  // !HAVE_PRINTF
#define scalar_print(...)
#define scalar_println(...)
#define scalar_print_array(...)
#endif  // !HAVE_PRINTF
