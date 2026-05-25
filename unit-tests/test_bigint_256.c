#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <cmocka.h>

#include "cx_errors.h"
#include "bigint_256.h"

static void test_u64(void **state)
{
    uint64_t a, b, c, ca, r;

    // u64_print
    a = 0x12345678abcdef12;
    u64_print(a);

    // u64_mul
    uint64_t res[2] = {0};
    a               = 0x815b6a4c561231fe;
    b               = 0x9478f42e2d654a56;
    u64_mul(a, b, res);
    assert_int_equal(res[0], 0x4b05f7b06c38f97f);
    assert_int_equal(res[1], 0x52aa02218bc63754);

    // u64_mac_with_carry
    a  = 0x815b6a4c561231fe;
    b  = 0x7478f42e2d654a56;
    c  = 0xc654e4d15a654c32;
    ca = 0x19bf1456544eee45;
    r  = u64_mac_with_carry(a, b, c, &ca);
    assert_int_equal(r, 0x383b2702a292d0f);
    assert_int_equal(ca, 0x5a3c2ca6cf9c5318);
}

static void test_bigint_256(void **state)
{
    (void) state;

    bigint_256_t a, b;
    uint8_t      bn[BN_LENGTH];

    // big_int_from_u64
    big_int_from_u64(&b, 0x12345678);
    assert_int_equal(b.u64[0], 0x12345678);
    assert_int_equal(b.u64[1], 0);
    assert_int_equal(b.u64[2], 0);
    assert_int_equal(b.u64[3], 0);

    // big_int_is_zero
    memset(&b, 0, sizeof(bigint_256_t));
    assert_true(big_int_is_zero(&b));
    b.u64[2] = 0x4654654;
    assert_false(big_int_is_zero(&b));

    // big_int_is_even
    assert_true(big_int_is_even(&b));
    b.u64[0] = 0x21eaf51;
    assert_false(big_int_is_even(&b));

    // big_int_add_nocarry
    a.u64[0] = 0x7478f42e2d654a56;
    a.u64[1] = 0xc654e4d15a654c32;
    a.u64[2] = 0x5a3c2ca6cf9c5318;
    a.u64[3] = 0x9383b2702a292d0f;

    b.u64[0] = 0x19bf1456544eee45;
    b.u64[1] = 0x15d45e7a83777c54;
    b.u64[2] = 0x131d89e1d7531857;
    b.u64[3] = 0xc654e4d15a654c32;

    assert_true(big_int_add_nocarry(&a, &b));
    assert_int_equal(a.u64[0], 0x8e38088481b4389b);
    assert_int_equal(a.u64[1], 0xdc29434bdddcc886);
    assert_int_equal(a.u64[2], 0x6d59b688a6ef6b6f);
    assert_int_equal(a.u64[3], 0x59d89741848e7941);

    // big_int_sub_noborrow
    a.u64[0] = 0x7478f42e2d654a56;
    a.u64[1] = 0xc654e4d15a654c32;
    a.u64[2] = 0x5a3c2ca6cf9c5318;
    a.u64[3] = 0x9383b2702a292d0f;

    b.u64[0] = 0x19bf1456544eee45;
    b.u64[1] = 0x15d45e7a83777c54;
    b.u64[2] = 0x131d89e1d7531857;
    b.u64[3] = 0xc654e4d15a654c32;

    assert_false(big_int_sub_noborrow(&b, &a));
    assert_int_equal(b.u64[0], 0xa546202826e9a3ef);
    assert_int_equal(b.u64[1], 0x4f7f79a929123021);
    assert_int_equal(b.u64[2], 0xb8e15d3b07b6c53e);
    assert_int_equal(b.u64[3], 0x32d13261303c1f22);

    // big_int_compare
    a.u64[0] = 0x7478f42e2d654a56;
    a.u64[1] = 0xc654e4d15a654c32;
    a.u64[2] = 0x5a3c2ca6cf9c5318;
    a.u64[3] = 0x9383b2702a292d0f;

    b.u64[0] = 0x19bf1456544eee45;
    b.u64[1] = 0x15d45e7a83777c54;
    b.u64[2] = 0x131d89e1d7531857;
    b.u64[3] = 0xc654e4d15a654c32;

    assert_int_equal(big_int_compare(&a, &a), 0);
    assert_int_equal(big_int_compare(&a, &b), -1);
    assert_int_equal(big_int_compare(&b, &a), 1);

    // big_int_bit_reduce
    a.u64[0] = 0x7478f42e2d654a56;
    a.u64[1] = 0xc654e4d15a654c32;
    a.u64[2] = 0x5a3c2ca6cf9c5318;
    a.u64[3] = 0x9383b2702a292d0f;

    big_int_bit_reduce(&a, 251);
    assert_int_equal(a.u64[0], 0x7478f42e2d654a56);
    assert_int_equal(a.u64[1], 0xc654e4d15a654c32);
    assert_int_equal(a.u64[2], 0x5a3c2ca6cf9c5318);
    assert_int_equal(a.u64[3], 0x383b2702a292d0f);

    // big_int_to_bn
    a.u64[0]           = 0x7478f42e2d654a56;
    a.u64[1]           = 0xc654e4d15a654c32;
    a.u64[2]           = 0x5a3c2ca6cf9c5318;
    a.u64[3]           = 0x9383b2702a292d0f;
    uint8_t bn_exp[32] = {0x93, 0x83, 0xb2, 0x70, 0x2a, 0x29, 0x2d, 0x0f, 0x5a, 0x3c, 0x2c,
                          0xa6, 0xcf, 0x9c, 0x53, 0x18, 0xc6, 0x54, 0xe4, 0xd1, 0x5a, 0x65,
                          0x4c, 0x32, 0x74, 0x78, 0xf4, 0x2e, 0x2d, 0x65, 0x4a, 0x56};

    big_int_to_bn(&a, bn);
    assert_memory_equal(bn, bn_exp, 32);

    // bn_to_big_int
    memset(&b, 0, sizeof(bigint_256_t));
    bn_to_big_int(bn, &b);
    assert_int_equal(a.u64[0], 0x7478f42e2d654a56);
    assert_int_equal(a.u64[1], 0xc654e4d15a654c32);
    assert_int_equal(a.u64[2], 0x5a3c2ca6cf9c5318);
    assert_int_equal(a.u64[3], 0x9383b2702a292d0f);

    // bn_reverse
    uint8_t bn_exp_2[32] = {0x56, 0x4a, 0x65, 0x2d, 0x2e, 0xf4, 0x78, 0x74, 0x32, 0x4c, 0x65,
                            0x5a, 0xd1, 0xe4, 0x54, 0xc6, 0x18, 0x53, 0x9c, 0xcf, 0xa6, 0x2c,
                            0x3c, 0x5a, 0x0f, 0x2d, 0x29, 0x2a, 0x70, 0xb2, 0x83, 0x93};
    bn_reverse(bn);
    assert_memory_equal(bn, bn_exp_2, 32);

    // big_int_random
    bigint_256_t modulus = {
        .u64
        = {725501752471715841U, 6461107452199829505U, 6968279316240510977U, 1345280370688173398U}
    };

    will_return(cx_bn_lock, -1);
    assert_int_equal(big_int_random(&a, &modulus), -1);

    will_return(cx_bn_lock, CX_OK);
    will_return(cx_bn_alloc_init, -1);
    will_return(cx_bn_unlock, CX_OK);
    assert_int_equal(big_int_random(&a, &modulus), -1);

    will_return(cx_bn_lock, CX_OK);
    will_return(cx_bn_alloc_init, CX_OK);
    will_return(cx_bn_alloc, -1);
    will_return(cx_bn_unlock, CX_OK);
    assert_int_equal(big_int_random(&a, &modulus), -1);

    will_return(cx_bn_lock, CX_OK);
    will_return(cx_bn_alloc_init, CX_OK);
    will_return(cx_bn_alloc, CX_OK);
    will_return(cx_bn_rng, -1);
    will_return(cx_bn_unlock, CX_OK);
    assert_int_equal(big_int_random(&a, &modulus), -1);

    will_return(cx_bn_lock, CX_OK);
    will_return(cx_bn_alloc_init, CX_OK);
    will_return(cx_bn_alloc, CX_OK);
    will_return(cx_bn_rng, CX_OK);
    will_return(cx_bn_export, 0);
    will_return(cx_bn_export, -1);
    will_return(cx_bn_unlock, CX_OK);
    assert_int_equal(big_int_random(&a, &modulus), -1);

    will_return(cx_bn_lock, CX_OK);
    will_return(cx_bn_alloc_init, CX_OK);
    will_return(cx_bn_alloc, CX_OK);
    will_return(cx_bn_rng, CX_OK);
    will_return(cx_bn_export, 0);
    will_return(cx_bn_export, CX_OK);
    will_return(cx_bn_unlock, -1);
    assert_int_equal(big_int_random(&a, &modulus), -1);

    will_return(cx_bn_lock, CX_OK);
    will_return(cx_bn_alloc_init, CX_OK);
    will_return(cx_bn_alloc, CX_OK);
    will_return(cx_bn_rng, CX_OK);
    will_return(cx_bn_export, bn_exp);
    will_return(cx_bn_export, CX_OK);
    will_return(cx_bn_unlock, CX_OK);
    assert_int_equal(big_int_random(&a, &modulus), CX_OK);
    assert_int_equal(a.u64[0], 0x7478f42e2d654a56);
    assert_int_equal(a.u64[1], 0xc654e4d15a654c32);
    assert_int_equal(a.u64[2], 0x5a3c2ca6cf9c5318);
    assert_int_equal(a.u64[3], 0x9383b2702a292d0f);
}

int main()
{
    const struct CMUnitTest tests[]
        = {cmocka_unit_test(test_u64), cmocka_unit_test(test_bigint_256)};

    return cmocka_run_group_tests(tests, NULL, NULL);
}
