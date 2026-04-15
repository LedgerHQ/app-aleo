#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <cmocka.h>

#include "cx_errors.h"
#include "bits.h"

static void test_bits(void **state)
{
    (void) state;

    uint8_t bit_buffer_out[32] = {0};

    // bits_add_single
    bit_buffer_out[0] = 0;
    bit_buffer_out[1] = 0;
    bits_add_single(bit_buffer_out, 3, 1);
    bits_add_single(bit_buffer_out, 14, 1);
    assert_int_equal(bit_buffer_out[0], 0x08);
    assert_int_equal(bit_buffer_out[1], 0x40);

    // bits_add
    uint8_t bit_buffer_in[32] = {0x56, 0x4a, 0x65, 0x2d, 0x2e, 0xf4, 0x78, 0x74, 0x32, 0x4c, 0x65,
                                 0x5a, 0xd1, 0xe4, 0x54, 0xc6, 0x18, 0x53, 0x9c, 0xcf, 0xa6, 0x2c,
                                 0x3c, 0x5a, 0x0f, 0x2d, 0x29, 0x2a, 0x70, 0xb2, 0x83, 0x93};
    uint8_t buffer_exp[32]    = {0x08, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x94, 0xb5,
                                 0xb8, 0xd0, 0xe3, 0xd1, 0xc9, 0x30, 0x95, 0x69, 0x45, 0x93, 0x53,
                                 0x19, 0x63, 0x4c, 0x71, 0x3e, 0x1b, 0x00, 0x00, 0x00, 0x00};
    assert_int_equal(bits_add(bit_buffer_in, 0, 150, bit_buffer_out, 160, 32 * 8), -1);
    assert_int_equal(bits_add(bit_buffer_in, 15, 150, bit_buffer_out, 73, 32 * 8), 0);
    assert_memory_equal(bit_buffer_out, buffer_exp, 32);

    // bits_from_plaintext
    uint8_t plaintext_type[2] = {0x01, 0x03};
    assert_int_equal(bits_from_plaintext(bit_buffer_in, plaintext_type, 32 * 8, bit_buffer_out, 1),
                     -1);
    assert_int_equal(
        bits_from_plaintext(bit_buffer_in, plaintext_type, 32 * 8, bit_buffer_out, 256), -1);
    plaintext_type[0] = 0x00;
    assert_int_equal(
        bits_from_plaintext(bit_buffer_in, plaintext_type, 32 * 8, bit_buffer_out, 256), -1);
    assert_int_equal(bits_from_plaintext(bit_buffer_in, plaintext_type, 231, bit_buffer_out, 17),
                     -1);
    assert_int_equal(bits_from_plaintext(bit_buffer_in, plaintext_type, 231, bit_buffer_out, 25),
                     -1);
    uint8_t buffer_exp2[32] = {0x0c, 0x00, 0x02, 0x58, 0x29, 0x95, 0xb5, 0xb8, 0xd0, 0xe3, 0xd1,
                               0xc9, 0x30, 0x95, 0x69, 0x45, 0x93, 0x53, 0x19, 0x47, 0x93, 0x53,
                               0x19, 0x63, 0x4c, 0x71, 0x3e, 0x1b, 0x00, 0x00, 0x00, 0x00};
    assert_int_equal(
        bits_from_plaintext(bit_buffer_in, plaintext_type, 16 * 8, bit_buffer_out, 256), 0x9a);
    assert_memory_equal(bit_buffer_out, buffer_exp2, 32);
}

int main()
{
    const struct CMUnitTest tests[] = {cmocka_unit_test(test_bits)};

    return cmocka_run_group_tests(tests, NULL, NULL);
}