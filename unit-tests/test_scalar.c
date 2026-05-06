#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <cmocka.h>

#include "cx_errors.h"
#include "scalar.h"

static void test_scalar(void **state)
{
    (void) state;
    scalar_t     a, b;
    field_t      f;
    bigint_256_t bg;

    // scalar_to_big_int
    a.big.u64[0] = 0xf8b497fd5a5ba7b0;
    a.big.u64[1] = 0x7ea9409979484264;
    a.big.u64[2] = 0x8a05576f4c19f4bf;
    a.big.u64[3] = 0x0399f3f0056b5577;
    scalar_to_big_int(&a, &bg);
    assert_int_equal(bg.u64[0], 0x98d7f194f46bd2f2);
    assert_int_equal(bg.u64[1], 0x94f4c29b9ffd666a);
    assert_int_equal(bg.u64[2], 0x327cdc583337c33e);
    assert_int_equal(bg.u64[3], 0x426895f8a3e1759);

    // scalar_from_big_int
    bg.u64[0] = 0x98d7f194f46bd2f2;
    bg.u64[1] = 0x94f4c29b9ffd666a;
    bg.u64[2] = 0x327cdc583337c33e;
    bg.u64[3] = 0x426895f8a3e1759;
    scalar_from_big_int(&a, &bg);
    assert_int_equal(a.big.u64[0], 0xf8b497fd5a5ba7b0);
    assert_int_equal(a.big.u64[1], 0x7ea9409979484264);
    assert_int_equal(a.big.u64[2], 0x8a05576f4c19f4bf);
    assert_int_equal(a.big.u64[3], 0x0399f3f0056b5577);

    // scalar_from_field_lossy
    f.big.u64[0] = 0x1bd97226fc1c4fb3;
    f.big.u64[1] = 0xe094ababa2bed992;
    f.big.u64[2] = 0x64d9a482f2f08f5c;
    f.big.u64[3] = 0x0bf8c834b2aa2870;
    scalar_from_field_lossy(&a, &f);
    assert_int_equal(a.big.u64[0], 0xe83d119fb0ba4ce6);
    assert_int_equal(a.big.u64[1], 0x76ade84b8c9e529e);
    assert_int_equal(a.big.u64[2], 0x7135cb221d528e15);
    assert_int_equal(a.big.u64[3], 0x0166d0140b42a54e);

    // scalar_to_field
    a.big.u64[0] = 0xf8b497fd5a5ba7b0;
    a.big.u64[1] = 0x7ea9409979484264;
    a.big.u64[2] = 0x8a05576f4c19f4bf;
    a.big.u64[3] = 0x0399f3f0056b5577;
    scalar_to_field(&a, &f);
    assert_int_equal(f.big.u64[0], 0x243f5f8951f8acaf);
    assert_int_equal(f.big.u64[1], 0xf9f9e81acb96496f);
    assert_int_equal(f.big.u64[2], 0xfe907c62b2b45a44);
    assert_int_equal(f.big.u64[3], 0xc32d8c4d353fae0);

    // scalar_add_assign
    a.big.u64[0] = 0xf8b497fd5a5ba7b0;
    a.big.u64[1] = 0x7ea9409979484264;
    a.big.u64[2] = 0x8a05576f4c19f4bf;
    a.big.u64[3] = 0x0399f3f0056b5577;

    b.big.u64[0] = 0xe83d119fb0ba4ce6;
    b.big.u64[1] = 0x76ade84b8c9e529e;
    b.big.u64[2] = 0x7135cb221d528e15;
    b.big.u64[3] = 0x0166d0140b42a54e;
    scalar_add_assign(&a, &b);
    assert_int_equal(a.big.u64[0], 0x2796bb0247d61a97);
    assert_int_equal(a.big.u64[1], 0xa2c3853541aa0a05);
    assert_int_equal(a.big.u64[2], 0x630e0f49d25e96d4);
    assert_int_equal(a.big.u64[3], 0x0055eaac6a22d170);

    // scalar_sub_assign
    a.big.u64[0] = 0xe83d119fb0ba4ce6;
    a.big.u64[1] = 0x76ade84b8c9e529e;
    a.big.u64[2] = 0x7135cb221d528e15;
    a.big.u64[3] = 0x0166d0140b42a54e;

    b.big.u64[0] = 0xf8b497fd5a5ba7b0;
    b.big.u64[1] = 0x7ea9409979484264;
    b.big.u64[2] = 0x8a05576f4c19f4bf;
    b.big.u64[3] = 0x0399f3f0056b5577;
    scalar_sub_assign(&a, &b);
    assert_int_equal(a.big.u64[0], 0xa8e3683d199e7f35);
    assert_int_equal(a.big.u64[1], 0x4a984b61d7929b38);
    assert_int_equal(a.big.u64[2], 0x7f5d86fa68468556);
    assert_int_equal(a.big.u64[3], 0x0277b57bac62792c);

    // scalar_mul_assign
    a.big.u64[0] = 0xe83d119fb0ba4ce6;
    a.big.u64[1] = 0x76ade84b8c9e529e;
    a.big.u64[2] = 0x7135cb221d528e15;
    a.big.u64[3] = 0x0166d0140b42a54e;

    b.big.u64[0] = 0x2796bb0247d61a97;
    b.big.u64[1] = 0xa2c3853541aa0a05;
    b.big.u64[2] = 0x630e0f49d25e96d4;
    b.big.u64[3] = 0x0055eaac6a22d170;
    scalar_mul_assign(&a, &b);
    assert_int_equal(a.big.u64[0], 0x66900f8b3bd0b943);
    assert_int_equal(a.big.u64[1], 0x713597e811f584b4);
    assert_int_equal(a.big.u64[2], 0xb7704ef90339049c);
    assert_int_equal(a.big.u64[3], 0x01271da5fe3d50c3);

    // scalar_random
    uint8_t bn_exp[32] = {0xd1, 0x83, 0xe4, 0x70, 0x2a, 0x29, 0x32, 0x0f, 0x5a, 0x3c, 0x2c,
                          0xa6, 0x29, 0x9c, 0x53, 0x4c, 0xc6, 0x54, 0xe4, 0xd1, 0x5a, 0x65,
                          0x4a, 0x32, 0x74, 0x78, 0xf4, 0x2e, 0x83, 0x65, 0x4a, 0xc6};

    will_return(cx_bn_lock, CX_OK);
    will_return(cx_bn_alloc_init, CX_OK);
    will_return(cx_bn_alloc, CX_OK);
    will_return(cx_bn_rng, CX_OK);
    will_return(cx_bn_export, bn_exp);
    will_return(cx_bn_export, CX_OK);
    will_return(cx_bn_unlock, CX_OK);
    assert_int_equal(scalar_random(&a), 0);
    assert_int_equal(a.big.u64[0], 0xf8b497fd5a5ba7b0);
    assert_int_equal(a.big.u64[1], 0x7ea9409979484264);
    assert_int_equal(a.big.u64[2], 0x8a05576f4c19f4bf);
    assert_int_equal(a.big.u64[3], 0x0399f3f0056b5577);
}

int main()
{
    const struct CMUnitTest tests[] = {cmocka_unit_test(test_scalar)};

    return cmocka_run_group_tests(tests, NULL, NULL);
}