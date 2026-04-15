#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <cmocka.h>

#include "cx_errors.h"
#include "field.h"

static void test_field(void **state)
{
    (void) state;
    field_t      a, b, r;
    bigint_256_t bg;

    // field_assign
    b.big.u64[0] = 0x55c0de02de4acbd3;
    b.big.u64[1] = 0xcb509de8001f15cb;
    b.big.u64[2] = 0xb5a0818df06d4016;
    b.big.u64[3] = 0x07eb1804280e9e3d;
    field_assign(&a, &b);
    assert_int_equal(a.big.u64[0], 0x55c0de02de4acbd3);
    assert_int_equal(a.big.u64[1], 0xcb509de8001f15cb);
    assert_int_equal(a.big.u64[2], 0xb5a0818df06d4016);
    assert_int_equal(a.big.u64[3], 0x07eb1804280e9e3d);

    // field_add_assign
    a.big.u64[0] = 0x1bd97226fc1c4fb3;
    a.big.u64[1] = 0xe094ababa2bed992;
    a.big.u64[2] = 0x64d9a482f2f08f5c;
    a.big.u64[3] = 0x0bf8c834b2aa2870;

    b.big.u64[0] = 0x55c0de02de4acbd3;
    b.big.u64[1] = 0xcb509de8001f15cb;
    b.big.u64[2] = 0xb5a0818df06d4016;
    b.big.u64[3] = 0x07eb1804280e9e3d;
    field_add_assign(&a, &b);
    assert_int_equal(a.big.u64[0], 0x6788d029da671b85);
    assert_int_equal(a.big.u64[1], 0x523ad294d2ddef5c);
    assert_int_equal(a.big.u64[2], 0xb9c5d8f287261f72);
    assert_int_equal(a.big.u64[3], 0x01387ada408c2157);

    // field_mul_assign
    a.big.u64[0] = 0x1bd97226fc1c4fb3;
    a.big.u64[1] = 0xe094ababa2bed992;
    a.big.u64[2] = 0x64d9a482f2f08f5c;
    a.big.u64[3] = 0x0bf8c834b2aa2870;

    b.big.u64[0] = 0x55c0de02de4acbd3;
    b.big.u64[1] = 0xcb509de8001f15cb;
    b.big.u64[2] = 0xb5a0818df06d4016;
    b.big.u64[3] = 0x07eb1804280e9e3d;
    field_mul_assign(&a, &b);
    assert_int_equal(a.big.u64[0], 0x5e2aefb0f0f28e84);
    assert_int_equal(a.big.u64[1], 0x4f6cdf628f2e350a);
    assert_int_equal(a.big.u64[2], 0xade171814dd2ad1e);
    assert_int_equal(a.big.u64[3], 0x06760a8bd1309115);

    // field_inverse_assign
    a.big.u64[0] = 0x1bd97226fc1c4fb3;
    a.big.u64[1] = 0xe094ababa2bed992;
    a.big.u64[2] = 0x64d9a482f2f08f5c;
    a.big.u64[3] = 0x0bf8c834b2aa2870;
    field_inverse_assign(&a);
    assert_int_equal(a.big.u64[0], 0xf2149802cf06fb3d);
    assert_int_equal(a.big.u64[1], 0x86e433c0497f4b2a);
    assert_int_equal(a.big.u64[2], 0xb2c8269a7c8c77b7);
    assert_int_equal(a.big.u64[3], 0x051a587248cddfa0);

    // field_pow_assign
    a.big.u64[0] = 0x1bd97226fc1c4fb3;
    a.big.u64[1] = 0xe094ababa2bed992;
    a.big.u64[2] = 0x64d9a482f2f08f5c;
    a.big.u64[3] = 0x0bf8c834b2aa2870;
    field_pow_assign(&a, 15);
    assert_int_equal(a.big.u64[0], 0x3266dde297982573);
    assert_int_equal(a.big.u64[1], 0x81b53b7a285538e1);
    assert_int_equal(a.big.u64[2], 0xe1be6082e7fa5e67);
    assert_int_equal(a.big.u64[3], 0x0c57dde71ccb6819);

    // field_sum_of_products
    field_t array_a[8]={
    {.big.u64 = {0x34c37d28c3f4c1ce, 0xa15d6ace8616e46e, 0x23cbab5413838926, 0x104c314c3151dae4,}},
    {.big.u64 = {0xaa647468e42a8bf2, 0x1fbfc06f37137b2e, 0xad99ee12341f6ede, 0x0bbfd74e0ec870e1,}},
    {.big.u64 = {0xddbbbc740d2294ca, 0xc3e128c0d7dd604d, 0x255f41bb46a79480, 0x068dbe6acd82f805,}},
    {.big.u64 = {0x90f678904cdbbb1b, 0x8cca6e86b931e33c, 0x2f5a8c702f02d1ef, 0x04f922bd39962578,}},
    {.big.u64 = {0xec1c2b7fa2cf16f3, 0x56cff368c27540df, 0xe94ce9e353905aa6, 0x08b69ab751354e78,}},
    {.big.u64 = {0xdcdb0ce947415b57, 0xa58640bf51d2d722, 0x1507d69f356b3efa, 0x09aef6be7d445a91,}},
    {.big.u64 = {0xada311e49e6344d4, 0x6b54bb8bf3f6c3bc, 0x2375d4e6dfa3f68a, 0x02a7697b221d8309,}},
    {.big.u64 = {0xfdcd4e42c21b4c57, 0x2fdf1f71db60dc35, 0x54e5fcd2cfca7e88, 0x073e18896b646529,}}};

    field_t array_b[8]={
    {.big.u64 = {0x9f617291b171a9f7, 0x3b039dfba1472575, 0x1869215c5eb1ec7d, 0x107fc91d08fb367b,}},
    {.big.u64 = {0xb587b1b73e934464, 0x5cc72919492a496d, 0x9da440f0b532735a, 0x06a44c5c19d392d9,}},
    {.big.u64 = {0x06a93869c3c208a1, 0xb529badf8f835874, 0x2a945765cc84e74d, 0x0f0bbf3cef073008,}},
    {.big.u64 = {0x7fe548f7e0aeded1, 0x95e00b600db64ff8, 0x44928f2f7cec7269, 0x08544cadc2117384,}},
    {.big.u64 = {0x400d083772377385, 0xcb269d24b1589ecc, 0x819fbeb9d515fa7c, 0x07f28567b239cc59,}},
    {.big.u64 = {0xcd8f3be99e042143, 0xcdeb14f751d2807d, 0xc8fa3207c948296f, 0x023e97cb99a87c8c,}},
    {.big.u64 = {0xc06dfd6c5b8eee77, 0x9a9c8b2f8c3ebde7, 0xa002e85c30aa07e4, 0x0b3cf51fb1859e50,}},
    {.big.u64 = {0xe14bc6e625da6023, 0x89eea52c5f07c766, 0x3c781bffdf3730b0, 0x094a532f8a00ea35,}}};
    field_sum_of_products(array_a, array_b, 8, &r);
    assert_int_equal(r.big.u64[0], 0x3bb8783cbc7a5d15);
    assert_int_equal(r.big.u64[1], 0xf84087c1b623f15f);
    assert_int_equal(r.big.u64[2], 0x950777144c1b84d8);
    assert_int_equal(r.big.u64[3], 0x0539e5a46a26a079);

    // field_from_int
    field_from_int(&a, 1);
    assert_int_equal(a.big.u64[0], FIELD_ONE.big.u64[0]);
    assert_int_equal(a.big.u64[1], FIELD_ONE.big.u64[1]);
    assert_int_equal(a.big.u64[2], FIELD_ONE.big.u64[2]);
    assert_int_equal(a.big.u64[3], FIELD_ONE.big.u64[3]);

    // field_to_big_int
    a.big.u64[0] = 0x1bd97226fc1c4fb3;
    a.big.u64[1] = 0xe094ababa2bed992;
    a.big.u64[2] = 0x64d9a482f2f08f5c;
    a.big.u64[3] = 0x0bf8c834b2aa2870;
    field_to_big_int(&a, &bg);
    assert_int_equal(bg.u64[0], 0xe0e976c3186def20);
    assert_int_equal(bg.u64[1], 0xa8966acac29b7f62);
    assert_int_equal(bg.u64[2], 0xcf874534924510bf);
    assert_int_equal(bg.u64[3], 0x1037df861b82f83d);

    // field_from_big_int
    bg.u64[0] = 0xe0e976c3186def20;
    bg.u64[1] = 0xa8966acac29b7f62;
    bg.u64[2] = 0xcf874534924510bf;
    bg.u64[3] = 0x1037df861b82f83d;
    field_from_big_int(&a, &bg);
    assert_int_equal(a.big.u64[0], 0x1bd97226fc1c4fb3);
    assert_int_equal(a.big.u64[1], 0xe094ababa2bed992);
    assert_int_equal(a.big.u64[2], 0x64d9a482f2f08f5c);
    assert_int_equal(a.big.u64[3], 0x0bf8c834b2aa2870);

    // field_from_bits
    uint8_t bit_buffer[32] = {0x56, 0x4a, 0x65, 0x2d, 0x2e, 0xf4, 0x78, 0x74, 0x32, 0x4c, 0x65,
                              0x5a, 0xd1, 0xe4, 0x54, 0xc6, 0x18, 0x53, 0x9c, 0xcf, 0xa6, 0x2c,
                              0x3c, 0x5a, 0x0f, 0x2d, 0x29, 0x2a, 0x70, 0xb2, 0x83, 0x93};
    assert_int_equal(field_from_bits(bit_buffer, 32 * 8, &r, 2), 2);
    assert_int_equal(r.big.u64[0], 0x00ced3078b6ebd49);
    assert_int_equal(r.big.u64[1], 0x4f56c9d15754ac1c);
    assert_int_equal(r.big.u64[2], 0xd2b0a156dddb9305);
    assert_int_equal(r.big.u64[3], 0x045ab91c572dc110);

    // field_random
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
    assert_int_equal(field_random(&a), 0);
    assert_int_equal(a.big.u64[0], 0xcab61523312e75b0);
    assert_int_equal(a.big.u64[1], 0x579edc96a597c3f);
    assert_int_equal(a.big.u64[2], 0xaf709554c2711b97);
    assert_int_equal(a.big.u64[3], 0x742a09efa797129);
}

int main()
{
    const struct CMUnitTest tests[] = {cmocka_unit_test(test_field)};

    return cmocka_run_group_tests(tests, NULL, NULL);
}