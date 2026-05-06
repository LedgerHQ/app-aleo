#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <cmocka.h>

#include "os.h"
#include "os_io.h"
#include "globals.h"

#include "send_response.h"

global_ctx_t G_context;

int io_send_response_buffers(const buffer_t *rdatalist, size_t count, uint16_t sw)
{
    uint8_t *mocked_ptr   = mock_type(uint8_t *);
    size_t   mocked_size  = mock_type(size_t);
    size_t   mocked_count = mock_type(size_t);
    uint16_t mocked_sw    = mock_type(uint16_t);

    assert_int_equal(count, mocked_count);
    assert_int_equal(rdatalist[0].size, mocked_size);
    assert_memory_equal(rdatalist[0].ptr, mocked_ptr, 32);
    assert_int_equal(sw, mocked_sw);

    return 0;
}

int io_legacy_apdu_tx(const unsigned char *buffer, unsigned short length)
{
    uint8_t *mocked_ptr    = mock_type(uint8_t *);
    size_t   mocked_length = mock_type(size_t);

    assert_int_equal(length, mocked_length);
    assert_memory_equal(buffer, mocked_ptr, 32);

    return 0;
}

static void test_send_response(void **state)
{
    (void) state;
    // helper_send_response_get_address

    const char *address = "aleo1c8hk0s7zfrmg99e4t0ekd9mgt5qgtz2yvfjamfgujndy959srufqqawxzz";
    const char *addr_apdu
        = "\x3f\x61\x6c\x65\x6f\x31\x63\x38\x68\x6b\x30\x73\x37\x7a\x66\x72\x6d\x67\x39\x39\x65\x34"
          "\x74\x30\x65\x6b\x64\x39\x6d\x67\x74\x35\x71\x67\x74\x7a\x32\x79\x76\x66\x6a\x61\x6d\x66"
          "\x67\x75\x6a\x6e\x64\x79\x39\x35\x39\x73\x72\x75\x66\x71\x71\x61\x77\x78\x7a\x7a";

    memcpy(G_context.address, address, sizeof(G_context.address));
    will_return(io_send_response_buffers, addr_apdu);
    will_return(io_send_response_buffers, 64);
    will_return(io_send_response_buffers, 1);
    will_return(io_send_response_buffers, 0x9000);
    assert_int_equal(helper_send_response_get_address(), 0);

    // helper_send_response_get_view_key
    const char *view_key = "AViewKey1hyAypRRpyq2JDjv3UpdRZdQjHrWyfPLbKf74QjgiQcjC";
    const char *view_key_apdu
        = "\x35\x41\x56\x69\x65\x77\x4b\x65\x79\x31\x68\x79\x41\x79\x70\x52\x52\x70\x79\x71\x32\x4a"
          "\x44\x6a\x76\x33\x55\x70\x64\x52\x5a\x64\x51\x6a\x48\x72\x57\x79\x66\x50\x4c\x62\x4b\x66"
          "\x37\x34\x51\x6a\x67\x69\x51\x63\x6a\x43";
    memcpy(G_context.view_key, view_key, sizeof(G_context.view_key));
    will_return(io_send_response_buffers, view_key_apdu);
    will_return(io_send_response_buffers, 54);
    will_return(io_send_response_buffers, 1);
    will_return(io_send_response_buffers, 0x9000);
    assert_int_equal(helper_send_response_get_view_key(), 0);

    // helper_send_response_sign_transaction
    group_t pk_sig = {
        .x.big.u64
        = {0xde8cc5ec217ad253, 0xea6bd6b4a4d0cf4b, 0x9556a907cd59336b, 0x0a3d18918155940e},
        .y.big.u64
        = {0xe456322861acf6ac, 0x68b72f3d5529218e, 0x3cbe762b292509f8, 0x0c03f4d9376b27cd}
    };
    memcpy(&G_context.account.compute_key.pk_sig, &pk_sig, sizeof(group_t));

    group_t pr_sig = {
        .x.big.u64
        = {0x773d1f206139cff5, 0xef1122c53bf34151, 0x58c4735db29ca6a7, 0x11d1c7a9b23a52f5},
        .y.big.u64
        = {0x799be5fcf4f15c35, 0x674847c970493e9b, 0xe1cb4589e5b4c7f3, 0x01c2c9083e4798e1}
    };
    memcpy(&G_context.account.compute_key.pr_sig, &pr_sig, sizeof(group_t));

    field_t tvk = {
        .big.u64 = {0xfcd99dd33fd711b9, 0x3085372523a8663b, 0x69091782d0a1eb9e, 0x56f3af574b36c6a}
    };
    memcpy(&G_context.sign_transaction_datas.prepared_request.tvk, &tvk, sizeof(field_t));
    group_t tpk = {
        .x.big.u64
        = {0xe8b1b5b13b95e77d, 0xe55190c120afffca, 0x903d7a482cacfa01, 0xb25f7526348007b},
        .y.big.u64
        = {0x9cf6379fb4164e10, 0x2c579edeb53e4c5e, 0xe937cc4854754624, 0xbcaeaa8087b3b4b}
    };
    memcpy(&G_context.sign_transaction_datas.prepared_request.tpk, &tpk, sizeof(group_t));
    scalar_t challenge = {
        .big.u64 = {0x9d0ace96da867be7, 0x4f5f4046389340fd, 0xe5b4a81b8695d703, 0x2a9c2673e0e1a65}
    };
    memcpy(
        &G_context.sign_transaction_datas.prepared_request.challenge, &challenge, sizeof(scalar_t));
    scalar_t response = {
        .big.u64 = {0x8ebf6f8be810664d, 0x44db9cdd3d5ef607, 0x7a0ecea96c1f29ef, 0x18b8866791e3045}
    };
    memcpy(
        &G_context.sign_transaction_datas.prepared_request.response, &response, sizeof(scalar_t));
    G_context.sign_transaction_datas.prepared_request.gammas_count = 1;
    group_t gamma                                                  = {
                                                         .x.big.u64
        = {0x3e0f47ba54ff2c20, 0xa4b263d02d37731f, 0xf6c650f0a280e050, 0x11b7382b66742e4d},
                                                         .y.big.u64
        = {0x387cfa1a55b496b7, 0x9de2032207dc5348, 0xbec120f8b0c0d11e, 0x13f7d479fee498d }
    };
    memcpy(&G_context.sign_transaction_datas.prepared_request.gammas[0], &gamma, sizeof(group_t));

    char *sign_apdu
        = "\x01\x01\x2a\x02\x01\x01\x15\x80\xaf\x14\x3c\xaa\x2d\x94\x1d\x20\x63\xa3\x47\x0f\x08\x55"
          "\x28\x2c\x70\x83\x63\x6a\xe0\xc6\xd4\xeb\x00\x8b\xaa\x01\x48\x51\x46\x01\x75\xde\x46\xd9"
          "\xac\xc5\x21\xf5\xe4\x6e\x05\xad\x9f\xc5\x1a\x6e\xa4\x83\x92\x1b\x86\xa9\x22\xe5\xf6\x67"
          "\x5f\x9e\xc5\xfe\xc0\x03\xee\x7c\xf7\xcd\xc0\xc5\x0b\x09\x7c\x27\x2a\x5a\x99\xd4\x8d\xff"
          "\x11\x7a\x7f\x9a\x2e\x6c\x55\x8c\x8f\x50\x63\xe5\x58\xb4\x51\x0a\x59\x61\x9b\x14\xb7\xae"
          "\xc3\xf1\x3b\x5e\x0d\x89\x14\xd1\xef\x81\x85\xe0\x82\x25\x46\x2e\x7a\xaa\x3d\xd0\x7e\xcf"
          "\x39\x09\x41\x01\x81\xbf\x20\xda\x7a\x28\xb6\x78\xa5\x07\x38\x57\xf4\x2b\x4a\x32\x27\xdd"
          "\xb3\x37\xcd\x29\x3c\x6b\xc0\x3b\x6e\xb5\x6b\xc8\xeb\xde\x83\x38\x11\x81\xc0\x20\x57\x2b"
          "\xc6\x10\xaa\x5b\x90\x53\xc4\xee\x92\xf8\xf8\x3f\xc5\xbb\x27\xd4\x53\x47\xfd\xf4\x77\xc6"
          "\x8a\xa5\x88\x94\xf6\x43\x36\x0e\x81\xc1\x01\x01\x81\xc2\x20\x70\xb0\x93\x4e\x7b\xb6\xd1"
          "\xee\xaf\x44\x18\x01\x89\x1e\xb7\xec\x72\xb5\x68\xb1\xd6\x20\x9f\x57\xb0\x2a\x2a\x21\x42"
          "\x7c\x7c\x12\x90\x00";
    will_return(io_legacy_apdu_tx, sign_apdu);
    will_return(io_legacy_apdu_tx, 247);
    assert_int_equal(helper_send_response_sign_transaction(), 0);
}

int main()
{
    const struct CMUnitTest tests[] = {cmocka_unit_test(test_send_response)};

    return cmocka_run_group_tests(tests, NULL, NULL);
}