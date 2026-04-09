#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <cmocka.h>

#include "cx_errors.h"
#include "fp256.h"

static fp256_parameters_t parameters = {
    .MODULUS_BITS = 253,
    // 8444461749428370424248824938781546531375899335154063827935233455917409239041
    .MODULUS
    = {.big.u64
       = {725501752471715841U, 6461107452199829505U, 6968279316240510977U, 1345280370688173398U}},
    .R
    = {.big.u64
       = {9015221291577245683U, 8239323489949974514U, 1646089257421115374U, 958099254763297437U}},
    .R2
    = {.big.u64
       = {2726216793283724667U, 14712177743343147295U, 12091039717619697043U, 81024008013859129U}},
    .INV = 725501752471715839U};

static void test_fp256(void **state)
{
    (void) state;

    fp256_t      a, b, r;
    bigint_256_t bg;

    // fp256_add_assign
    a.big.u64[0] = 0x9383b2702a292d0f;
    a.big.u64[1] = 0xc654e4d15a654c32;
    a.big.u64[2] = 0x19bf1456544eee45;
    a.big.u64[3] = 0x7478f42e2d654a56;

    b.big.u64[0] = 0x5a3c2ca6cf9c5318;
    b.big.u64[1] = 0x131d89e1d7531857;
    b.big.u64[2] = 0x15d45e7a83777c54;
    b.big.u64[3] = 0xc654e4d15a654c32;

    fp256_add_assign(&parameters, &a, &b);
    assert_int_equal(a.big.u64[0], 0xe3ae5f16f9c58026);
    assert_int_equal(a.big.u64[1], 0x7fc7f7b461b86488);
    assert_int_equal(a.big.u64[2], 0xcedf25b27b8eba98);
    assert_int_equal(a.big.u64[3], 0x282273a0ed9df131);

    a.big.u64[3] = 0x00ad89e1d7531800;
    b.big.u64[3] = 0x0019bf1456544e00;
    fp256_add_assign(&parameters, &a, &b);

    // fp256_sub_assign
    a.big.u64[0] = 0x9383b2702a292d0f;
    a.big.u64[1] = 0xc654e4d15a654c32;
    a.big.u64[2] = 0x19bf1456544eee45;
    a.big.u64[3] = 0x7478f42e2d654a56;

    b.big.u64[0] = 0x5a3c2ca6cf9c5318;
    b.big.u64[1] = 0x131d89e1d7531857;
    b.big.u64[2] = 0x15d45e7a83777c54;
    b.big.u64[3] = 0xc654e4d15a654c32;

    fp256_sub_assign(&parameters, &a, &b);
    assert_int_equal(a.big.u64[0], 0x435905c95a8cd9f8);
    assert_int_equal(a.big.u64[1], 0x0ce1d1ee531233dc);
    assert_int_equal(a.big.u64[2], 0x649f02fa2d0f21f3);
    assert_int_equal(a.big.u64[3], 0xc0cf74bb6d2ca37a);

    // fp256_mul_assign
    a.big.u64[0] = 0x9383b2702a292d0f;
    a.big.u64[1] = 0xc654e4d15a654c32;
    a.big.u64[2] = 0x19bf1456544eee45;
    a.big.u64[3] = 0x7478f42e2d654a56;

    b.big.u64[0] = 0x5a3c2ca6cf9c5318;
    b.big.u64[1] = 0x131d89e1d7531857;
    b.big.u64[2] = 0x15d45e7a83777c54;
    b.big.u64[3] = 0xc654e4d15a654c32;

    fp256_mul_assign(&parameters, &a, &b);
    assert_int_equal(a.big.u64[0], 0x18836bcb0f475f2c);
    assert_int_equal(a.big.u64[1], 0x62d00b3a5510d167);
    assert_int_equal(a.big.u64[2], 0x8459294fbc359457);
    assert_int_equal(a.big.u64[3], 0x4d2d6364ee81a48d);

    // fp256_pow_assign
    a.big.u64[0] = 0x9383b2702a292d0f;
    a.big.u64[1] = 0xc654e4d15a654c32;
    a.big.u64[2] = 0x19bf1456544eee45;
    a.big.u64[3] = 0x7478f42e2d654a56;

    fp256_pow_assign(&parameters, &a, 31);
    assert_int_equal(a.big.u64[0], 0x15d7c0ee9bda2feb);
    assert_int_equal(a.big.u64[1], 0xb69cc5049654a1cb);
    assert_int_equal(a.big.u64[2], 0x80ab52751ce7d912);
    assert_int_equal(a.big.u64[3], 0x0caed454ce7de5f1);

    // fp256_sum_of_products
    fp256_t array_a[9]={
    {.big.u64 = {0xd6c7b140f4848e16, 0x53991981a73f41c1, 0x4be72fee32ebe848, 0x0df5c5e7bd0c338e,}},
    {.big.u64 = {0xc6eadaff3c128b04, 0x37c430ae1daec691, 0xe12922da820dba75, 0x1121dd73267d6d57,}},
    {.big.u64 = {0xe99118064cfd0999, 0x25955e3ea47bfb3d, 0x7d57a10eb3c32639, 0x0db4791b8ba2a465,}},
    {.big.u64 = {0x17c5f21695404f89, 0x0446a4404843074c, 0xef9f24fd37e56874, 0x02717c7ef0ad3b18,}},
    {.big.u64 = {0x66181a54c8882d3e, 0x2721041f61e7cb30, 0x18e05457b053cbc9, 0x0716b0aab526190a,}},
    {.big.u64 = {0x4ab198049ad2a762, 0xfcd5ee7185e65901, 0x12afac9a1a98e79e, 0x112412f37ecf6d71,}},
    {.big.u64 = {0x05977bc95c8ffc5b, 0xae9c3b72131b58f2, 0x58086f110a950eec, 0x06a9b761c5ef90a6,}},
    {.big.u64 = {0xbe34f5b012e7279d, 0x5e535e33d50a512c, 0x94cc75d43719fe44, 0x0d291df320971baa,}},
    {.big.u64 = {0xb562b38623afd21b, 0x9de78a6bc3032948, 0x3c7c29ee858937fa, 0x0672bad7e17ec57c,}}};

    fp256_t array_b[9]={
    {.big.u64 = {0x971775a2bdf40803, 0x0d38e7d60de22dd4, 0x8a18a0e2806bed3c, 0x116bcacaccf56784,}},
    {.big.u64 = {0xcf7ee7d6ff9c266c, 0x5e0fade0afe0d233, 0xf9171a2d04f8da1d, 0x12655f2559cb4378,}},
    {.big.u64 = {0xa33477761ee894e8, 0x8472ac18eda3e34a, 0x9efc357cd1687ac4, 0x09e42e07c1ea762e,}},
    {.big.u64 = {0x38ab2034796fd0fb, 0xb3c0d138762c3221, 0x7659e068ee779c8e, 0x0935ce2c29881ada,}},
    {.big.u64 = {0x81b84197e0301f09, 0x48b27d4d20304353, 0x5ec2c672fbe75ac8, 0x070227d9eececc34,}},
    {.big.u64 = {0x40cdd495c5bd3ba9, 0x04cfebce1f78de96, 0x7611a496275f6966, 0x0791611be69aa1a3,}},
    {.big.u64 = {0x1669c35a1b2af770, 0x53224f5e9b585be7, 0xdb69f15dbfaa6868, 0x03b7259cc4195dce,}},
    {.big.u64 = {0xbefbe16974aa538e, 0xdc1c3d950b492c47, 0x2945c801df7264ab, 0x06880da2e28efd73,}},
    {.big.u64 = {0xf7e40ba6a8d77e57, 0x7327958e2a8795ca, 0x712289ba59184337, 0x06c68475207c59cf,}}};

    fp256_sum_of_products(&parameters, array_a, array_b, 9, &r);
    assert_int_equal(r.big.u64[0], 0x20da3ee61a63d447);
    assert_int_equal(r.big.u64[1], 0x2b39480e4c866675);
    assert_int_equal(r.big.u64[2], 0x8f471282613e2bed);
    assert_int_equal(r.big.u64[3], 0x0fbecc7f1306a05e);

    // fp256_inverse_assign
    a.big.u64[0] = 0x9383b2702a292d0f;
    a.big.u64[1] = 0xc654e4d15a654c32;
    a.big.u64[2] = 0x19bf1456544eee45;
    a.big.u64[3] = 0x7478f42e2d654a56;
    fp256_inverse_assign(&parameters, &a);
    assert_int_equal(a.big.u64[0], 0xeae1f63d827445bc);
    assert_int_equal(a.big.u64[1], 0xa5e8e9d9549a7d94);
    assert_int_equal(a.big.u64[2], 0x4480695451283c45);
    assert_int_equal(a.big.u64[3], 0x10371b8bc5b60c76);

    parameters.MODULUS.big.u64[0] = 1;
    parameters.MODULUS.big.u64[1] = 0;
    parameters.MODULUS.big.u64[2] = 0;
    parameters.MODULUS.big.u64[3] = 0;
    fp256_inverse_assign(&parameters, &a);
    parameters.MODULUS.big.u64[0] = 725501752471715841U;
    parameters.MODULUS.big.u64[1] = 6461107452199829505U;
    parameters.MODULUS.big.u64[2] = 6968279316240510977U;
    parameters.MODULUS.big.u64[3] = 1345280370688173398U;
    assert_int_equal(a.big.u64[0], 0);
    assert_int_equal(a.big.u64[1], 0);
    assert_int_equal(a.big.u64[2], 0);
    assert_int_equal(a.big.u64[3], 0);

    // fp256_from_big_int
    bg.u64[0] = 0x7611a496275f6966;
    bg.u64[1] = 0x40cdd495c5bd3ba9;
    bg.u64[2] = 0x9efc357cd1687ac4;
    bg.u64[3] = 0xa33477761ee894e8;
    fp256_from_big_int(&parameters, &a, &bg);
    assert_int_equal(a.big.u64[0], 0x964ee4830227ecc0);
    assert_int_equal(a.big.u64[1], 0x4cad68dee631abc9);
    assert_int_equal(a.big.u64[2], 0x132878186b8027ea);
    assert_int_equal(a.big.u64[3], 0xd760da4d7edda54);

    // fp256_from_int
    fp256_from_int(&parameters, &a, 0x38ab2034796fd0fb);
    assert_int_equal(a.big.u64[0], 0x4ad27dc1914fde2a);
    assert_int_equal(a.big.u64[1], 0x3dfe907e59b86bca);
    assert_int_equal(a.big.u64[2], 0xe4c363cce555d6a3);
    assert_int_equal(a.big.u64[3], 0x70934c2d648423c);

    // fp256_to_big_int
    a.big.u64[0] = 0x9383b2702a292d0f;
    a.big.u64[1] = 0xc654e4d15a654c32;
    a.big.u64[2] = 0x19bf1456544eee45;
    a.big.u64[3] = 0x7478f42e2d654a56;
    fp256_to_big_int(&parameters, &a, &bg);
    assert_int_equal(bg.u64[0], 0x67df4204a2135083);
    assert_int_equal(bg.u64[1], 0xa03d43c5c510935d);
    assert_int_equal(bg.u64[2], 0x8bd76288b0c83c9b);
    assert_int_equal(bg.u64[3], 0x10fb37b14db0af19);

    // fp256_random
    uint8_t bn_exp[32] = {0x93, 0x83, 0xb2, 0x70, 0x2a, 0x29, 0x2d, 0x0f, 0x5a, 0x3c, 0x2c,
                          0xa6, 0xcf, 0x9c, 0x53, 0x18, 0xc6, 0x54, 0xe4, 0xd1, 0x5a, 0x65,
                          0x4c, 0x32, 0x74, 0x78, 0xf4, 0x2e, 0x2d, 0x65, 0x4a, 0x56};

    will_return(cx_bn_lock, -1);
    assert_int_equal(fp256_random(&parameters, &a), -1);

    will_return(cx_bn_lock, CX_OK);
    will_return(cx_bn_alloc_init, CX_OK);
    will_return(cx_bn_alloc, CX_OK);
    will_return(cx_bn_rng, CX_OK);
    will_return(cx_bn_export, bn_exp);
    will_return(cx_bn_export, CX_OK);
    will_return(cx_bn_unlock, CX_OK);
    assert_int_equal(fp256_random(&parameters, &a), 0);
    assert_int_equal(a.big.u64[0], 0x54339e609325985e);
    assert_int_equal(a.big.u64[1], 0x315fa9d7ad0ade3d);
    assert_int_equal(a.big.u64[2], 0xef193321ddd2d28);
    assert_int_equal(a.big.u64[3], 0x17c813d113d6341);
}

int main()
{
    const struct CMUnitTest tests[] = {cmocka_unit_test(test_fp256)};

    return cmocka_run_group_tests(tests, NULL, NULL);
}