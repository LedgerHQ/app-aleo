#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include "cmocka.h"

#include "ox.h"
#include "cxlib.h"

cx_err_t cx_ecpoint_alloc(cx_ecpoint_t *P PLENGTH(sizeof(cx_ecpoint_t)), cx_curve_t cv)
{
    sys_cx_ecpoint_alloc(P, cv);
    return mock_type(cx_err_t);
}

cx_err_t cx_ecpoint_init(cx_ecpoint_t *P  PLENGTH(sizeof(cx_ecpoint_t)),
                         const uint8_t *x PLENGTH(x_len),
                         size_t           x_len,
                         const uint8_t *y PLENGTH(y_len),
                         size_t           y_len)
{
    sys_cx_ecpoint_init(P, x, x_len, y, y_len);
    return mock_type(cx_err_t);
}

cx_err_t cx_ecpoint_add(cx_ecpoint_t *R       PLENGTH(sizeof(cx_ecpoint_t)),
                        const cx_ecpoint_t *P PLENGTH(sizeof(cx_ecpoint_t)),
                        const cx_ecpoint_t *Q PLENGTH(sizeof(cx_ecpoint_t)))
{
    sys_cx_ecpoint_add(R, P, Q);
    return mock_type(cx_err_t);
}

cx_err_t cx_ecpoint_export(const cx_ecpoint_t *P PLENGTH(sizeof(cx_ecpoint_t)),
                           uint8_t *x            PLENGTH(x_len),
                           size_t                x_len,
                           uint8_t *y            PLENGTH(y_len),
                           size_t                y_len)
{
    sys_cx_ecpoint_export(P, x, x_len, y, y_len);
    return mock_type(cx_err_t);
}

cx_err_t cx_ecpoint_destroy(cx_ecpoint_t *P PLENGTH(sizeof(cx_ecpoint_t)))
{
    sys_cx_ecpoint_destroy(P);
    return mock_type(cx_err_t);
}

cx_err_t cx_ecpoint_scalarmul(cx_ecpoint_t *P  PLENGTH(sizeof(cx_ecpoint_t)),
                              const uint8_t *k PLENGTH(k_len),
                              size_t           k_len)
{
    sys_cx_ecpoint_scalarmul(P, k, k_len);
    return mock_type(cx_err_t);
}