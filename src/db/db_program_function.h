#include <stddef.h>  // size_t

#include "types.h"
#include "field.h"

typedef enum {
    NETWORK_ID_MAINNET = 0,
    NETWORK_ID_TESTNET = 1,
    NETWORK_ID_COUNT   = 2,
} network_id_e;

typedef struct {
    const char *string;
    tx_type_e   tx_type;
    uint8_t     input_count;
    field_t     bhp_1024_hashes[NETWORK_ID_COUNT];
} function_parameters_t;

typedef struct {
    const char                  *program_id;
    size_t                       nb_of_functions;
    const function_parameters_t *functions;
} program_parameter_t;

#define NB_OF_PROGRAMS (7)
extern const program_parameter_t program_parameters[NB_OF_PROGRAMS];
