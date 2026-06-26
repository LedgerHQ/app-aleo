#pragma once

#include <stddef.h>  // size_t

#include "types.h"
#include "db.h"

#define TOKEN_FUNCTION_MAX_COUNT (5)

typedef struct {
    char                         program_id[PROGRAM_ID_NAME_MAX_LEN + 1];
    token_display_info_t         display_info;
    field_t                      token_id;
    size_t                       nb_of_functions;
    const function_parameters_t *functions;
} token_parameter_t;

#define NB_OF_TOKENS (14)
extern const token_parameter_t token_parameters[NB_OF_TOKENS];
