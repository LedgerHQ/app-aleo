#pragma once

#include <stdint.h>  // uint*_t


void bits_add_single(uint8_t *input, uint16_t input_bit_offset, bool bit);
void bits_add(uint8_t *input, uint16_t input_bit_offset, uint8_t *data, uint16_t data_bit_length);

void bits_print(uint8_t *input, uint16_t input_bit_length);