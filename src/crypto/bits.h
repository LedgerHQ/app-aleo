#pragma once

#include <stdint.h>  // uint*_t

void bits_add_single(uint8_t *output, uint16_t output_bit_offset, bool bit);
int  bits_add(const uint8_t *input,
              uint16_t       input_bit_offset,
              uint16_t       input_length,
              uint8_t       *output,
              uint16_t       output_offset,
              uint16_t       output_max_bits);

int bits_from_plaintext_literal(const uint8_t *plaintext,
                                uint16_t       plaintext_bit_length,
                                uint8_t        variant,
                                uint8_t       *out,
                                uint16_t       output_max_bits);
