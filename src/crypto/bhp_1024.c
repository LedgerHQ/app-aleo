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

#include "db_program_function.h"
#include "bhp_1024.h"

int bhp_1024_hash_function_id(prepared_request_t *request)
{
    size_t                 index         = 0;
    size_t                 program_index = 0;
    function_parameters_t *functions     = NULL;

    LEDGER_ASSERT(request != NULL, "NULL request");

    if (request->network_id >= NETWORK_ID_COUNT) {
        return -1;
    }

    // Find program
    for (index = 0; index < NB_OF_PROGRAMS; index++) {
        if (request->program_id_length != strlen(PIC(program_parameters[index].program_id))) {
            continue;
        }
        if (memcmp(request->program_id,
                   PIC(program_parameters[index].program_id),
                   request->program_id_length)) {
            continue;
        }
        break;
    }

    if (index >= NB_OF_PROGRAMS) {
        return -1;
    }

    // Find program's function
    program_index = index;
    functions     = PIC(program_parameters[program_index].functions);

    for (index = 0; index < program_parameters[program_index].nb_of_functions; index++) {
        if (request->function_name_length != strlen(PIC(functions[index].string))) {
            continue;
        }
        if (memcmp(request->function_name,
                   PIC(functions[index].string),
                   request->function_name_length)) {
            continue;
        }
        memcpy(&request->function_id,
               PIC(&functions[index].bhp_1024_hashes[request->network_id]),
               sizeof(field_t));
        return 0;
    }

    return -1;
}
