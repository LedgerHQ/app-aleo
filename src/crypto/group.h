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
    uint8_t x[BN_LENGTH];
    uint8_t y[BN_LENGTH];
} bn_group_t;

int group_add_assign(group_t *a, const group_t *b);
int group_scalar_multiply(const group_t *a, const scalar_t *b, group_t *r);
int group_g_scalar_multiply(const scalar_t *b, group_t *r);

#ifdef HAVE_PRINTF
void group_print(const group_t *a);
void group_println(const group_t *a);
void group_print_array(const group_t *array, size_t length);
#else  // !HAVE_PRINTF
#define group_print(...)
#define group_println(...)
#define group_print_array(...)
#endif  // !HAVE_PRINTF
