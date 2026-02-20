#pragma once

#include <stdint.h>  // uint*_t

#include "field.h"
#include "scalar.h"
#include "group.h"

void hash_to_scalar_psd2(field_t *input, size_t input_length, scalar_t *r);
void hash_to_scalar_psd4(field_t *input, size_t input_length, scalar_t *r);
void hash_to_scalar_psd8(field_t *input, size_t input_length, scalar_t *r);

void hash_psd2(field_t *input, size_t input_length, field_t *r);
void hash_psd4(field_t *input, size_t input_length, field_t *r);
void hash_psd8(field_t *input, size_t input_length, field_t *r);

void hash_many_psd8(field_t *input, size_t input_length, field_t *output, size_t output_length);
