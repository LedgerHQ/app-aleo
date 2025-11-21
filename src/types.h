#pragma once

#include <stddef.h>  // size_t
#include <stdint.h>  // uint*_t

#include "bip32.h"

#include "constants.h"

/**
 * Enumeration with expected INS of APDU commands.
 */
typedef enum {
    CMD_GET_VERSION = 0x03,       /// version of the application
    CMD_GET_APP_NAME = 0x04,      /// name of the application
    CMD_GET_ADDRESS = 0x05,       ///
    CMD_SIGN_TRANSACTION = 0x06,  ///
    CMD_GET_VIEW_KEY = 0x07       ///
} command_e;
/**
 * Enumeration with parsing state.
 */
typedef enum {
    STATE_NONE,     /// No state
    STATE_PARSED,   /// Transaction data parsed
    STATE_APPROVED  /// Transaction data approved
} state_e;

/**
 * Enumeration with user request type.
 */
typedef enum {
    CONFIRM_ADDRESS,     /// confirm address derived from view key
    CONFIRM_VIEW_KEY,    /// confirm view key derived from seed
    CONFIRM_TRANSACTION  /// confirm transaction information
} request_type_e;

/**
 * Structure for global context.
 */
typedef struct {
    state_e state;  /// state of the context
    union {
        char address[ADDRESS_LEN];
        char view_key[VIEW_KEY_LEN];
    };
    request_type_e req_type;              /// user request
    uint32_t bip32_path[MAX_BIP32_PATH];  /// BIP32 path
    uint8_t bip32_path_len;               /// length of BIP32 path
} global_ctx_t;
