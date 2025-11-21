#pragma once

#include <stdint.h>  // uint*_t
#include <stddef.h>   // size_t

#include "field.h"
#include "scalar.h"

typedef struct {
    field_t x;
    field_t y;
} group_t;

void group_g_scalar_multiply(const scalar_t *b, group_t *r);

// PRINT
void group_print(const group_t *a);
void group_println(const group_t *a);
void group_print_array(const group_t *array, size_t length);
