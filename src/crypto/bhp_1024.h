#pragma once

#include <stdint.h>  // uint*_t
#include "constants.h"

#include "field.h"

typedef struct {
    uint16_t network_id;
    char     program_id_name[PROGRAM_ID_NAME_MAX_LEN + 1];
    char     program_id_network[PROGRAM_ID_NETWORK_MAX_LEN + 1];
    char     function_name[FUNCTION_NAME_MAX_LEN + 1];

} function_id_datas_t;

extern int bhp_1024_hash_function_id(const function_id_datas_t *data, field_t *hash);
