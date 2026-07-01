#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <cmocka.h>

#include "os.h"
#include "types.h"
#include "bhp_1024.h"

typedef struct {
    int                status;
    prepared_request_t request;
    field_t            hash[2];
} bhp_test_t;

static bhp_test_t bhp_tests[] = {
    {
     .status  = 0,
     .request = {.network_id           = 0,
                    .program_id           = "credits.aleo",
                    .program_id_length    = 12,
                    .function_name        = "transfer_public",
                    .function_name_length = 15},
     .hash
        = {{.big.u64
            = {0x405245447621401a, 0x51c07b62a9c53d26, 0xb928e2f9112d0953, 0x0f511e31d6855446}},
           {.big.u64
            = {0x1a45c36fff8ff4a4, 0x69712402399b89f1, 0xb3e921a03d522403, 0x0b8deb3e586ce4f4}}},
     },
    {
     .status  = 0,
     .request = {.network_id           = 0,
                    .program_id           = "credits.aleo",
                    .program_id_length    = 12,
                    .function_name        = "transfer_private",
                    .function_name_length = 16},
     .hash
        = {{.big.u64
            = {0x57853955ec65e959, 0xc04dacceb1a2026a, 0x0b8c74f5b07ee388, 0x0913b0e1e8289dba}},
           {.big.u64
            = {0x14050f83d8bc8ad5, 0x68c1128533fd9691, 0xb5bfd0497dcbe60d, 0x061bcb7383584d42}}},
     },
    {
     .status  = 0,
     .request = {.network_id           = 0,
                    .program_id           = "credits.aleo",
                    .program_id_length    = 12,
                    .function_name        = "transfer_private_to_public",
                    .function_name_length = 26},
     .hash
        = {{.big.u64
            = {0x2ed6c22fde6f93b0, 0x7673e8d503d0995f, 0x6ec97fff81556086, 0x099abee7b8c03597}},
           {.big.u64
            = {0x7c27587b57c5d2d4, 0x78e193e449543290, 0xf4e07ed6a53dea1e, 0x018801c51078171d}}},
     },
    {
     .status  = 0,
     .request = {.network_id           = 0,
                    .program_id           = "credits.aleo",
                    .program_id_length    = 12,
                    .function_name        = "transfer_public_to_private",
                    .function_name_length = 26},
     .hash
        = {{.big.u64
            = {0x6e2c39b3c306d0f5, 0x1dc66c4befdfbc77, 0x0e2106ac3953c377, 0x0d3c811de289fd10}},
           {.big.u64
            = {0x116b3d6259fc8332, 0x3b6f6a1a5b3344dd, 0x66c3bccc5ed9580b, 0x0b218600c10bd9a3}}},
     },
    {
     .status  = 0,
     .request = {.network_id           = 0,
                    .program_id           = "credits.aleo",
                    .program_id_length    = 12,
                    .function_name        = "fee_public",
                    .function_name_length = 10},
     .hash
        = {{.big.u64
            = {0xf9bcb7f7a577a934, 0x98f4346a10f70cf8, 0x8f26408e9addc51b, 0x0aa82c82d4ff15a4}},
           {.big.u64
            = {0x8d31084295c605ef, 0xbfdc39996575504e, 0xae32c9cce746275e, 0x0bbfb3251558eee9}}},
     },
    {
     .status  = 0,
     .request = {.network_id           = 0,
                    .program_id           = "credits.aleo",
                    .program_id_length    = 12,
                    .function_name        = "fee_private",
                    .function_name_length = 11},
     .hash
        = {{.big.u64
            = {0xccf5dc9907364acc, 0x1cf2578ad1d69649, 0x95c4227bca877a48, 0x0c218239cc930255}},
           {.big.u64
            = {0xc3c204d98b5e12b2, 0x318d93a9b12ba7c9, 0x9d42d8fb1c715281, 0x01505646987444fa}}},
     },
    {
     .status  = 0,
     .request = {.network_id           = 0,
                    .program_id           = "credits.aleo",
                    .program_id_length    = 12,
                    .function_name        = "split",
                    .function_name_length = 5},
     .hash
        = {{.big.u64
            = {0x842cb43ec1d6bbe9, 0xa2c33251d4bded16, 0x0fe74f79fc7c63d4, 0x022fb51a7d2acc90}},
           {.big.u64
            = {0x0bb53e032f965178, 0x6cd697d8b5df7efc, 0x12e2736c492f2495, 0x0529c62123003bca}}},
     },
    {
     .status  = 0,
     .request = {.network_id           = 0,
                    .program_id           = "ldg_p_28.aleo",
                    .program_id_length    = 13,
                    .function_name        = "transfer_private_2",
                    .function_name_length = 18},
     .hash
        = {{.big.u64
            = {0xaa53141c28f75d2c, 0x65b13fba609a6035, 0x7040a11cb3a3f62b, 0x05201e528f3480c4}},
           {.big.u64
            = {0x122b908e513a4ee9, 0xb1eaf2a82d86ea62, 0x213f99d902e331a6, 0x0578c3617e2651c8}}},
     },
    {
     .status  = 0,
     .request = {.network_id           = 0,
                    .program_id           = "ldg_p_28.aleo",
                    .program_id_length    = 13,
                    .function_name        = "transfer_private_3",
                    .function_name_length = 18},
     .hash
        = {{.big.u64
            = {0xc5efbffe7658eef4, 0xdc84d624b875bdbd, 0xa37465e2a36bec63, 0x04b51a50403ecfea}},
           {.big.u64
            = {0xf29d99f2c2c53871, 0x719b0165a61d8b76, 0x01849030f9becec1, 0x04bb3a79745f7a02}}},
     },
    {
     .status  = 0,
     .request = {.network_id           = 0,
                    .program_id           = "ldg_p_28.aleo",
                    .program_id_length    = 13,
                    .function_name        = "transfer_private_4",
                    .function_name_length = 18},
     .hash
        = {{.big.u64
            = {0x9cc05109092305ba, 0x7b615a532555f011, 0x8b094b3f1c0edc45, 0x0e9a68dc1b103c7a}},
           {.big.u64
            = {0xbb293f8f1b48a386, 0xb86465e5ec6ad220, 0x3bc07a6b8afbb685, 0x124550060e45f07b}}},
     },
    {
     .status  = 0,
     .request = {.network_id           = 0,
                    .program_id           = "ldg_p_28.aleo",
                    .program_id_length    = 13,
                    .function_name        = "transfer_private_5",
                    .function_name_length = 18},
     .hash
        = {{.big.u64
            = {0xe8b3ef130b97897b, 0x8e4fd93480f74e02, 0x764544d67a317dc9, 0x0017b6bd21303713}},
           {.big.u64
            = {0xc647789c16496645, 0xa7a50dfc5400cfce, 0xd006732d30108dad, 0x0a28719827978fb4}}},
     },
    {
     .status  = 0,
     .request = {.network_id           = 0,
                    .program_id           = "ldg_p_28.aleo",
                    .program_id_length    = 13,
                    .function_name        = "transfer_private_6",
                    .function_name_length = 18},
     .hash
        = {{.big.u64
            = {0x1d28cca31b2f3b52, 0x0090094678ede7c5, 0x59302b1fa066d6cb, 0x055921d2d3c08d88}},
           {.big.u64
            = {0x72c488a09596a7cb, 0xb129fed5ebb2d725, 0x4e463da58d947f1f, 0x00a20449067cf7b6}}},
     },
    {
     .status  = 0,
     .request = {.network_id           = 0,
                    .program_id           = "ldg_p_28.aleo",
                    .program_id_length    = 13,
                    .function_name        = "transfer_private_7",
                    .function_name_length = 18},
     .hash
        = {{.big.u64
            = {0x69347a906aefae3e, 0xf0aef8bff72b73aa, 0x4a20df41143a795e, 0x032729808ae8392e}},
           {.big.u64
            = {0xbe6517af6fdc663b, 0xe38a0322ed28df31, 0x29fc25306d983d43, 0x03ea993c3a2bbe2b}}},
     },
    {
     .status  = 0,
     .request = {.network_id           = 0,
                    .program_id           = "ldg_p_28.aleo",
                    .program_id_length    = 13,
                    .function_name        = "transfer_private_8",
                    .function_name_length = 18},
     .hash
        = {{.big.u64
            = {0x72d1f721ee2455e2, 0x6a2eca0622bf617a, 0x8adfa61d037e3b19, 0x119d53a85545f29d}},
           {.big.u64
            = {0x3ad6cd9ea53fbfce, 0xdf3d3dbe8a6c7f3b, 0xb077de61a52f2ee8, 0x0a2983140a27e484}}},
     },

 // failure
    {
     .status  = -1,
     .request = {.network_id           = 0,
                    .program_id           = "null",
                    .program_id_length    = 5,
                    .function_name        = "null",
                    .function_name_length = 5},
     },

 // End
    {.status = -1111}
};

