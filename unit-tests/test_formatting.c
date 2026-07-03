#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <cmocka.h>

#include "cx_errors.h"
#include "format_u128.h"
#include "aleo_swap_utils.h"

static void test_formatting(void **state)
{
    (void) state;

    char temp[40] = {0};

    u128_t amount;
    amount.high = 0;
    amount.low  = 100000000ull;
    assert_false(format_u128(NULL, sizeof(temp), amount));
    assert_false(format_u128(temp, 1, amount));

    amount.high = 0;
    amount.low  = 0;
    memset(temp, 0, sizeof(temp));
    assert_true(format_u128(temp, sizeof(temp), amount));
    assert_string_equal(temp, "0");

    amount.high = 100000000ull;
    amount.low  = 100000000ull;
    assert_false(format_u128(temp, 4, amount));

    amount.high = 0x000002C2A9DBC4B9;
    amount.low  = 0xD645D966C70B4847;
    memset(temp, 0, sizeof(temp));
    assert_true(format_fpu128(temp, sizeof(temp), amount, 0));
    assert_string_equal(temp, "55987651385496843543465465432135");
    assert_false(format_fpu128(temp, 15, amount, 0));

    amount.high = 0x000000000192D327;
    amount.low  = 0x2DFA07F293682E23;
    memset(temp, 0, sizeof(temp));
    assert_true(format_fpu128(temp, sizeof(temp), amount, 8));
    assert_string_equal(temp, "4869853215489545123.65465123");
    assert_false(format_fpu128(temp, 20, amount, 8));

    amount.high = 0;
    amount.low  = 123456789;
    memset(temp, 0, sizeof(temp));
    assert_false(format_u128(temp, 4, amount));
    assert_true(format_fpu128(temp, sizeof(temp), amount, 12));
    assert_string_equal(temp, "0.000123456789");
    assert_false(format_fpu128(temp, 9, amount, 12));
}

static void test_swap_str_to_u128(void **state)
{
    (void) state;

    u128_t result;

    // Invalid inputs
    assert_false(swap_str_to_u128(NULL, 1, &result));
    assert_false(swap_str_to_u128("\x01", 1, NULL));
    assert_false(swap_str_to_u128("\x01", 0, &result));
    // Overflow: 17 bytes exceed 128 bits
    assert_false(swap_str_to_u128("\x01\x02\x03\x04\x05\x06\x07\x08"
                                  "\x09\x0a\x0b\x0c\x0d\x0e\x0f\x10\x11",
                                  17,
                                  &result));

    // Single zero byte
    assert_true(swap_str_to_u128("\x00", 1, &result));
    assert_int_equal(result.high, 0);
    assert_int_equal(result.low, 0);

    // Single byte
    assert_true(swap_str_to_u128("\x2A", 1, &result));
    assert_int_equal(result.high, 0);
    assert_true(result.low == 0x2A);

    // 4 bytes (big-endian)
    assert_true(swap_str_to_u128("\x12\x34\x56\x78", 4, &result));
    assert_int_equal(result.high, 0);
    assert_true(result.low == 0x12345678);

    // 8 bytes (fills low exactly)
    assert_true(swap_str_to_u128("\x01\x23\x45\x67\x01\x23\x45\x67", 8, &result));
    assert_int_equal(result.high, 0);
    assert_true(result.low == 0x0123456701234567);

    // 9 bytes (spills into high)
    assert_true(swap_str_to_u128("\x01\x02\x03\x04\x05\x06\x07\x08\x09", 9, &result));
    assert_true(result.high == 0x01);
    assert_true(result.low == 0x0203040506070809);

    // 16 bytes (max capacity, all in safe range)
    assert_true(swap_str_to_u128("\x01\x02\x03\x04\x05\x06\x07\x08"
                                 "\x09\x0A\x0B\x0C\x0D\x0E\x0F\x10",
                                 16,
                                 &result));
    assert_true(result.high == 0x0102030405060708);
    assert_true(result.low == 0x090A0B0C0D0E0F10);

    // 16 bytes with zeros in high
    assert_true(swap_str_to_u128("\x00\x00\x00\x00\x00\x00\x00\x01"
                                 "\x00\x00\x00\x00\x00\x00\x00\x01",
                                 16,
                                 &result));
    assert_true(result.high == 0x0000000000000001);
    assert_true(result.low == 0x0000000000000001);
}

int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_formatting),
        cmocka_unit_test(test_swap_str_to_u128),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
