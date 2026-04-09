#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <cmocka.h>

#include "cx_errors.h"
#include "group.h"

static void test_group(void **state)
{
    (void) state;

    // group_add_assign
    group_t aa = {
        .x.big.u64
        = {0xc4cd9f1bb1de0944, 0xc8cb981406c4584b, 0x27c929395cf62fe7, 0x10ef9744777f9b8e},
        .y.big.u64
        = {0x648ede4d719168f6, 0xbb63fecb653312df, 0x650de15bd80de1c4, 0x0bbddcd125d67ede}
    };
    group_t bb = {
        .x.big.u64
        = {0x5ebc83b515776e99, 0xcc3967723fb5375a, 0xb2afd6566afd3d8b, 0x10bfccadcd357b19},
        .y.big.u64
        = {0x9a4256c37884b0f2, 0x66971181ec612a0b, 0x084e13fe0e5b3a60, 0x006a582e6159fc50}
    };

    will_return(cx_bn_lock, -1);
    assert_int_equal(group_add_assign(&aa, &bb), -1);

    will_return(cx_bn_lock, CX_OK);
    will_return(cx_ecpoint_alloc, -1);
    will_return(cx_bn_unlock, CX_OK);
    assert_int_equal(group_add_assign(&aa, &bb), -1);

    will_return(cx_bn_lock, CX_OK);
    will_return(cx_ecpoint_alloc, CX_OK);
    will_return(cx_ecpoint_init, -1);
    will_return(cx_bn_unlock, CX_OK);
    assert_int_equal(group_add_assign(&aa, &bb), -1);

    will_return(cx_bn_lock, CX_OK);
    will_return(cx_ecpoint_alloc, CX_OK);
    will_return(cx_ecpoint_init, CX_OK);
    will_return(cx_ecpoint_alloc, -1);
    will_return(cx_bn_unlock, CX_OK);
    assert_int_equal(group_add_assign(&aa, &bb), -1);

    will_return(cx_bn_lock, CX_OK);
    will_return(cx_ecpoint_alloc, CX_OK);
    will_return(cx_ecpoint_init, CX_OK);
    will_return(cx_ecpoint_alloc, CX_OK);
    will_return(cx_bn_unlock, CX_OK);
    will_return(cx_ecpoint_init, -1);
    assert_int_equal(group_add_assign(&aa, &bb), -1);

    will_return(cx_bn_lock, CX_OK);
    will_return(cx_ecpoint_alloc, CX_OK);
    will_return(cx_ecpoint_init, CX_OK);
    will_return(cx_ecpoint_alloc, CX_OK);
    will_return(cx_bn_unlock, CX_OK);
    will_return(cx_ecpoint_init, CX_OK);
    will_return(cx_ecpoint_alloc, -1);
    assert_int_equal(group_add_assign(&aa, &bb), -1);

    will_return(cx_bn_lock, CX_OK);
    will_return(cx_ecpoint_alloc, CX_OK);
    will_return(cx_ecpoint_init, CX_OK);
    will_return(cx_ecpoint_alloc, CX_OK);
    will_return(cx_bn_unlock, CX_OK);
    will_return(cx_ecpoint_init, CX_OK);
    will_return(cx_ecpoint_alloc, CX_OK);
    will_return(cx_ecpoint_add, -1);
    assert_int_equal(group_add_assign(&aa, &bb), -1);

    will_return(cx_bn_lock, CX_OK);
    will_return(cx_ecpoint_alloc, CX_OK);
    will_return(cx_ecpoint_init, CX_OK);
    will_return(cx_ecpoint_alloc, CX_OK);
    will_return(cx_bn_unlock, CX_OK);
    will_return(cx_ecpoint_init, CX_OK);
    will_return(cx_ecpoint_alloc, CX_OK);
    will_return(cx_ecpoint_add, CX_OK);
    will_return(cx_ecpoint_export, -1);
    assert_int_equal(group_add_assign(&aa, &bb), -1);

    will_return(cx_bn_lock, CX_OK);
    will_return(cx_ecpoint_alloc, CX_OK);
    will_return(cx_ecpoint_init, CX_OK);
    will_return(cx_ecpoint_alloc, CX_OK);
    will_return(cx_bn_unlock, CX_OK);
    will_return(cx_ecpoint_init, CX_OK);
    will_return(cx_ecpoint_alloc, CX_OK);
    will_return(cx_ecpoint_add, CX_OK);
    will_return(cx_ecpoint_export, CX_OK);
    will_return(cx_ecpoint_destroy, -1);
    assert_int_equal(group_add_assign(&aa, &bb), -1);

    will_return(cx_bn_lock, CX_OK);
    will_return(cx_ecpoint_alloc, CX_OK);
    will_return(cx_ecpoint_init, CX_OK);
    will_return(cx_ecpoint_alloc, CX_OK);
    will_return(cx_bn_unlock, CX_OK);
    will_return(cx_ecpoint_init, CX_OK);
    will_return(cx_ecpoint_alloc, CX_OK);
    will_return(cx_ecpoint_add, CX_OK);
    will_return(cx_ecpoint_export, CX_OK);
    will_return(cx_ecpoint_destroy, CX_OK);
    will_return(cx_ecpoint_destroy, -1);
    assert_int_equal(group_add_assign(&aa, &bb), -1);

    will_return(cx_bn_lock, CX_OK);
    will_return(cx_ecpoint_alloc, CX_OK);
    will_return(cx_ecpoint_init, CX_OK);
    will_return(cx_ecpoint_alloc, CX_OK);
    will_return(cx_bn_unlock, CX_OK);
    will_return(cx_ecpoint_init, CX_OK);
    will_return(cx_ecpoint_alloc, CX_OK);
    will_return(cx_ecpoint_add, CX_OK);
    will_return(cx_ecpoint_export, CX_OK);
    will_return(cx_ecpoint_destroy, CX_OK);
    will_return(cx_ecpoint_destroy, CX_OK);
    will_return(cx_ecpoint_destroy, -1);
    assert_int_equal(group_add_assign(&aa, &bb), -1);

    group_t a = {
        .x.big.u64
        = {0xc4cd9f1bb1de0944, 0xc8cb981406c4584b, 0x27c929395cf62fe7, 0x10ef9744777f9b8e},
        .y.big.u64
        = {0x648ede4d719168f6, 0xbb63fecb653312df, 0x650de15bd80de1c4, 0x0bbddcd125d67ede}
    };
    group_t b = {
        .x.big.u64
        = {0x5ebc83b515776e99, 0xcc3967723fb5375a, 0xb2afd6566afd3d8b, 0x10bfccadcd357b19},
        .y.big.u64
        = {0x9a4256c37884b0f2, 0x66971181ec612a0b, 0x084e13fe0e5b3a60, 0x006a582e6159fc50}
    };

    will_return(cx_bn_lock, CX_OK);
    will_return(cx_ecpoint_alloc, CX_OK);
    will_return(cx_ecpoint_init, CX_OK);
    will_return(cx_ecpoint_alloc, CX_OK);
    will_return(cx_bn_unlock, CX_OK);
    will_return(cx_ecpoint_init, CX_OK);
    will_return(cx_ecpoint_alloc, CX_OK);
    will_return(cx_ecpoint_add, CX_OK);
    will_return(cx_ecpoint_export, CX_OK);
    will_return(cx_ecpoint_destroy, CX_OK);
    will_return(cx_ecpoint_destroy, CX_OK);
    will_return(cx_ecpoint_destroy, CX_OK);
    assert_int_equal(group_add_assign(&a, &b), 0);
    assert_int_equal(a.x.big.u64[0], 0x035c43ec14ac9ad5);
    assert_int_equal(a.x.big.u64[1], 0xc1fe03f90b8fbc8e);
    assert_int_equal(a.x.big.u64[2], 0x3de3e05e120bcf71);
    assert_int_equal(a.x.big.u64[3], 0x0e14f72f3a141f86);
    assert_int_equal(a.y.big.u64[0], 0x2fa49e075269d164);
    assert_int_equal(a.y.big.u64[1], 0x537bf47febd77c16);
    assert_int_equal(a.y.big.u64[2], 0x042c9a6273f715ed);
    assert_int_equal(a.y.big.u64[3], 0x003e0c7586f9e8ac);

    // group_scalar_multiply
    group_t  r;
    scalar_t s;
    group_t  aaa = {
         .x.big.u64
        = {0xc4cd9f1bb1de0944, 0xc8cb981406c4584b, 0x27c929395cf62fe7, 0x10ef9744777f9b8e},
         .y.big.u64
        = {0x648ede4d719168f6, 0xbb63fecb653312df, 0x650de15bd80de1c4, 0x0bbddcd125d67ede}
    };

    s.big.u64[0] = 0xf8b497fd5a5ba7b0;
    s.big.u64[1] = 0x7ea9409979484264;
    s.big.u64[2] = 0x8a05576f4c19f4bf;
    s.big.u64[3] = 0x0399f3f0056b5577;

    will_return(cx_bn_lock, -1);
    assert_int_equal(group_scalar_multiply(&aaa, &s, &r), -1);

    will_return(cx_bn_lock, CX_OK);
    will_return(cx_ecpoint_alloc, -1);
    will_return(cx_bn_unlock, CX_OK);
    assert_int_equal(group_scalar_multiply(&aaa, &s, &r), -1);

    will_return(cx_bn_lock, CX_OK);
    will_return(cx_ecpoint_alloc, CX_OK);
    will_return(cx_ecpoint_init, -1);
    will_return(cx_bn_unlock, CX_OK);
    assert_int_equal(group_scalar_multiply(&aaa, &s, &r), -1);

    will_return(cx_bn_lock, CX_OK);
    will_return(cx_ecpoint_alloc, CX_OK);
    will_return(cx_ecpoint_init, CX_OK);
    will_return(cx_ecpoint_scalarmul, -1);
    will_return(cx_bn_unlock, CX_OK);
    assert_int_equal(group_scalar_multiply(&aaa, &s, &r), -1);

    will_return(cx_bn_lock, CX_OK);
    will_return(cx_ecpoint_alloc, CX_OK);
    will_return(cx_ecpoint_init, CX_OK);
    will_return(cx_ecpoint_scalarmul, CX_OK);
    will_return(cx_ecpoint_export, -1);
    will_return(cx_bn_unlock, CX_OK);
    assert_int_equal(group_scalar_multiply(&aaa, &s, &r), -1);

    will_return(cx_bn_lock, CX_OK);
    will_return(cx_ecpoint_alloc, CX_OK);
    will_return(cx_ecpoint_init, CX_OK);
    will_return(cx_ecpoint_scalarmul, CX_OK);
    will_return(cx_ecpoint_export, CX_OK);
    will_return(cx_ecpoint_destroy, -1);
    will_return(cx_bn_unlock, CX_OK);
    assert_int_equal(group_scalar_multiply(&aaa, &s, &r), -1);

    will_return(cx_bn_lock, CX_OK);
    will_return(cx_ecpoint_alloc, CX_OK);
    will_return(cx_ecpoint_init, CX_OK);
    will_return(cx_ecpoint_scalarmul, CX_OK);
    will_return(cx_ecpoint_export, CX_OK);
    will_return(cx_ecpoint_destroy, CX_OK);
    will_return(cx_bn_unlock, -1);
    assert_int_equal(group_scalar_multiply(&aaa, &s, &r), -1);

    will_return(cx_bn_lock, CX_OK);
    will_return(cx_ecpoint_alloc, CX_OK);
    will_return(cx_ecpoint_init, CX_OK);
    will_return(cx_ecpoint_scalarmul, CX_OK);
    will_return(cx_ecpoint_export, CX_OK);
    will_return(cx_ecpoint_destroy, CX_OK);
    will_return(cx_bn_unlock, CX_OK);
    assert_int_equal(group_scalar_multiply(&aaa, &s, &r), 0);
    assert_int_equal(r.x.big.u64[0], 0x0d6e211ba1a06372);
    assert_int_equal(r.x.big.u64[1], 0xdbb40d65316c9c57);
    assert_int_equal(r.x.big.u64[2], 0xff1fc3d0468d1c82);
    assert_int_equal(r.x.big.u64[3], 0x12a95287628b21c3);
    assert_int_equal(r.y.big.u64[0], 0x4afd04deaf604187);
    assert_int_equal(r.y.big.u64[1], 0x5339d6d00a0c1587);
    assert_int_equal(r.y.big.u64[2], 0xbe0418a395ae5482);
    assert_int_equal(r.y.big.u64[3], 0x6c8260efbf8431c);

    // group_g_scalar_multiply
    will_return(cx_bn_lock, CX_OK);
    will_return(cx_ecpoint_alloc, CX_OK);
    will_return(cx_ecpoint_init, CX_OK);
    will_return(cx_ecpoint_scalarmul, CX_OK);
    will_return(cx_ecpoint_export, CX_OK);
    will_return(cx_ecpoint_destroy, CX_OK);
    will_return(cx_bn_unlock, CX_OK);
    assert_int_equal(group_g_scalar_multiply(&s, &r), 0);
    assert_int_equal(r.x.big.u64[0], 0x900936dee8538768);
    assert_int_equal(r.x.big.u64[1], 0xdfec83424ef8b77e);
    assert_int_equal(r.x.big.u64[2], 0x930a967d565fb6b3);
    assert_int_equal(r.x.big.u64[3], 0x03eabc8ce2c49ea4);
    assert_int_equal(r.y.big.u64[0], 0xbd3ee5a434daef87);
    assert_int_equal(r.y.big.u64[1], 0x051ca0786448663e);
    assert_int_equal(r.y.big.u64[2], 0x7bb81b96dcb06631);
    assert_int_equal(r.y.big.u64[3], 0x0ee00741dc5140ac);
}

int main()
{
    const struct CMUnitTest tests[] = {cmocka_unit_test(test_group)};

    return cmocka_run_group_tests(tests, NULL, NULL);
}