#include <stddef.h>  // size_t
#include "field.h"

typedef enum {
    NETWORK_ID_MAINNET = 0,
    NETWORK_ID_TESTNET = 1,
    NETWORK_ID_COUNT   = 2,
} network_id_e;

typedef struct {
    const char *string;
    field_t     hashes[NETWORK_ID_COUNT];
} function_hashes_t;

typedef struct {
    const char              *program_id;
    const char              *program_id_network;
    size_t                   nb_of_functions;
    const function_hashes_t *functions;
} bhp_1024_parameter_t;

#define NB_OF_PROGRAMS (1)
extern const bhp_1024_parameter_t bhp_1024_parameters[NB_OF_PROGRAMS];
