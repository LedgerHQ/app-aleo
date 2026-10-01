#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <cmocka.h>

#include "os.h"
#include "cx.h"
#include "types.h"
#include "account.h"

global_ctx_t G_context;

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

static void prepare_random_ok(uint8_t *random_bn)
{
    will_return(cx_bn_alloc_init, CX_OK);
    will_return(cx_bn_alloc, CX_OK);
    will_return(cx_bn_rng, CX_OK);
    will_return(cx_bn_export, random_bn);
    will_return(cx_bn_export, CX_OK);
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
    will_return_always(cx_ecpoint_rnd_scalarmul, CX_OK);
    will_return_always(cx_ecpoint_export, CX_OK);
    will_return_always(cx_ecpoint_destroy, CX_OK);
    will_return_always(cx_bn_unlock, CX_OK);

    // account_get_address_string
    uint8_t bn_seed[32] = {0xcd, 0x28, 0x45, 0x51, 0xff, 0x6f, 0x3f, 0x39, 0x3d, 0x74, 0xac,
                           0x8b, 0x78, 0x2a, 0x04, 0xec, 0x9c, 0x56, 0xe4, 0xa0, 0xa6, 0x87,
                           0xd2, 0x3f, 0xe6, 0x89, 0xf3, 0x64, 0x22, 0x1e, 0x15, 0xe6};

    will_return(sys_hdkey_derive, bn_seed);
    will_return(sys_hdkey_derive, SWO_OK);
    assert_int_equal(account_get_address_string(path, 4, address), 0);
    assert_string_equal(address, "aleo1k7349nakx72h3q3gm8zg6f0dksnkp9p9ha93lrx6zu2gnvjzpyrsj28j54");

    will_return(sys_hdkey_derive, bn_seed);
    will_return(sys_hdkey_derive, 0x0001);
    assert_int_equal(account_get_address_string(path, 4, address), -1);

    // account_get_view_key_string
    memset(address, 0, sizeof(address));
    will_return(sys_hdkey_derive, bn_seed);
    will_return(sys_hdkey_derive, SWO_OK);
    assert_int_equal(account_get_view_key_string(path, 4, address), 0);
    assert_string_equal(address, "AViewKey1fnXDtDJz1Vr8hRFXa7ZxwWA37E3TX9MrQJei691gSJkA");

    will_return(sys_hdkey_derive, bn_seed);
    will_return(sys_hdkey_derive, 0x0001);
    assert_int_equal(account_get_view_key_string(path, 4, address), -1);

    // account_generate_keys
    will_return(sys_hdkey_derive, bn_seed);
    will_return(sys_hdkey_derive, SWO_OK);
    assert_int_equal(account_generate_keys(path, 4, &G_context.account), 0);
    field_t seed = {
        .big.u64
        = {0x0996c322a2025bcc, 0xfef1016dbdaf6040, 0xc2c27d26118a9f12, 0x03bfbdc8f5eda6a3}
    };
    check_field(&G_context.account.private_key.seed, &seed);
    scalar_t sk_sig = {
        .big.u64
        = {0x1c73b9d53b26e207, 0x6b23d27b323bda6f, 0xbe52721ab5cac4e5, 0x03a99198a3e8b863}
    };
    check_scalar(&G_context.account.private_key.sk_sig, &sk_sig);
    scalar_t r_sig = {
        .big.u64
        = {0xfac590fb3eeae0bf, 0xc033c550a1d33c34, 0x152e40e33c016f6e, 0x044be9621b55abb3}
    };
    check_scalar(&G_context.account.private_key.r_sig, &r_sig);
    group_t pk_sig = {
        .x.big.u64
        = {0x65a90e2367fd4bde, 0x4b13e57d98114bfc, 0x2fd9cc6dcc66de0f, 0x01fd1c40c990feb1},
        .y.big.u64
        = {0x12b301c833a15757, 0x19745bef29e672c9, 0x1e5213d746e0db94, 0x11b372291ed5af5b}
    };
    check_group(&G_context.account.compute_key.pk_sig, &pk_sig);
    group_t pr_sig = {
        .x.big.u64
        = {0x58440841ace0d328, 0x20a752f3969513d4, 0x0f5c1d14a6ea85c6, 0x0212667895211a48},
        .y.big.u64
        = {0x1193ec8be3b283f8, 0x32f0fb029658036e, 0xa5f1dc6c59fa0815, 0x0c01374a0f53370a}
    };
    check_group(&G_context.account.compute_key.pr_sig, &pr_sig);
    scalar_t sk_prf = {
        .big.u64
        = {0x35749f41df37a25a, 0x13a14bd33eb8a0d3, 0xec2b547f2a8661cf, 0x028c8288e1fdd8be}
    };
    check_scalar(&G_context.account.compute_key.sk_prf, &sk_prf);
    scalar_t view_key = {
        .big.u64
        = {0xd9f80cdcd2c9b122, 0x99d19c3f8a4ea179, 0x8f51e0edee36be22, 0x012c4ad45425ea2a}
    };
    check_scalar(&G_context.account.view_key, &view_key);
    group_t addr = {
        .x.big.u64
        = {0x9e18053365eb34c0, 0x8c2a530cb9e43a0f, 0xdbd653f3e9580c3a, 0x025879fc05f3d59c},
        .y.big.u64
        = {0x11266f855761be08, 0x5348d3f24208293e, 0x9e1e145233e0683f, 0x073efb150d267dcd}
    };
    check_group(&G_context.account.address, &addr);
    field_t graph_key = {
        .big.u64
        = {0x58a0133558eb07d3, 0x5200b9c8a42639a0, 0x2a8a2361a86d4132, 0x09f021890c5ed0d6}
    };
    check_field(&G_context.account.graph_key, &graph_key);

    will_return(sys_hdkey_derive, bn_seed);
    will_return(sys_hdkey_derive, 0x0001);
    assert_int_equal(account_generate_keys(path, 4, &G_context.account), -1);

    // account_erase
    account_t empty_account = {0};
    account_erase(&G_context.account);
    assert_memory_equal(&G_context.account, &empty_account, sizeof(account_t));

    // account_parse_and_check_bip32_path
    uint32_t bip32_path[MAX_BIP32_PATH];
    uint8_t  bip32_path_len;
    buffer_t cdata_1 = {
        .ptr  = (uint8_t *) "\x04\x80\x00\x00\x2c\x80\x00\x02\xab\x80\x00\x00\x00\x80\x00\x00\x00",
        .size = 17,
        .offset = 0};
    assert_int_equal(account_parse_and_check_bip32_path(&cdata_1, bip32_path, &bip32_path_len), 0);

    buffer_t cdata_2 = {
        .ptr  = (uint8_t *) "\x04\x80\x00\x00\x2c\x80\x00\x02\xab\x80\x00\x00\x00\x80\x00\x00\x00",
        .size = 18,
        .offset = 18};
    assert_int_equal(account_parse_and_check_bip32_path(&cdata_2, bip32_path, &bip32_path_len), -1);

    buffer_t cdata_3 = {
        .ptr  = (uint8_t *) "\x04\x80\x00\x00\x2c\x80\x00\x02\xab\x80\x00\x00\x00",
        .size = 13,
        .offset = 0};
    assert_int_equal(account_parse_and_check_bip32_path(&cdata_3, bip32_path, &bip32_path_len), -1);

    buffer_t cdata_4 = {
        .ptr  = (uint8_t *) "\x05\x80\x00\x00\x2c\x80\x00\x02\xab\x80\x00\x00\x00\x80\x00\x00\x00\x80\x00\x00\x00",
        .size = 21,
        .offset = 0};
    assert_int_equal(account_parse_and_check_bip32_path(&cdata_4, bip32_path, &bip32_path_len), -1);

    buffer_t cdata_5 = {
        .ptr  = (uint8_t *) "\x04\x80\x00\x00\x2d\x80\x00\x02\xab\x80\x00\x00\x00\x80\x00\x00\x00",
        .size = 17,
        .offset = 0};
    assert_int_equal(account_parse_and_check_bip32_path(&cdata_5, bip32_path, &bip32_path_len), -1);

    buffer_t cdata_6 = {
        .ptr  = (uint8_t *) "\x04\x80\x00\x00\x2c\x80\x00\x02\xac\x80\x00\x00\x00\x80\x00\x00\x00",
        .size = 17,
        .offset = 0};
    assert_int_equal(account_parse_and_check_bip32_path(&cdata_6, bip32_path, &bip32_path_len), -1);

    buffer_t cdata_7 = {
        .ptr  = (uint8_t *) "\x04\x80\x00\x00\x2c\x80\x00\x02\xab\x80\x00\x00\x00\x80\x00\x00\x01",
        .size = 17,
        .offset = 0};
    assert_int_equal(account_parse_and_check_bip32_path(&cdata_7, bip32_path, &bip32_path_len), -1);

    buffer_t cdata_8 = {
        .ptr  = (uint8_t *) "\x04\x80\x00\x00\x2c\x80\x00\x02\xab\x00\x00\x00\x01\x80\x00\x00\x00",
        .size = 17,
        .offset = 0};
    assert_int_equal(account_parse_and_check_bip32_path(&cdata_8, bip32_path, &bip32_path_len), -1);
}

