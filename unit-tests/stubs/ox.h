#include <stddef.h>  // size_t
#include <stdint.h>  // uint*_t

#include <openssl/objects.h>
#include <openssl/ec.h>

#include "cx_ec.h"

#include "decorators.h"
#include "os_helpers.h"
#include "cx_errors.h"

cx_err_t cx_bn_alloc_init(cx_bn_t *bn_x, size_t size, const uint8_t *bytes, size_t nbytes);
cx_err_t cx_bn_alloc(cx_bn_t *x PLENGTH(sizeof(cx_bn_t)), size_t nbytes);
cx_err_t cx_bn_export(const cx_bn_t x, uint8_t *bytes PLENGTH(nbytes), size_t nbytes);
cx_err_t cx_bn_lock(size_t word_nbytes, uint32_t flags);
uint32_t cx_bn_unlock(void);
cx_err_t cx_bn_rng(cx_bn_t r, const cx_bn_t n);

cx_err_t cx_ecpoint_alloc(cx_ecpoint_t *P PLENGTH(sizeof(cx_ecpoint_t)), cx_curve_t cv);
cx_err_t cx_ecpoint_init(cx_ecpoint_t *P  PLENGTH(sizeof(cx_ecpoint_t)),
                         const uint8_t *x PLENGTH(x_len),
                         size_t           x_len,
                         const uint8_t *y PLENGTH(y_len),
                         size_t           y_len);
cx_err_t cx_ecpoint_add(cx_ecpoint_t *R       PLENGTH(sizeof(cx_ecpoint_t)),
                        const cx_ecpoint_t *P PLENGTH(sizeof(cx_ecpoint_t)),
                        const cx_ecpoint_t *Q PLENGTH(sizeof(cx_ecpoint_t)));

cx_err_t cx_ecpoint_export(const cx_ecpoint_t *P PLENGTH(sizeof(cx_ecpoint_t)),
                           uint8_t *x            PLENGTH(x_len),
                           size_t                x_len,
                           uint8_t *y            PLENGTH(y_len),
                           size_t                y_len);
cx_err_t cx_ecpoint_destroy(cx_ecpoint_t *P PLENGTH(sizeof(cx_ecpoint_t)));
cx_err_t cx_ecpoint_scalarmul(cx_ecpoint_t *P  PLENGTH(sizeof(cx_ecpoint_t)),
                              const uint8_t *k PLENGTH(k_len),
                              size_t           k_len);
