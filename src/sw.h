#pragma once

#include "status_words.h"

/**
 * Status word for fail to display BIP32 path.
 */
#define SW_DISPLAY_BIP32_PATH_FAIL 0xb001
/**
 * Status word for swap failure
 */
#define SW_SWAP_FAIL 0xc000
/**
 * Application specific swap error code
 */
typedef enum swap_error_application_specific_code_t {
    SWAP_ERROR_CODE             = 0x00,
    SWAP_ERROR_WRONG_TOKEN_INFO = 0x01,
} swap_error_application_specific_code_t;

/**
 * OS status word for OK
 */
#define SWO_OK 0x0000