static void check_field(field_t *a, field_t *b)
{
    assert_int_equal(a->big.u64[0], b->big.u64[0]);
    assert_int_equal(a->big.u64[1], b->big.u64[1]);
    assert_int_equal(a->big.u64[2], b->big.u64[2]);
    assert_int_equal(a->big.u64[3], b->big.u64[3]);
}

static void test_bhp_1024(void **state)
{
    (void) state;

    int index = 0;
    while (bhp_tests[index].status != -1111) {
        bhp_tests[index].request.network_id = 0;
        assert_int_equal(bhp_1024_hash_function_id(&bhp_tests[index].request),
                         bhp_tests[index].status);
        if (bhp_tests[index].status == 0) {
            check_field(&bhp_tests[index].request.function_id, &bhp_tests[index].hash[0]);
        }
        bhp_tests[index].request.network_id = 1;
        assert_int_equal(bhp_1024_hash_function_id(&bhp_tests[index].request),
                         bhp_tests[index].status);
        if (bhp_tests[index].status == 0) {
            check_field(&bhp_tests[index].request.function_id, &bhp_tests[index].hash[1]);
        }
        index++;
    }
}

int main()
{
    const struct CMUnitTest tests[] = {cmocka_unit_test(test_bhp_1024)};

    return cmocka_run_group_tests(tests, NULL, NULL);
}
