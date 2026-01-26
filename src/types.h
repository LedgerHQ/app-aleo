#pragma once

#include <stddef.h>  // size_t
#include <stdint.h>  // uint*_t

#include "bip32.h"

#include "constants.h"

/**
 * Enumeration with expected INS of APDU commands.
 */
typedef enum {
    CMD_GET_VERSION      = 0x03,  /// version of the application
    CMD_GET_APP_NAME     = 0x04,  /// name of the application
    CMD_GET_ADDRESS      = 0x05,  ///
    CMD_SIGN_TRANSACTION = 0x06,  ///
    CMD_GET_VIEW_KEY     = 0x07,  ///
    CMD_GET_PRIVATE_KEY  = 0xD1,  ///
    CMD_SET_PRIVATE_KEY  = 0xFD,  ///
    CMD_TEST             = 0xFE   ///
} command_e;

typedef enum {
    SIGN_MODE_ROOT        = 0x00,
    SIGN_MODE_NESTED_CALL = 0x01,
    SIGN_MODE_FEES        = 0x02,
} sign_transaction_mode_e;

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

typedef struct {
    uint16_t value_length;
    uint8_t *value;
    uint8_t  type_length;
    uint8_t *type;
} input_t;

typedef struct {
    bool     is_root;
    uint16_t network_id;
    uint8_t  program_id_length;
    char    *program_id;
    uint8_t  function_name_length;
    char    *function_name;
    uint8_t  inputs_count;
    input_t  inputs[8];

    // internal (for parsing)
    uint8_t inputs_value_offset;
    uint8_t inputs_type_offset;
} prepared_request_t;

typedef struct {
    uint32_t max_base_fee;
    uint32_t max_priority_fee;
    uint8_t  fee_function_name_length;
    char     fee_function_name[32];
    uint8_t  fee_program_id_length;
    char     fee_program_id[64];

    prepared_request_t root_prepared_request;
    uint8_t            nested_call_count;
    uint8_t            nested_call_offset;
    prepared_request_t nested_calls[4];

} sign_transaction_datas_t;

/**
 * Structure for global context.
 */
typedef struct {
    state_e state;  /// state of the context
    union {
        char address[ADDRESS_LEN];
        char view_key[VIEW_KEY_LEN];
        char private_key[PRIVATE_KEY_LEN];
    };
    request_type_e req_type;                    /// user request
    uint32_t       bip32_path[MAX_BIP32_PATH];  /// BIP32 path
    uint8_t        bip32_path_len;              /// length of BIP32 path

    sign_transaction_datas_t sign_transaction_datas;
} global_ctx_t;
