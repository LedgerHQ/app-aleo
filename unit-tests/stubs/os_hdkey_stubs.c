#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include "cmocka.h"

#include "os_hdkey.h"

bolos_err_t sys_hdkey_derive(HDKEY_derive_mode_t derivation_mode,
                             cx_curve_t          curve,
                             const uint32_t     *path,
                             size_t              path_len,
                             uint8_t            *private_key,
                             size_t              private_key_len,
                             uint8_t            *chain_code,
                             size_t              chain_code_len,
                             uint8_t            *seed,
                             size_t              seed_len)
{
    uint8_t *mocked_private_key = mock_type(uint8_t *);
    memcpy(private_key, mocked_private_key, private_key_len);
    return mock_type(bolos_err_t);
}