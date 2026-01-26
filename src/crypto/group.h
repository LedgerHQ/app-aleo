#pragma once

#include <stdint.h>  // uint*_t
#include <stddef.h>  // size_t

#include "field.h"
#include "scalar.h"

typedef struct {
    field_t x;
    field_t y;
} group_t;

typedef struct {
    uint8_t  compressed[32];
    uint32_t sign;
} compressed_group_t;

typedef struct {
    uint8_t x[32];
    uint8_t y[32];
} bn_group_t;

void group_add_assign(group_t *a, const group_t *b);
void group_scalar_multiply(const group_t *a, const scalar_t *b, group_t *r);
void group_g_scalar_multiply(const scalar_t *b, group_t *r);

// PRINT
void group_print(const group_t *a);
void group_println(const group_t *a);
void group_print_array(const group_t *array, size_t length);
