#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <cmocka.h>

#include "os.h"
#include "cx.h"
#include "account.h"

static void check_field(field_t *a, field_t *b)
{
    assert_int_equal(a->big.u64[0], b->big.u64[0]);
    assert_int_equal(a->big.u64[1], b->big.u64[1]);
    assert_int_equal(a->big.u64[2], b->big.u64[2]);
    assert_int_equal(a->big.u64[3], b->big.u64[3]);
}

static void check_scalar(scalar_t *a, scalar_t *b)
{
    assert_int_equal(a->big.u64[0], b->big.u64[0]);
    assert_int_equal(a->big.u64[1], b->big.u64[1]);
    assert_int_equal(a->big.u64[2], b->big.u64[2]);
    assert_int_equal(a->big.u64[3], b->big.u64[3]);
}

static void check_group(group_t *a, group_t *b)
{
    assert_int_equal(a->x.big.u64[0], b->x.big.u64[0]);
    assert_int_equal(a->x.big.u64[1], b->x.big.u64[1]);
    assert_int_equal(a->x.big.u64[2], b->x.big.u64[2]);
    assert_int_equal(a->x.big.u64[3], b->x.big.u64[3]);
    assert_int_equal(a->y.big.u64[0], b->y.big.u64[0]);
    assert_int_equal(a->y.big.u64[1], b->y.big.u64[1]);
    assert_int_equal(a->y.big.u64[2], b->y.big.u64[2]);
    assert_int_equal(a->y.big.u64[3], b->y.big.u64[3]);
}

static void test_account(void **state)
{
    (void) state;

    uint32_t path[4]                  = {0x8000002c, 0x800002ab, 0x80000000, 0x80000000};
    char     address[ADDRESS_LEN + 1] = {0};

    will_return_always(cx_bn_lock, CX_OK);
    will_return_always(cx_ecpoint_alloc, CX_OK);
    will_return_always(cx_ecpoint_init, CX_OK);
    will_return_always(cx_ecpoint_scalarmul, CX_OK);
    will_return_always(cx_ecpoint_export, CX_OK);
    will_return_always(cx_ecpoint_destroy, CX_OK);
    will_return_always(cx_bn_unlock, CX_OK);

    // account_get_address_string
    uint8_t bn_seed[32] = {0xff, 0xc3, 0xde, 0x3c, 0x85, 0x23, 0x3e, 0x2c, 0xca, 0x13, 0x90,
                           0xdb, 0xdd, 0x6b, 0x6f, 0x04, 0x5b, 0x6a, 0x74, 0xfa, 0xde, 0x6e,
                           0x58, 0x07, 0xd2, 0xe3, 0x15, 0x27, 0x05, 0xea, 0x65, 0x7e};

    will_return(sys_hdkey_derive, bn_seed);
    will_return(sys_hdkey_derive, SWO_OK);
    assert_int_equal(account_get_address_string(path, 4, address), 0);
    assert_string_equal(address, "aleo1c8hk0s7zfrmg99e4t0ekd9mgt5qgtz2yvfjamfgujndy959srufqqawxzz");

    will_return(sys_hdkey_derive, bn_seed);
    will_return(sys_hdkey_derive, 0x0001);
    assert_int_equal(account_get_address_string(path, 4, address), -1);

    // account_get_view_key_string
    memset(address, 0, sizeof(address));
    will_return(sys_hdkey_derive, bn_seed);
    will_return(sys_hdkey_derive, SWO_OK);
    assert_int_equal(account_get_view_key_string(path, 4, address), 0);
    assert_string_equal(address, "AViewKey1hyAypRRpyq2JDjv3UpdRZdQjHrWyfPLbKf74QjgiQcjC");

    will_return(sys_hdkey_derive, bn_seed);
    will_return(sys_hdkey_derive, 0x0001);
    assert_int_equal(account_get_view_key_string(path, 4, address), -1);

    // account_generate_keys
    account_t account;
    will_return(sys_hdkey_derive, bn_seed);
    will_return(sys_hdkey_derive, SWO_OK);
    assert_int_equal(account_generate_keys(path, 4, &account), 0);
    field_t seed = {
        .big.u64 = {0x326dab4d918236e0, 0xec37e9452f68725, 0x7099819b51333787, 0x46977186edbf965}
    };
    check_field(&account.private_key.seed, &seed);
    scalar_t sk_sig = {
        .big.u64
        = {0x07eabdc6534c858f, 0x44f1027b4d903652, 0xe8496250b2b42c9a, 0x044f53f538456ce2}
    };
    check_scalar(&account.private_key.sk_sig, &sk_sig);
    scalar_t r_sig = {
        .big.u64
        = {0xe05c5bfcd63c0709, 0x928969617eb6172e, 0xa1bef1a9817696c4, 0x02cab0981b27e5e5}
    };
    check_scalar(&account.private_key.r_sig, &r_sig);
    group_t pk_sig = {
        .x.big.u64
        = {0xde8cc5ec217ad253, 0xea6bd6b4a4d0cf4b, 0x9556a907cd59336b, 0x0a3d18918155940e},
        .y.big.u64
        = {0xe456322861acf6ac, 0x68b72f3d5529218e, 0x3cbe762b292509f8, 0x0c03f4d9376b27cd}
    };
    check_group(&account.compute_key.pk_sig, &pk_sig);
    group_t pr_sig = {
        .x.big.u64
        = {0x773d1f206139cff5, 0xef1122c53bf34151, 0x58c4735db29ca6a7, 0x11d1c7a9b23a52f5},
        .y.big.u64
        = {0x799be5fcf4f15c35, 0x674847c970493e9b, 0xe1cb4589e5b4c7f3, 0x01c2c9083e4798e1}
    };
    check_group(&account.compute_key.pr_sig, &pr_sig);
    scalar_t sk_prf = {
        .big.u64
        = {0xa99e4b1f9b8435cb, 0x973b73acef28d87e, 0x84efb6bac52969ca, 0x0401510674801756}
    };
    check_scalar(&account.compute_key.sk_prf, &sk_prf);
    scalar_t view_key = {
        .big.u64
        = {0x1f2f87ad3e8d0e65, 0xc98e982a32f61002, 0xde9de425cb385528, 0x01c5a2e47ad71773}
    };
    check_scalar(&account.view_key, &view_key);
    group_t addr = {
        .x.big.u64
        = {0xce0f78517f7376e6, 0xf4ea033ea47d149e, 0x287213cc619348f0, 0x06687fceca169ebd},
        .y.big.u64
        = {0x013ace29509f45c6, 0x37979a102f766dd3, 0xcf78a3e195b59666, 0x0893f1ae42d7df25}
    };
    check_group(&account.address, &addr);
    field_t graph_key = {
        .big.u64
        = {0x837f9d098b4fd96a, 0x8e3724af86c9b19d, 0x19dbeeebcba9e6f8, 0x0caf51a7e4157238}
    };
    check_field(&account.graph_key, &graph_key);

    will_return(sys_hdkey_derive, bn_seed);
    will_return(sys_hdkey_derive, 0x0001);
    assert_int_equal(account_generate_keys(path, 4, &account), -1);
}

int main()
{
    const struct CMUnitTest tests[] = {cmocka_unit_test(test_account)};

    return cmocka_run_group_tests(tests, NULL, NULL);
}
