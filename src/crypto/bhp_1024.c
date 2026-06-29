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

#include "db.h"
#include "bhp_1024.h"

int bhp_1024_hash_function_id(prepared_request_t *request)
{
    LEDGER_ASSERT(request != NULL, "NULL request");

    function_parameters_t *function_parameters = NULL;

    if (request->network_id >= NETWORK_ID_COUNT) {
        return -1;
    }

    int status = db_get_function_parameters(request->program_id,
                                            request->program_id_length,
                                            request->function_name,
                                            request->function_name_length,
                                            &function_parameters);
    if (status < 0) {
        return status;
    }

    memcpy(&request->function_id,
           PIC(&function_parameters->bhp_1024_hashes[request->network_id]),
           sizeof(field_t));

    return 0;
}
