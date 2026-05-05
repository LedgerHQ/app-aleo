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
                    .program_id           = "ldgbatcher_p28.aleo",
                    .program_id_length    = 19,
                    .function_name        = "transfer_private_2",
                    .function_name_length = 18},
     .hash
        = {{.big.u64
            = {0x711bd2f0fd3b8860, 0x2140898228c34911, 0x0aae5605d3b999fe, 0x0487b38e575e54a4}},
           {.big.u64
            = {0x94fe7832f23f69f8, 0x79ed7c0a94a507b8, 0x19375744cd4d66cf, 0x0a0d8242d888337b}}},
     },
    {
     .status  = 0,
     .request = {.network_id           = 0,
                    .program_id           = "ldgbatcher_p28.aleo",
                    .program_id_length    = 19,
                    .function_name        = "transfer_private_3",
                    .function_name_length = 18},
     .hash
        = {{.big.u64
            = {0xc52d556215ca8d42, 0xdd6855255a8999fc, 0x96a0067debdae976, 0x05457b2b07ff2258}},
           {.big.u64
            = {0xaa089e65aad01997, 0x3c449656a3f1cfb6, 0x034a6d7ada794cc6, 0x0e0823456dc1865e}}},
     },
    {
     .status  = 0,
     .request = {.network_id           = 0,
                    .program_id           = "ldgbatcher_p28.aleo",
                    .program_id_length    = 19,
                    .function_name        = "transfer_private_4",
                    .function_name_length = 18},
     .hash
        = {{.big.u64
            = {0x5a549bc57d7ed08e, 0x57081afbde550716, 0xfd460bb66b8552f7, 0x0d98770ff6216767}},
           {.big.u64
            = {0x6bcc11ae56749122, 0x260e2b57d5d5fbef, 0xce5371258b939c59, 0x0884f29ecd7a4f1f}}},
     },
    {
     .status  = 0,
     .request = {.network_id           = 0,
                    .program_id           = "ldgbatcher_p28.aleo",
                    .program_id_length    = 19,
                    .function_name        = "transfer_private_5",
                    .function_name_length = 18},
     .hash
        = {{.big.u64
            = {0x224995cd589d1640, 0x6373aaa70a095470, 0xdf9bc58530309041, 0x01f320b522e45a10}},
           {.big.u64
            = {0xfec2e5eb27e81ed1, 0x30342feb79e7b799, 0x895988e43a223100, 0x0d77f3954dc2bd3a}}},
     },
    {
     .status  = 0,
     .request = {.network_id           = 0,
                    .program_id           = "ldgbatcher_p28.aleo",
                    .program_id_length    = 19,
                    .function_name        = "transfer_private_6",
                    .function_name_length = 18},
     .hash
        = {{.big.u64
            = {0xbc81a96cea70096d, 0x8b056fb89161bad6, 0x63cacb85d316cadb, 0x0e3562712c147531}},
           {.big.u64
            = {0xdd5ec7c602f5a9db, 0xc846d46174e69814, 0xcb0457c70b198957, 0x1178e2cfd2c4f6de}}},
     },
    {
     .status  = 0,
     .request = {.network_id           = 0,
                    .program_id           = "ldgbatcher_p28.aleo",
                    .program_id_length    = 19,
                    .function_name        = "transfer_private_7",
                    .function_name_length = 18},
     .hash
        = {{.big.u64
            = {0x433f8bef1e650481, 0x10d600f6d41e3197, 0xe862611390273369, 0x0158dab93d6cbea6}},
           {.big.u64
            = {0x080a8386e49889ef, 0xb896be0d0173b849, 0x74fe7461b27002f1, 0x0002580b37e0e04f}}},
     },
    {
     .status  = 0,
     .request = {.network_id           = 0,
                    .program_id           = "ldgbatcher_p28.aleo",
                    .program_id_length    = 19,
                    .function_name        = "transfer_private_8",
                    .function_name_length = 18},
     .hash
        = {{.big.u64
            = {0x738c7d8d9546e4c7, 0xb3a42c93014c13b5, 0xa7bfbaa1fdbfaedf, 0x0cdd3604cfb57700}},
           {.big.u64
            = {0x963d210f3ca0a9c6, 0xdef5b86772cb88ae, 0xc937f1616d1cabbb, 0x0050e835094c6a46}}},
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
		print_message("index %d\n", index);
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
