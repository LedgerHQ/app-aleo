#pragma once

#include <stddef.h>  // size_t

#include "types.h"
#include "field.h"

typedef enum {
    NETWORK_ID_MAINNET = 0,
    NETWORK_ID_TESTNET = 1,
    NETWORK_ID_COUNT   = 2,
} network_id_e;

typedef struct {
    char      name[FUNCTION_NAME_MAX_LEN + 1];
    tx_type_e tx_type;
    uint8_t   input_count;
    field_t   bhp_1024_hashes[NETWORK_ID_COUNT];
} function_parameters_t;

int db_get_function_parameters(char                   *program_id,
                               uint8_t                 program_id_length,
                               char                   *function_name,
                               uint8_t                 function_name_length,
                               function_parameters_t **function_parameters);

int db_get_token_display_info(char                  *program_id,
                              uint8_t                program_id_length,
                              field_t               *token_id,
                              token_display_info_t **token_display_info);
