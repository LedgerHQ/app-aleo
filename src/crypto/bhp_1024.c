/*****************************************************************************
 *   Ledger App Aleo.
 *   (c) 2025 Ledger SAS.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *****************************************************************************/

#include <stdint.h>   // uint*_t
#include <stddef.h>   // size_t
#include <stdbool.h>  // bool
#include <string.h>   // memmove

#include "os.h"
#include "ledger_assert.h"
#include "os_utils.h"
#include "cx.h"

#include "bhp_1024_parameters.h"
#include "bhp_1024.h"

int bhp_1024_hash_function_id(const function_id_datas_t *data, field_t *hash)
{
    size_t             index         = 0;
    size_t             program_index = 0;
    function_hashes_t *functions     = NULL;

    LEDGER_ASSERT(data != NULL, "NULL data");
    LEDGER_ASSERT(hash != NULL, "NULL hash");

    if (data->network_id >= NETWORK_ID_COUNT) {
        return -1;
    }

    for (index = 0; index < NB_OF_PROGRAMS; index++) {
        if (strlen(data->program_id_name) != strlen(PIC(bhp_1024_parameters[index].program_id))) {
            continue;
        }
        if (memcmp(data->program_id_name,
                   PIC(bhp_1024_parameters[index].program_id),
                   strlen(data->program_id_name))) {
            continue;
        }
        if (strlen(data->program_id_network)
            != strlen(PIC(bhp_1024_parameters[index].program_id_network))) {
            continue;
        }
        if (memcmp(data->program_id_network,
                   PIC(bhp_1024_parameters[index].program_id_network),
                   strlen(data->program_id_network))) {
            continue;
        }
        break;
    }

    if (index >= NB_OF_PROGRAMS) {
        return -1;
    }
    program_index = index;
    functions     = PIC(bhp_1024_parameters[program_index].functions);

    for (index = 0; index < bhp_1024_parameters[program_index].nb_of_functions; index++) {
        if (strlen(data->function_name) != strlen(PIC(functions[index].string))) {
            continue;
        }
        if (memcmp(
                data->function_name, PIC(functions[index].string), strlen(data->function_name))) {
            continue;
        }
        memcpy(hash, PIC(&functions[index].hashes[data->network_id]), sizeof(field_t));
        return 0;
    }

    return -1;
}
