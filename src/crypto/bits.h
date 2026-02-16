#pragma once

#include <stdint.h>  // uint*_t

void bits_add_single(uint8_t *output, uint16_t output_bit_offset, bool bit);
void bits_add(const uint8_t *input,
              uint16_t       input_bit_offset,
              uint16_t       input_length,
              uint8_t       *output,
              uint16_t       output_offset);

uint16_t bits_from_plaintext(const uint8_t *plaintext,
                             const uint8_t *plaintext_type,
                             uint16_t       plaintext_bit_length,
                             uint8_t       *out);

void bits_print(const uint8_t *input, uint16_t input_bit_length);
void bits_println(const uint8_t *input, uint16_t input_bit_length);
