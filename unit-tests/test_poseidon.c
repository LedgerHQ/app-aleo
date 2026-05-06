#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <cmocka.h>

#include "poseidon.h"

static void test_poseidon(void **state)
{
    field_t r;

    // hash_to_scalar_psd2

    field_t hash_input_0[6] = {
    {.big.u64 = {0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,}},
    {.big.u64 = {0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,}},
    {.big.u64 = {0xb87b2455a433f4af, 0xf871a198d59dceb7, 0x0f0c0dc8cfccae52, 0x050480245a2fd0f3,}},
    {.big.u64 = {0x65c96345906465bc, 0x4d102a632b5c3864, 0x958abd70d99a4e47, 0x0805d82c1d4d1dac,}},
    {.big.u64 = {0xc188075f55d9e35a, 0x57db16a1f99ac37f, 0x917a9fbee3e84bfe, 0x0da5044ed3ec926b,}},
    {.big.u64 = {0x2425606f78e318d2, 0xb2dc9e5f260f6817, 0xfa822b7742b677ac, 0x10ad41a2309f5020,}}};

    assert_int_equal(hash_to_scalar_psd2(hash_input_0, 1, &r), -1);

    assert_int_equal(hash_to_scalar_psd2(hash_input_0, 6, &r), 0);
    assert_int_equal(r.big.u64[0], 0x5fb376d0ab86da25);
    assert_int_equal(r.big.u64[1], 0xa1ec059bf1558909);
    assert_int_equal(r.big.u64[2], 0x9ef7116dd32118c8);
    assert_int_equal(r.big.u64[3], 0x022094f6a3892f92);

    // hash_psd2
    assert_int_equal(hash_psd2(hash_input_0, 6, &r), 0);
    assert_int_equal(r.big.u64[0], 0x0461a177c923ff50);
    assert_int_equal(r.big.u64[1], 0xf9d9a3a6505439b4);
    assert_int_equal(r.big.u64[2], 0xd93dc22656892157);
    assert_int_equal(r.big.u64[3], 0x0cfc6ba1ccddf9fa);

    // hash_to_scalar_psd4
    field_t hash_input_1[8] = {
    {.big.u64 = {0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,}},
    {.big.u64 = {0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,}},
    {.big.u64 = {0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,}},
    {.big.u64 = {0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,}},
    {.big.u64 = {0xc188075f55d9e35a, 0x57db16a1f99ac37f, 0x917a9fbee3e84bfe, 0x0da5044ed3ec926b,}},
    {.big.u64 = {0x2425606f78e318d2, 0xb2dc9e5f260f6817, 0xfa822b7742b677ac, 0x10ad41a2309f5020,}},
    {.big.u64 = {0x688ea9cfe303b7c8, 0x139fa7e653420b04, 0xb4802dacdfa1fbde, 0x01b635cf3389e678,}},
    {.big.u64 = {0x6938538c3b33f939, 0x63af4ac9c2de1cc1, 0x67dc46362bc6e521, 0x07c3c9118294bb8c,}}};
    assert_int_equal(hash_to_scalar_psd4(hash_input_1, 8, &r), 0);
    assert_int_equal(r.big.u64[0], 0xe19f21ae2ae9e21d);
    assert_int_equal(r.big.u64[1], 0xeec79052e925c5b6);
    assert_int_equal(r.big.u64[2], 0xd9dde93dd746ea0f);
    assert_int_equal(r.big.u64[3], 0x02c9887205f68290);

    // hash_psd4
    assert_int_equal(hash_psd4(hash_input_1, 8, &r), 0);
    assert_int_equal(r.big.u64[0], 0xee38c339cb0b98a5);
    assert_int_equal(r.big.u64[1], 0x21480b39f97238eb);
    assert_int_equal(r.big.u64[2], 0x7bcca410ec3658e9);
    assert_int_equal(r.big.u64[3], 0x11218dda389feecd);

    // hash_to_scalar_psd8
    field_t hash_input_2[12] = {
    {.big.u64 = {0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,}},
    {.big.u64 = {0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,}},
    {.big.u64 = {0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,}},
    {.big.u64 = {0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,}},
    {.big.u64 = {0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,}},
    {.big.u64 = {0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,}},
    {.big.u64 = {0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,}},
    {.big.u64 = {0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,}},
    {.big.u64 = {0xc188075f55d9e35a, 0x57db16a1f99ac37f, 0x917a9fbee3e84bfe, 0x0da5044ed3ec926b,}},
    {.big.u64 = {0x2425606f78e318d2, 0xb2dc9e5f260f6817, 0xfa822b7742b677ac, 0x10ad41a2309f5020,}},
    {.big.u64 = {0x688ea9cfe303b7c8, 0x139fa7e653420b04, 0xb4802dacdfa1fbde, 0x01b635cf3389e678,}},
    {.big.u64 = {0x6938538c3b33f939, 0x63af4ac9c2de1cc1, 0x67dc46362bc6e521, 0x07c3c9118294bb8c,}}};
    assert_int_equal(hash_to_scalar_psd8(hash_input_2, 12, &r), 0);
    assert_int_equal(r.big.u64[0], 0x347aac07cc447252);
    assert_int_equal(r.big.u64[1], 0x79d6bb537fe35752);
    assert_int_equal(r.big.u64[2], 0x81766f8d678fe177);
    assert_int_equal(r.big.u64[3], 0x034d4149494e6939);

    // hash_psd8
    assert_int_equal(hash_psd8(hash_input_2, 8, &r), 0);
    assert_int_equal(r.big.u64[0], 0x6316e0db4d0f89aa);
    assert_int_equal(r.big.u64[1], 0xe0ed66e1308f11af);
    assert_int_equal(r.big.u64[2], 0x345e2d481ff91bfa);
    assert_int_equal(r.big.u64[3], 0x00543b2611f68e96);

    // hash_many_psd8
    field_t rr[32];
    assert_int_equal(hash_many_psd8(hash_input_1, 8, rr, 16), -1);

    assert_int_equal(hash_many_psd8(hash_input_1, 8, rr, 4), 0);
    assert_int_equal(rr[0].big.u64[0], 0x6316e0db4d0f89aa);
    assert_int_equal(rr[0].big.u64[1], 0xe0ed66e1308f11af);
    assert_int_equal(rr[0].big.u64[2], 0x345e2d481ff91bfa);
    assert_int_equal(rr[0].big.u64[3], 0x00543b2611f68e96);
    assert_int_equal(rr[1].big.u64[0], 0x00aa8f8a22a1115d);
    assert_int_equal(rr[1].big.u64[1], 0x29dfce148b56b696);
    assert_int_equal(rr[1].big.u64[2], 0xeb8236fc3260181b);
    assert_int_equal(rr[1].big.u64[3], 0x04d4f336ff6e1d27);
    assert_int_equal(rr[2].big.u64[0], 0x4a8752ffb52e8f81);
    assert_int_equal(rr[2].big.u64[1], 0x652d38580155bb2b);
    assert_int_equal(rr[2].big.u64[2], 0xdb3d30ede49145f3);
    assert_int_equal(rr[2].big.u64[3], 0x0d5d141b86b17214);
    assert_int_equal(rr[3].big.u64[0], 0x7f5418e3e3428896);
    assert_int_equal(rr[3].big.u64[1], 0xa84272cee03b2196);
    assert_int_equal(rr[3].big.u64[2], 0x7776089124e5da64);
    assert_int_equal(rr[3].big.u64[3], 0x07b6c34a4e1cf223);
}

int main()
{
    const struct CMUnitTest tests[] = {cmocka_unit_test(test_poseidon)};

    return cmocka_run_group_tests(tests, NULL, NULL);
}
