#pragma once

#include <stdint.h>  // uint*_t

#include "fp256.h"
#include "field.h"

typedef fp256_t scalar_t;

void scalar_from_field_lossy(scalar_t *a, const field_t *f);

void scalar_to_big_int(const scalar_t *a, bigint_256_t *bigint);

void scalar_add_assign(scalar_t *a, const scalar_t *b);
void scalar_sub_assign(scalar_t *a, const scalar_t *b);
void scalar_mul_assign(scalar_t *a, const scalar_t *b);

void scalar_random(scalar_t *a);

// PRINT
void scalar_print(const scalar_t *a);
void scalar_println(const scalar_t *a);
void scalar_print_array(const scalar_t *array, size_t length);

