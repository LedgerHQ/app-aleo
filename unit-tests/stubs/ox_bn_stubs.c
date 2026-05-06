#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include "cmocka.h"

#include "ox.h"
#include "cxlib.h"

cx_err_t cx_bn_alloc(cx_bn_t *x PLENGTH(sizeof(cx_bn_t)), size_t nbytes)
{
    sys_cx_bn_alloc(x, nbytes);
    return mock_type(cx_err_t);
}

cx_err_t cx_bn_alloc_init(cx_bn_t *bn_x, size_t size, const uint8_t *bytes, size_t nbytes)
{
    sys_cx_bn_alloc_init(bn_x, size, bytes, nbytes);
    return mock_type(cx_err_t);
}

cx_err_t cx_bn_export(const cx_bn_t x, uint8_t *bytes PLENGTH(nbytes), size_t nbytes)
{
    uint8_t *mocked_bn = mock_type(uint8_t *);
    if (mocked_bn) {
        memcpy(bytes, mocked_bn, nbytes);
    }
    else {
        sys_cx_bn_export(x, bytes, nbytes);
    }
    return mock_type(cx_err_t);
}

cx_err_t cx_bn_lock(size_t word_nbytes, uint32_t flags)
{
    sys_cx_bn_lock(word_nbytes, flags);
    return mock_type(cx_err_t);
}

uint32_t cx_bn_unlock(void)
{
    sys_cx_bn_unlock();
    return mock_type(cx_err_t);
}

cx_err_t cx_bn_rng(cx_bn_t r, const cx_bn_t n)
{
    sys_cx_bn_rng(r, n);
    return mock_type(cx_err_t);
}
