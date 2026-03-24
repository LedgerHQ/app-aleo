#pragma once

#include <stddef.h>  // size_t
#include <stdint.h>  // uint*_t

#include "bip32.h"
#include "account.h"

#include "constants.h"

/**
 * Enumeration with expected INS of APDU commands.
 */
typedef enum {
    CMD_GET_VERSION      = 0x03,  /// version of the application
    CMD_GET_APP_NAME     = 0x04,  /// name of the application
    CMD_GET_ADDRESS      = 0x05,  /// get public address
    CMD_SIGN_TRANSACTION = 0x06,  /// sign transaction
    CMD_GET_VIEW_KEY     = 0x07,  /// get the view key
#ifdef ENABLE_PRIVATE_KEY_MANAGEMENT
    CMD_GET_PRIVATE_KEY = 0xD1,
    CMD_SET_PRIVATE_KEY = 0xFD,
#endif  // ENABLE_PRIVATE_KEY_MANAGEMENT
} command_e;

typedef enum {
    SIGN_MODE_ROOT        = 0x00,
    SIGN_MODE_NESTED_CALL = 0x01,
    SIGN_MODE_FEE         = 0x02,
} sign_transaction_mode_e;

typedef enum {
    SIGNING_STATE_WAIT_INTENT      = 0x00,
    SIGNING_STATE_INTENT           = 0x01,
    SIGNING_STATE_WAIT_NESTED_CALL = 0x02,
    SIGNING_STATE_NESTED_CALL      = 0x03,
    SIGNING_STATE_WAIT_FEES        = 0x04,
    SIGNING_STATE_FEES             = 0x05,
} signing_state_e;

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
    // input
    bool     is_root;
    uint16_t network_id;
    uint8_t  program_id_length;
    char    *program_id;
    uint8_t  function_name_length;
    char    *function_name;
    uint8_t  inputs_count;
    input_t  inputs[MAX_NB_OF_INPUTS];
    uint8_t *program_checksum;
    uint8_t  nested_call_count;

    // internal (for parsing)
    uint8_t inputs_value_offset;
    uint8_t inputs_type_offset;

    // output
    scalar_t r;    // Transition Secret Key
    field_t  tvk;  // Transition View Key
    group_t  tpk;  // Transition Public Key
    field_t  tcm;  // Transition Commitment
    field_t  function_id;
    scalar_t challenge;
    scalar_t response;
    uint8_t  gammas_count;
    group_t  gammas[MAX_NB_OF_RECORDS];

} prepared_request_t;

typedef struct {
    uint32_t max_base_fee;
    uint32_t max_priority_fee;
    uint8_t  fee_function_name_length;
    char     fee_function_name[32];
    uint8_t  fee_program_id_length;
    char     fee_program_id[64];

    prepared_request_t prepared_request;

} sign_transaction_datas_t;

typedef enum {
    TX_UNKNOWN,
    TX_TRANSFER,
    TX_FEE,
} tx_type_e;

typedef enum {
    TX_TRANSFER_PUBLIC,
    TX_TRANSFER_PRIVATE,
    TX_TRANSFER_PUBLIC_TO_PRIVATE,
    TX_TRANSFER_PRIVATE_TO_PUBLIC,
} tx_transfer_type_e;

typedef enum {
    TX_FEE_PUBLIC,
    TX_FEE_PRIVATE,
} tx_fee_type_e;

typedef struct {
    tx_transfer_type_e type;
    char               address_to[ADDRESS_LEN + 1];
    uint64_t           amount;
} tx_transfer_t;

typedef struct {
    tx_fee_type_e type;
    uint64_t      base_fee;
    uint64_t      priority_fee;
} tx_fee_t;

typedef struct {
    tx_type_e     type;
    tx_transfer_t transfer;
    tx_fee_t      fee;
} tx_t;

/**
 * Structure for global context.
 */
typedef struct {
    state_e state;  /// state of the context
    union {
        char address[ADDRESS_LEN];
        char view_key[VIEW_KEY_LEN];
#ifdef ENABLE_PRIVATE_KEY_MANAGEMENT
        char private_key[PRIVATE_KEY_LEN];
#endif  // ENABLE_PRIVATE_KEY_MANAGEMENT
    };
    request_type_e req_type;                    /// user request
    uint32_t       bip32_path[MAX_BIP32_PATH];  /// BIP32 path
    uint8_t        bip32_path_len;              /// length of BIP32 path

    account_t account;

    uint32_t                 fees_waiting_time_ms;
    signing_state_e          signing_state;
    uint8_t                  nested_call_count;
    uint8_t                  nested_call_offset;
    sign_transaction_datas_t sign_transaction_datas;
    tx_t                     tx;
} global_ctx_t;
