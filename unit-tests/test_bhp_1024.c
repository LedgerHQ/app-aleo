#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <cmocka.h>

#include "os.h"
#include "bhp_1024.h"

typedef struct {
    int                 status;
    function_id_datas_t datas;
    field_t             hash[2];
} bhp_test_t;

static bhp_test_t bhp_tests[] = {
    {
     .status = 0,
     .datas  = {.network_id         = 0,
                   .program_id_name    = "credits",
                   .program_id_network = "aleo",
                   .function_name      = "transfer_public"},
     .hash
        = {{.big.u64
            = {0x405245447621401a, 0x51c07b62a9c53d26, 0xb928e2f9112d0953, 0x0f511e31d6855446}},
           {.big.u64
            = {0x1a45c36fff8ff4a4, 0x69712402399b89f1, 0xb3e921a03d522403, 0x0b8deb3e586ce4f4}}},
     },
    {
     .status = 0,
     .datas  = {.network_id         = 0,
                   .program_id_name    = "credits",
                   .program_id_network = "aleo",
                   .function_name      = "transfer_private"},
     .hash
        = {{.big.u64
            = {0x57853955ec65e959, 0xc04dacceb1a2026a, 0x0b8c74f5b07ee388, 0x0913b0e1e8289dba}},
           {.big.u64
            = {0x14050f83d8bc8ad5, 0x68c1128533fd9691, 0xb5bfd0497dcbe60d, 0x061bcb7383584d42}}},
     },
    {
     .status = 0,
     .datas  = {.network_id         = 0,
                   .program_id_name    = "credits",
                   .program_id_network = "aleo",
                   .function_name      = "transfer_private_to_public"},
     .hash
        = {{.big.u64
            = {0x2ed6c22fde6f93b0, 0x7673e8d503d0995f, 0x6ec97fff81556086, 0x099abee7b8c03597}},
           {.big.u64
            = {0x7c27587b57c5d2d4, 0x78e193e449543290, 0xf4e07ed6a53dea1e, 0x018801c51078171d}}},
     },
    {
     .status = 0,
     .datas  = {.network_id         = 0,
                   .program_id_name    = "credits",
                   .program_id_network = "aleo",
                   .function_name      = "transfer_public_to_private"},
     .hash
        = {{.big.u64
            = {0x6e2c39b3c306d0f5, 0x1dc66c4befdfbc77, 0x0e2106ac3953c377, 0x0d3c811de289fd10}},
           {.big.u64
            = {0x116b3d6259fc8332, 0x3b6f6a1a5b3344dd, 0x66c3bccc5ed9580b, 0x0b218600c10bd9a3}}},
     },
    {
     .status = 0,
     .datas  = {.network_id         = 0,
                   .program_id_name    = "credits",
                   .program_id_network = "aleo",
                   .function_name      = "fee_public"},
     .hash
        = {{.big.u64
            = {0xf9bcb7f7a577a934, 0x98f4346a10f70cf8, 0x8f26408e9addc51b, 0x0aa82c82d4ff15a4}},
           {.big.u64
            = {0x8d31084295c605ef, 0xbfdc39996575504e, 0xae32c9cce746275e, 0x0bbfb3251558eee9}}},
     },
    {
     .status = 0,
     .datas  = {.network_id         = 0,
                   .program_id_name    = "credits",
                   .program_id_network = "aleo",
                   .function_name      = "fee_private"},
     .hash
        = {{.big.u64
            = {0xccf5dc9907364acc, 0x1cf2578ad1d69649, 0x95c4227bca877a48, 0x0c218239cc930255}},
           {.big.u64
            = {0xc3c204d98b5e12b2, 0x318d93a9b12ba7c9, 0x9d42d8fb1c715281, 0x01505646987444fa}}},
     },
    {
     .status = 0,
     .datas  = {.network_id         = 0,
                   .program_id_name    = "credits",
                   .program_id_network = "aleo",
                   .function_name      = "split"},
     .hash
        = {{.big.u64
            = {0x842cb43ec1d6bbe9, 0xa2c33251d4bded16, 0x0fe74f79fc7c63d4, 0x022fb51a7d2acc90}},
           {.big.u64
            = {0x0bb53e032f965178, 0x6cd697d8b5df7efc, 0x12e2736c492f2495, 0x0529c62123003bca}}},
     },

 // failure
    {
     .status = -1,
     .datas  = {.network_id         = 0,
                   .program_id_name    = "null",
                   .program_id_network = "null",
                   .function_name      = "null"},
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

    field_t hash;

    int index = 0;
    while (bhp_tests[index].status != -1111) {
        bhp_tests[index].datas.network_id = 0;
        assert_int_equal(bhp_1024_hash_function_id(&bhp_tests[index].datas, &hash),
                         bhp_tests[index].status);
        if (bhp_tests[index].status == 0) {
            check_field(&hash, &bhp_tests[index].hash[0]);
        }
        bhp_tests[index].datas.network_id = 1;
        assert_int_equal(bhp_1024_hash_function_id(&bhp_tests[index].datas, &hash),
                         bhp_tests[index].status);
        if (bhp_tests[index].status == 0) {
            check_field(&hash, &bhp_tests[index].hash[1]);
        }
        index++;
    }
}

int main()
{
    const struct CMUnitTest tests[] = {cmocka_unit_test(test_bhp_1024)};

    return cmocka_run_group_tests(tests, NULL, NULL);
}