static void test_r_list(void **state)
{
    (void) state;

    uint32_t path[4]     = {0x8000002c, 0x800002ab, 0x80000000, 0x80000000};
    uint8_t  bn_seed[32] = {0xff, 0xc3, 0xde, 0x3c, 0x85, 0x23, 0x3e, 0x2c, 0xca, 0x13, 0x90,
                            0xdb, 0xdd, 0x6b, 0x6f, 0x04, 0x5b, 0x6a, 0x74, 0xfa, 0xde, 0x6e,
                            0x58, 0x07, 0xd2, 0xe3, 0x15, 0x27, 0x05, 0xea, 0x65, 0x7e};

    uint8_t random_bn[BN_LENGTH]
        = {0x93, 0x83, 0xb2, 0x70, 0x2a, 0x29, 0x2d, 0x0f, 0x5a, 0x3c, 0x2c,
           0xa6, 0xcf, 0x9c, 0x53, 0x18, 0xc6, 0x54, 0xe4, 0xd1, 0x5a, 0x65,
           0x4c, 0x32, 0x74, 0x78, 0xf4, 0x2e, 0x2d, 0x65, 0x4a, 0x56};

    will_return_always(cx_bn_lock, CX_OK);
    will_return_always(cx_ecpoint_alloc, CX_OK);
    will_return_always(cx_ecpoint_init, CX_OK);
    will_return_always(cx_ecpoint_rnd_scalarmul, CX_OK);
    will_return_always(cx_ecpoint_export, CX_OK);
    will_return_always(cx_ecpoint_destroy, CX_OK);
    will_return_always(cx_bn_unlock, CX_OK);
    will_return(sys_hdkey_derive, bn_seed);
    will_return(sys_hdkey_derive, SWO_OK);
    assert_int_equal(account_generate_keys(path, 4, &G_context.account), 0);

    // r_list_set
    r_list_erase();
    assert_int_equal(r_list_set(&G_context.account, 1), -1);

    r_list_erase();
    prepare_random_ok(random_bn);
    assert_int_equal(r_list_set(&G_context.account, 0), 0);
    assert_int_equal(G_context.r_list.index, 0);
    assert_int_equal(G_context.r_list.count, 1);
    scalar_t r_0 = {
        .big.u64 = {0x6e545a11bc879b13, 0x59a25b6387b3de05, 0x836534e4f20da9a9, 0x2ed6f017494bea8}
    };
    check_scalar(&G_context.r_list.array[0], &r_0);

    prepare_random_ok(random_bn);
    assert_int_equal(r_list_set(&G_context.account, 1), 0);
    assert_int_equal(G_context.r_list.index, 0);
    assert_int_equal(G_context.r_list.count, 2);
    scalar_t r_1 = {
        .big.u64 = {0x23707f160447f275, 0x2e2f1b73603788de, 0x24fcf7e0c6e0a9a9, 0x10e68c0dc743b19}
    };
    check_scalar(&G_context.r_list.array[1], &r_1);

    assert_int_equal(r_list_set(&G_context.account, 1), -1);

    assert_int_equal(r_list_set(&G_context.account, R_LIST_MAX_LENGTH), -1);

    G_context.r_list.index = 1;
    assert_int_equal(r_list_set(&G_context.account, 1), -1);

    will_return(cx_bn_alloc_init, -1);
    assert_int_equal(r_list_set(&G_context.account, 0), -1);

    prepare_random_ok(random_bn);
    assert_int_equal(r_list_set(&G_context.account, 0), 0);
    assert_int_equal(G_context.r_list.index, 0);
    assert_int_equal(G_context.r_list.count, 1);
    check_scalar(&G_context.r_list.array[0], &r_0);
    check_scalar(&G_context.r_list.array[1], (scalar_t *) &SCALAR_ZERO);

    // r_list_get
    r_list_erase();
    prepare_random_ok(random_bn);
    assert_int_equal(r_list_set(&G_context.account, 0), 0);
    prepare_random_ok(random_bn);
    assert_int_equal(r_list_set(&G_context.account, 1), 0);

    scalar_t r;
    assert_int_equal(r_list_get(0, &r, false), 0);
    check_scalar(&r, &r_0);
    assert_int_equal(r_list_get(1, &r, false), 0);
    check_scalar(&r, &r_1);
    assert_int_equal(r_list_get(R_LIST_MAX_LENGTH, &r, false), -1);
    assert_int_equal(r_list_get(2, &r, false), -1);
    assert_int_equal(r_list_get(1, &r, true), 0);
    check_scalar(&r, &r_1);
    assert_int_equal(r_list_get(1, &r, true), -1);

    // r_list_get_tvk
    r_list_erase();
    prepare_random_ok(random_bn);
    assert_int_equal(r_list_set(&G_context.account, 0), 0);
    prepare_random_ok(random_bn);
    assert_int_equal(r_list_set(&G_context.account, 1), 0);

    field_t tvk;
    assert_int_equal(r_list_get_tvk(&G_context.account, R_LIST_MAX_LENGTH, &tvk), -1);

    field_t tvk_0 = {
        .big.u64
        = {0xc27b9f8c4d99f7c4, 0xf448a69122e3080f, 0xcd33e6190da2c413, 0x1259b4ad2a3c4469}
    };
    assert_int_equal(r_list_get_tvk(&G_context.account, 0, &tvk), 0);

    // r_list_erase
    r_list_t r_list_empty = {0};
    r_list_erase();
    assert_memory_equal(&G_context.r_list, &r_list_empty, sizeof(r_list_t));

    check_field(&tvk, &tvk_0);
}

int main()
{
    const struct CMUnitTest tests[]
        = {cmocka_unit_test(test_account), cmocka_unit_test(test_r_list)};

    return cmocka_run_group_tests(tests, NULL, NULL);
}
