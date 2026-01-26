#pragma once

#include <stdint.h>  // uint*_t

typedef struct {
    uint16_t    network_id;
    char program_id_name[32+1];
    char program_id_network[32+1];
    char function_name[32+1];

} function_id_datas_t;

extern int bhp_1024_hash_function_id(const function_id_datas_t *datas, field_t *hash);
