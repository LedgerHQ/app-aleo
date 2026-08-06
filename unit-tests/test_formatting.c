#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <cmocka.h>

#include "cx_errors.h"
#include "format_u128.h"

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

int main()
{
    const struct CMUnitTest tests[] = {cmocka_unit_test(test_formatting)};

    return cmocka_run_group_tests(tests, NULL, NULL);
}
