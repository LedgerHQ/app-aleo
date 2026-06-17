#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <cmocka.h>

#include "os.h"
#include "account.h"
#include "signature.h"

global_ctx_t G_context;

static void prepare_random_ok(uint8_t *random_bn)
{
    will_return(cx_bn_lock, CX_OK);
    will_return(cx_bn_alloc_init, CX_OK);
    will_return(cx_bn_alloc, CX_OK);
    will_return(cx_bn_rng, CX_OK);
    will_return(cx_bn_export, random_bn);
    will_return(cx_bn_export, CX_OK);
    will_return(cx_bn_unlock, CX_OK);
}

static void prepare_scalar_mult_ok(void)
{
    will_return(cx_bn_lock, CX_OK);
    will_return(cx_ecpoint_alloc, CX_OK);
    will_return(cx_ecpoint_init, CX_OK);
    will_return(cx_ecpoint_scalarmul, CX_OK);
    will_return(cx_ecpoint_export, CX_OK);
    will_return(cx_ecpoint_destroy, CX_OK);
    will_return(cx_bn_unlock, CX_OK);
}

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

static void test_signature(void **state)
{
    (void) state;

    uint32_t path[4]            = {0x8000002c, 0x800002ab, 0x80000000, 0x80000000};
    uint8_t  bn_seed[BN_LENGTH] = {0xff, 0xc3, 0xde, 0x3c, 0x85, 0x23, 0x3e, 0x2c, 0xca, 0x13, 0x90,
                                   0xdb, 0xdd, 0x6b, 0x6f, 0x04, 0x5b, 0x6a, 0x74, 0xfa, 0xde, 0x6e,
                                   0x58, 0x07, 0xd2, 0xe3, 0x15, 0x27, 0x05, 0xea, 0x65, 0x7e};

    will_return(sys_hdkey_derive, bn_seed);
    will_return(sys_hdkey_derive, SWO_OK);
    will_return_count(cx_bn_lock, CX_OK, 3);
    will_return_count(cx_ecpoint_alloc, CX_OK, 3);
    will_return_count(cx_ecpoint_init, CX_OK, 3);
    will_return_count(cx_ecpoint_scalarmul, CX_OK, 3);
    will_return_count(cx_ecpoint_export, CX_OK, 3);
    will_return_count(cx_ecpoint_destroy, CX_OK, 3);
    will_return_count(cx_bn_unlock, CX_OK, 3);
    assert_int_equal(account_generate_keys(path, 4, &G_context.account), 0);

    uint8_t random_bn[BN_LENGTH]
        = {0x93, 0x83, 0xb2, 0x70, 0x2a, 0x29, 0x2d, 0x0f, 0x5a, 0x3c, 0x2c,
           0xa6, 0xcf, 0x9c, 0x53, 0x18, 0xc6, 0x54, 0xe4, 0xd1, 0x5a, 0x65,
           0x4c, 0x32, 0x74, 0x78, 0xf4, 0x2e, 0x2d, 0x65, 0x4a, 0x56};

    prepared_request_t request = {
        .is_root              = true,
        .network_id           = 1,
        .program_id_length    = 12,
        .program_id           = "credits.aleo",
        .function_name_length = 15,
        .function_name        = "transfer_public",
        .inputs_count         = 2,
        .inputs
        = {{.value_length = 32,
            .value = (uint8_t *) "\x82\x48\xd5\xe8\x5a\xc4\xc1\x23\x46\xf8\x45\x8b\xd9\x39\xf1\xce"
                                 "\x25\xae\x03\xe9\xc6\xcb\xc8\x86\x28\x6d\xf1\x61\x63\x0a\x75\x0c",
            .type_length = 3,
            .type        = (uint8_t *) "\x01\x00\x00"},
           {.value_length = 8,
            .value        = (uint8_t *) "\xe8\x03\x00\x00\x00\x00\x00\x00",
            .type_length  = 3,
            .type         = (uint8_t *) "\x01\x00\x0c"}},
        .program_checksum  = NULL,
        .nested_call_count = 0,
    };

    // sign_prepared_request
    request.program_id_length = 1;
    prepare_random_ok(random_bn);
    prepare_scalar_mult_ok();
    prepare_scalar_mult_ok();
    assert_int_equal(sign_prepared_request(&G_context.account, &request), -1);
    request.program_id_length = 12;

    request.function_name_length = 80;
    prepare_random_ok(random_bn);
    prepare_scalar_mult_ok();
    prepare_scalar_mult_ok();
    assert_int_equal(sign_prepared_request(&G_context.account, &request), -1);
    request.function_name_length = 15;

    will_return(cx_bn_lock, -1);
    assert_int_equal(sign_prepared_request(&G_context.account, &request), -1);

    prepare_random_ok(random_bn);
    will_return(cx_bn_lock, -1);
    assert_int_equal(sign_prepared_request(&G_context.account, &request), -1);

    prepare_random_ok(random_bn);
    prepare_scalar_mult_ok();
    will_return(cx_bn_lock, -1);
    assert_int_equal(sign_prepared_request(&G_context.account, &request), -1);

    request.program_id
        = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz.aleo";
    request.program_id_length = 83;
    prepare_random_ok(random_bn);
    prepare_scalar_mult_ok();
    prepare_scalar_mult_ok();
    assert_int_equal(sign_prepared_request(&G_context.account, &request), -1);
    request.program_id        = "credits.aleo";
    request.program_id_length = 12;

    request.program_id
        = "credits.abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";
    request.program_id_length = 86;
    prepare_random_ok(random_bn);
    prepare_scalar_mult_ok();
    prepare_scalar_mult_ok();
    assert_int_equal(sign_prepared_request(&G_context.account, &request), -1);
    request.program_id        = "credits.aleo";
    request.program_id_length = 12;

    request.inputs[0].type = (uint8_t *) "\xff\x00\x00";
    prepare_random_ok(random_bn);
    prepare_scalar_mult_ok();
    prepare_scalar_mult_ok();
    assert_int_equal(sign_prepared_request(&G_context.account, &request), -1);
    request.inputs[0].type = (uint8_t *) "\x01\x00\x00";

    request.inputs[0].type = (uint8_t *) "\x00\x00\x00";
    prepare_random_ok(random_bn);
    prepare_scalar_mult_ok();
    prepare_scalar_mult_ok();
    assert_int_equal(sign_prepared_request(&G_context.account, &request), -1);
    request.inputs[0].type = (uint8_t *) "\x01\x00\x00";

    request.inputs[0].type = (uint8_t *) "\x01\x08\x00";
    prepare_random_ok(random_bn);
    prepare_scalar_mult_ok();
    prepare_scalar_mult_ok();
    assert_int_equal(sign_prepared_request(&G_context.account, &request), -1);
    request.inputs[0].type = (uint8_t *) "\x01\x00\x00";

    request.inputs[0].type = (uint8_t *) "\x01\x00\x09";
    prepare_random_ok(random_bn);
    prepare_scalar_mult_ok();
    prepare_scalar_mult_ok();
    assert_int_equal(sign_prepared_request(&G_context.account, &request), -1);
    request.inputs[0].type = (uint8_t *) "\x01\x00\x00";

    scalar_t r_1 = {
        .big.u64 = {0xd12073752c196d6d, 0x6f96db151559f1ff, 0x25a34b586f7f9723, 0x2697302b89ed397}
    };
    field_t tvk_1 = {
        .big.u64 = {0xfcd99dd33fd711b9, 0x3085372523a8663b, 0x69091782d0a1eb9e, 0x56f3af574b36c6a}
    };
    group_t tpk_1 = {
        .x.big.u64
        = {0xe8b1b5b13b95e77d, 0xe55190c120afffca, 0x903d7a482cacfa01, 0xb25f7526348007b},
        .y.big.u64
        = {0x9cf6379fb4164e10, 0x2c579edeb53e4c5e, 0xe937cc4854754624, 0xbcaeaa8087b3b4b}
    };
    field_t tcm_1 = {
        .big.u64 = {0x25910ef54db490ed, 0x1748ffd6736145e7, 0x288b9a16ca91d094, 0x220b2a6efe46d2c}
    };
    scalar_t challenge_1 = {
        .big.u64 = {0x635dc69a9f606382, 0x80ff880588658b7b, 0x5237558ee4d759c1, 0x32b801808f3d29a}
    };
    scalar_t response_1 = {
        .big.u64 = {0xa302f8c4e97b2cb7, 0xc41ec07b5815e53f, 0x168ca6d7bdc9365e, 0x3a6f4913f6a850c}
    };

    prepare_random_ok(random_bn);
    prepare_scalar_mult_ok();
    prepare_scalar_mult_ok();
    assert_int_equal(sign_prepared_request(&G_context.account, &request), 0);
    assert_int_equal(request.gammas_count, 0);
    check_scalar(&request.r, &r_1);
    check_field(&request.tvk, &tvk_1);
    check_group(&request.tpk, &tpk_1);
    check_field(&request.tcm, &tcm_1);
    check_scalar(&request.challenge, &challenge_1);
    check_scalar(&request.response, &response_1);

    scalar_t challenge_2 = {
        .big.u64 = {0x86c48867d9c17da2, 0xc85a1cfc35cfa6eb, 0x7b40835b5f215b13, 0x167e30d287ef76a}
    };
    scalar_t response_2 = {
        .big.u64 = {0x1821a9a8969fd50d, 0x25a0a26f70d168f0, 0x4cd4d6d76b60ce3e, 0x45f6663ec4bbcce}
    };
    request.is_root = false;
    prepare_random_ok(random_bn);
    prepare_scalar_mult_ok();
    prepare_scalar_mult_ok();
    assert_int_equal(sign_prepared_request(&G_context.account, &request), 0);
    assert_int_equal(request.gammas_count, 0);
    check_scalar(&request.r, &r_1);
    check_field(&request.tvk, &tvk_1);
    check_group(&request.tpk, &tpk_1);
    check_field(&request.tcm, &tcm_1);
    check_scalar(&request.challenge, &challenge_2);
    check_scalar(&request.response, &response_2);
    request.is_root = true;

    scalar_t challenge_3 = {
        .big.u64 = {0xc42164ae308502c4, 0xe09092339e988b97, 0xfb5601e3e62a8271, 0x14e6cb4b521e410}
    };
    scalar_t response_3 = {
        .big.u64 = {0xc5584185e831c852, 0x1ef361fc1c6165be, 0xd750ea8478297502, 0x42f1b7c6ea2571c}
    };
    uint8_t program_checksum_c[32]
        = "\x82\x48\xd5\xe8\x5a\xc4\xc1\x23\x46\xf8\x45\x8b\xd9\x39\xf1\xce\x25\xae\x03\xe9\xc6\xcb"
          "\xc8\x86\x28\x6d\xf1\x61\x63\x0a\x75\x0c";
    uint8_t program_checksum[32];
    memcpy(program_checksum, program_checksum_c, 32);

    request.program_checksum = program_checksum;
    prepare_random_ok(random_bn);
    prepare_scalar_mult_ok();
    prepare_scalar_mult_ok();
    assert_int_equal(sign_prepared_request(&G_context.account, &request), 0);
    assert_int_equal(request.gammas_count, 0);
    check_scalar(&request.r, &r_1);
    check_field(&request.tvk, &tvk_1);
    check_group(&request.tpk, &tpk_1);
    check_field(&request.tcm, &tcm_1);
    check_scalar(&request.challenge, &challenge_3);
    check_scalar(&request.response, &response_3);
    request.program_checksum = NULL;

    const uint8_t hash_record_c[96]
        = "\xf4\x69\x19\x61\x50\x7b\x8f\x32\x92\xaf\x47\xac\x64\xdf\x59\xf7"
          "\xc4\x39\xf6\xb2\x48\xa9\x55\x10\xfa\x95\xcc\x96\x25\xe7\xfd\x07"
          "\x2a\xe1\x18\xb5\x2d\x46\xd6\x0b\x96\x63\x06\x72\x73\x3d\x25\x2c"
          "\xa9\x3f\xbb\x8d\x56\xd5\x26\x1c\x0c\x4c\xbb\x8c\xf2\x92\x5d\x05"
          "\xd9\x75\x64\x3e\x43\x24\x5c\x7a\xf2\x8d\xb2\xa8\x5b\x54\x59\xb4"
          "\xb7\x7b\x38\x2a\x0c\x74\x33\x0c\x1d\x53\x7a\xa9\x22\xe1\x7c\x00";
    uint8_t            hash_record[96];
    prepared_request_t request_private = {
        .is_root              = true,
        .network_id           = 1,
        .program_id_length    = 12,
        .program_id           = "credits.aleo",
        .function_name_length = 16,
        .function_name        = "transfer_private",
        .inputs_count         = 3,
        .inputs
        = {{.value_length = 96,
            .value        = hash_record,
            .type_length  = 9,
            .type         = (uint8_t *) "\x03\x07\x63\x72\x65\x64\x69\x74\x73"},
           {.value_length = 32,
            .value = (uint8_t *) "\x82\x48\xd5\xe8\x5a\xc4\xc1\x23\x46\xf8\x45\x8b\xd9\x39\xf1\xce"
                                 "\x25\xae\x03\xe9\xc6\xcb\xc8\x86\x28\x6d\xf1\x61\x63\x0a\x75\x0c",
            .type_length = 3,
            .type        = (uint8_t *) "\x02\x00\x00"},
           {.value_length = 8,
            .value        = (uint8_t *) "\xe8\x03\x00\x00\x00\x00\x00\x00",
            .type_length  = 3,
            .type         = (uint8_t *) "\x02\x00\x0c"}},
        .program_checksum  = NULL,
        .nested_call_count = 0,
    };

    memcpy(hash_record, hash_record_c, 96);
    prepare_random_ok(random_bn);
    prepare_scalar_mult_ok();
    will_return(cx_bn_lock, -1);
    assert_int_equal(sign_prepared_request(&G_context.account, &request_private), -1);

    memcpy(hash_record, hash_record_c, 96);
    prepare_random_ok(random_bn);
    prepare_scalar_mult_ok();
    prepare_scalar_mult_ok();
    will_return(cx_bn_lock, -1);
    assert_int_equal(sign_prepared_request(&G_context.account, &request_private), -1);

    memcpy(hash_record, hash_record_c, 96);
    prepare_random_ok(random_bn);
    prepare_scalar_mult_ok();
    prepare_scalar_mult_ok();
    prepare_scalar_mult_ok();
    will_return(cx_bn_lock, -1);
    assert_int_equal(sign_prepared_request(&G_context.account, &request_private), -1);

    memcpy(hash_record, hash_record_c, 96);
    request.inputs[1].type = (uint8_t *) "\x02\x01\x00";
    prepare_random_ok(random_bn);
    prepare_scalar_mult_ok();
    prepare_scalar_mult_ok();
    assert_int_equal(sign_prepared_request(&G_context.account, &request), -1);
    request.inputs[1].type = (uint8_t *) "\x02\x00\x00";

    memcpy(hash_record, hash_record_c, 96);
    request.inputs[1].type = (uint8_t *) "\x02\x00\x09";
    prepare_random_ok(random_bn);
    prepare_scalar_mult_ok();
    prepare_scalar_mult_ok();
    assert_int_equal(sign_prepared_request(&G_context.account, &request), -1);
    request.inputs[1].type = (uint8_t *) "\x02\x00\x00";

    scalar_t r_10 = {
        .big.u64 = {0xd12073752c196d6d, 0x6f96db151559f1ff, 0x25a34b586f7f9723, 0x2697302b89ed397}
    };
    field_t tvk_10 = {
        .big.u64 = {0xfcd99dd33fd711b9, 0x3085372523a8663b, 0x69091782d0a1eb9e, 0x56f3af574b36c6a}
    };
    group_t tpk_10 = {
        .x.big.u64
        = {0xe8b1b5b13b95e77d, 0xe55190c120afffca, 0x903d7a482cacfa01, 0xb25f7526348007b},
        .y.big.u64
        = {0x9cf6379fb4164e10, 0x2c579edeb53e4c5e, 0xe937cc4854754624, 0xbcaeaa8087b3b4b}
    };
    field_t tcm_10 = {
        .big.u64 = {0x25910ef54db490ed, 0x1748ffd6736145e7, 0x288b9a16ca91d094, 0x220b2a6efe46d2c}
    };
    scalar_t challenge_10 = {
        .big.u64 = {0x9d0ace96da867be7, 0x4f5f4046389340fd, 0xe5b4a81b8695d703, 0x2a9c2673e0e1a65}
    };
    scalar_t response_10 = {
        .big.u64 = {0x8ebf6f8be810664d, 0x44db9cdd3d5ef607, 0x7a0ecea96c1f29ef, 0x18b8866791e3045}
    };
    group_t gamma_10 = {
        .x.big.u64
        = {0x3e0f47ba54ff2c20, 0xa4b263d02d37731f, 0xf6c650f0a280e050, 0x11b7382b66742e4d},
        .y.big.u64
        = {0x387cfa1a55b496b7, 0x9de2032207dc5348, 0xbec120f8b0c0d11e, 0x13f7d479fee498d }
    };

    memcpy(hash_record, hash_record_c, 96);
    request_private.gammas_count = 0;
    prepare_random_ok(random_bn);
    prepare_scalar_mult_ok();
    prepare_scalar_mult_ok();
    prepare_scalar_mult_ok();
    prepare_scalar_mult_ok();
    assert_int_equal(sign_prepared_request(&G_context.account, &request_private), 0);
    assert_int_equal(request_private.gammas_count, 1);
    check_scalar(&request_private.r, &r_10);
    check_field(&request_private.tvk, &tvk_10);
    check_group(&request_private.tpk, &tpk_10);
    check_field(&request_private.tcm, &tcm_10);
    check_scalar(&request_private.challenge, &challenge_10);
    check_scalar(&request_private.response, &response_10);
    check_group(&request_private.gammas[0], &gamma_10);

    memcpy(hash_record, hash_record_c, 96);
    request_private.gammas_count = 0;
    G_context.signing_state      = SIGNING_STATE_FEES;
    prepare_random_ok(random_bn);
    prepare_scalar_mult_ok();
    prepare_scalar_mult_ok();
    prepare_scalar_mult_ok();
    prepare_scalar_mult_ok();
    assert_int_equal(sign_prepared_request(&G_context.account, &request_private), 0);
    assert_int_equal(request_private.gammas_count, 1);
    check_scalar(&request_private.r, &r_10);
    check_field(&request_private.tvk, &tvk_10);
    check_group(&request_private.tpk, &tpk_10);
    check_field(&request_private.tcm, &tcm_10);
    check_scalar(&request_private.challenge, &challenge_10);
    check_scalar(&request_private.response, &response_10);
    check_group(&request_private.gammas[0], &gamma_10);

    memcpy(hash_record, hash_record_c, 96);
    request_private.gammas_count = 0;
    G_context.signing_state      = SIGNING_STATE_NESTED_CALL;
    prepare_random_ok(random_bn);
    prepare_scalar_mult_ok();
    prepare_scalar_mult_ok();
    prepare_scalar_mult_ok();
    prepare_scalar_mult_ok();
    assert_int_equal(sign_prepared_request(&G_context.account, &request_private), 0);
    assert_int_equal(request_private.gammas_count, 1);
    check_scalar(&request_private.r, &r_10);
    check_field(&request_private.tvk, &tvk_10);
    check_group(&request_private.tpk, &tpk_10);
    check_field(&request_private.tcm, &tcm_10);
    check_scalar(&request_private.challenge, &challenge_10);
    check_scalar(&request_private.response, &response_10);
    check_group(&request_private.gammas[0], &gamma_10);

    prepared_request_t request_batch_private = {
        .is_root              = true,
        .network_id           = 1,
        .program_id_length    = 23,
        .program_id           = "ldgbatcher_ppub_28.aleo",
        .function_name_length = 28,
        .function_name        = "transfer_private_to_public_2",
        .inputs_count         = 3,
        .inputs
        = {{.value_length = 96, .value = hash_record, .type_length = 1, .type = (uint8_t *) "\x04"},
           {.value_length = 96, .value = hash_record, .type_length = 1, .type = (uint8_t *) "\x04"},
           {.value_length = 8,
            .value        = (uint8_t *) "\xe8\x03\x00\x00\x00\x00\x00\x00",
            .type_length  = 3,
            .type         = (uint8_t *) "\x01\x00\x0c"}},
        .program_checksum  = NULL,
        .nested_call_count = 2,
    };

    memcpy(hash_record, hash_record_c, 96);
    prepare_random_ok(random_bn);
    prepare_scalar_mult_ok();
    will_return(cx_bn_lock, -1);
    assert_int_equal(sign_prepared_request(&G_context.account, &request_batch_private), -1);

    memcpy(hash_record, hash_record_c, 96);
    prepare_random_ok(random_bn);
    prepare_scalar_mult_ok();
    prepare_scalar_mult_ok();
    request_batch_private.inputs[0].value_length = 95;
    assert_int_equal(sign_prepared_request(&G_context.account, &request_batch_private), -1);
    request_batch_private.inputs[0].value_length = 96;

    memcpy(hash_record, hash_record_c, 96);
    prepare_random_ok(random_bn);
    prepare_scalar_mult_ok();
    prepare_scalar_mult_ok();
    request_batch_private.inputs[0].value_length = 97;
    assert_int_equal(sign_prepared_request(&G_context.account, &request_batch_private), -1);
    request_batch_private.inputs[0].value_length = 96;

    memcpy(hash_record, hash_record_c, 96);
    prepare_random_ok(random_bn);
    prepare_scalar_mult_ok();
    prepare_scalar_mult_ok();
    request_batch_private.inputs[0].value_length = (12+26)*32;
    assert_int_equal(sign_prepared_request(&G_context.account, &request_batch_private), -1);
    request_batch_private.inputs[0].value_length = 96;

    scalar_t r_11 = {
        .big.u64 = {0xd12073752c196d6d, 0x6f96db151559f1ff, 0x25a34b586f7f9723, 0x2697302b89ed397}
    };
    field_t tvk_11 = {
        .big.u64 = {0xfcd99dd33fd711b9, 0x3085372523a8663b, 0x69091782d0a1eb9e, 0x56f3af574b36c6a}
    };
    group_t tpk_11 = {
        .x.big.u64
        = {0xe8b1b5b13b95e77d, 0xe55190c120afffca, 0x903d7a482cacfa01, 0xb25f7526348007b},
        .y.big.u64
        = {0x9cf6379fb4164e10, 0x2c579edeb53e4c5e, 0xe937cc4854754624, 0xbcaeaa8087b3b4b}
    };
    field_t tcm_11 = {
        .big.u64 = {0x25910ef54db490ed, 0x1748ffd6736145e7, 0x288b9a16ca91d094, 0x220b2a6efe46d2c}
    };
    scalar_t challenge_11 = {
        .big.u64 = {0xaf39868d3ced57ab, 0x325b5ef796728723, 0x80060bb6fd767dfb, 0x21a1e0b8703a6fc}
    };
    scalar_t response_11 = {
        .big.u64 = {0x46ab387b0d0eba28, 0xc19d9f6b26bf5346, 0x6db0d19a90511dd4, 0x12af8b54c4c3a5e}
    };

    memcpy(program_checksum, program_checksum_c, 32);
    request.program_checksum = program_checksum;
    memcpy(hash_record, hash_record_c, 96);
    request_batch_private.gammas_count = 0;
    prepare_random_ok(random_bn);
    prepare_scalar_mult_ok();
    prepare_scalar_mult_ok();
    assert_int_equal(sign_prepared_request(&G_context.account, &request_batch_private), 0);
    assert_int_equal(request_batch_private.gammas_count, 0);
    check_scalar(&request_batch_private.r, &r_11);
    check_field(&request_batch_private.tvk, &tvk_11);
    check_group(&request_batch_private.tpk, &tpk_11);
    check_field(&request_batch_private.tcm, &tcm_11);
    check_scalar(&request_batch_private.challenge, &challenge_11);
    check_scalar(&request_batch_private.response, &response_11);

    r_list_erase();
    field_t pre_tvk_0;
    field_t pre_tvk_1;

    prepare_random_ok(random_bn);
    prepare_scalar_mult_ok();
    prepare_scalar_mult_ok();
    assert_int_equal(r_list_set(&G_context.account, 0), 0);
    assert_int_equal(r_list_set(&G_context.account, 1), 0);
    assert_int_equal(r_list_get_tvk(&G_context.account, 0, &pre_tvk_0), 0);
    assert_int_equal(r_list_get_tvk(&G_context.account, 1, &pre_tvk_1), 0);

    memcpy(program_checksum, program_checksum_c, 32);
    request.program_checksum = program_checksum;
    memcpy(hash_record, hash_record_c, 96);
    request_batch_private.gammas_count = 0;
    G_context.r_list.index             = 1;
    assert_int_equal(sign_prepared_request(&G_context.account, &request_batch_private), -1);

    G_context.r_list.index = 0;
    prepare_scalar_mult_ok();
    prepare_scalar_mult_ok();
    assert_int_equal(sign_prepared_request(&G_context.account, &request_batch_private), 0);
    check_field(&request_batch_private.tvk, &pre_tvk_0);

    prepared_request_t request_join = {
        .is_root              = false,
        .network_id           = 1,
        .program_id_length    = 12,
        .program_id           = "credits.aleo",
        .function_name_length = 4,
        .function_name        = "join",
        .inputs_count         = 2,
        .inputs               = {{.value_length = 96,
                                  .value        = hash_record,
                                  .type_length  = 1,
                                  .type         = (uint8_t *) "\x03\x07\x63\x72\x65\x64\x69\x74\x73"},
                                 {.value_length = 96,
                                  .value        = hash_record,
                                  .type_length  = 1,
                                  .type         = (uint8_t *) "\x03\x07\x63\x72\x65\x64\x69\x74\x73"}},
        .program_checksum     = NULL,
        .nested_call_count    = 0,
    };

    memcpy(hash_record, hash_record_c, 96);
    request_join.gammas_count = 0;
    prepare_scalar_mult_ok();
    prepare_scalar_mult_ok();
    prepare_scalar_mult_ok();
    prepare_scalar_mult_ok();
    prepare_scalar_mult_ok();
    prepare_scalar_mult_ok();
    assert_int_equal(sign_prepared_request(&G_context.account, &request_join), 0);
    check_field(&request_join.tvk, &pre_tvk_1);

    r_list_erase();

    prepare_random_ok(random_bn);
    prepare_scalar_mult_ok();
    assert_int_equal(r_list_set(&G_context.account, 0), 0);
    assert_int_equal(r_list_get_tvk(&G_context.account, 0, &pre_tvk_0), 0);

    memcpy(program_checksum, program_checksum_c, 32);
    request.program_checksum = program_checksum;
    memcpy(hash_record, hash_record_c, 96);
    request_batch_private.gammas_count = 0;

    prepare_scalar_mult_ok();
    prepare_scalar_mult_ok();
    assert_int_equal(sign_prepared_request(&G_context.account, &request_batch_private), 0);
    check_field(&request_batch_private.tvk, &pre_tvk_0);

    memcpy(hash_record, hash_record_c, 96);
    request_join.gammas_count = 0;
    assert_int_equal(sign_prepared_request(&G_context.account, &request_join), -1);
}

int main()
{
    const struct CMUnitTest tests[] = {cmocka_unit_test(test_signature)};

    return cmocka_run_group_tests(tests, NULL, NULL);
}
