#pragma once

#include <stddef.h>  // size_t

#include "types.h"
#include "db.h"

typedef struct {
    const char                  *program_id;
    size_t                       nb_of_functions;
    const function_parameters_t *functions;
} program_parameter_t;

#define NB_OF_PROGRAMS (7)
extern const program_parameter_t program_parameters[NB_OF_PROGRAMS];
