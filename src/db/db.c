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

#include "os.h"
#include "ledger_assert.h"

#include "db.h"
#include "db_program_function.h"
#include "db_tokens.h"

static int get_function_parameters(char                   *program_id,
                                   uint8_t                 program_id_length,
                                   char                   *function_name,
                                   uint8_t                 function_name_length,
                                   function_parameters_t **function_parameters)
{
    size_t                 index         = 0;
    size_t                 program_index = 0;
    function_parameters_t *functions     = NULL;

    // Find program
    for (index = 0; index < NB_OF_PROGRAMS; index++) {
        if (program_id_length != strlen(PIC(program_parameters[index].program_id))) {
            continue;
        }
        if (memcmp(program_id, PIC(program_parameters[index].program_id), program_id_length)) {
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
        if (function_name_length != strlen(PIC(functions[index].name))) {
            continue;
        }
        if (memcmp(function_name, PIC(functions[index].name), function_name_length)) {
            continue;
        }
        break;
    }

    if (index >= program_parameters[program_index].nb_of_functions) {
        return -1;
    }

    *function_parameters
        = (function_parameters_t *) PIC(&program_parameters[program_index].functions[index]);

    return 0;
}

static int get_token_function_parameters(char                   *program_id,
                                         uint8_t                 program_id_length,
                                         char                   *function_name,
                                         uint8_t                 function_name_length,
                                         function_parameters_t **function_parameters)
{
    size_t                 index         = 0;
    size_t                 program_index = 0;
    function_parameters_t *functions     = NULL;

    // Find program
    for (index = 0; index < NB_OF_TOKENS; index++) {
        if (program_id_length != strlen(PIC(token_parameters[index].program_id))) {
            continue;
        }
        if (memcmp(program_id, PIC(token_parameters[index].program_id), program_id_length)) {
            continue;
        }
        break;
    }

    if (index >= NB_OF_TOKENS) {
        return -1;
    }

    // Find program's function
    program_index = index;
    functions     = PIC(token_parameters[program_index].functions);

    for (index = 0; index < token_parameters[program_index].nb_of_functions; index++) {
        if (function_name_length != strlen(PIC(functions[index].name))) {
            continue;
        }
        if (memcmp(function_name, PIC(functions[index].name), function_name_length)) {
            continue;
        }
        break;
    }

    if (index >= token_parameters[program_index].nb_of_functions) {
        return -1;
    }

    *function_parameters
        = (function_parameters_t *) PIC(&token_parameters[program_index].functions[index]);

    return 0;
}

int db_get_function_parameters(char                   *program_id,
                               uint8_t                 program_id_length,
                               char                   *function_name,
                               uint8_t                 function_name_length,
                               function_parameters_t **function_parameters)
{
    LEDGER_ASSERT(program_id != NULL, "NULL program_id");
    LEDGER_ASSERT(function_name != NULL, "NULL function_name");
    LEDGER_ASSERT(function_parameters != NULL, "NULL function_parameters");

    int status = get_function_parameters(
        program_id, program_id_length, function_name, function_name_length, function_parameters);
    if (status == 0) {
        return status;
    }

    return get_token_function_parameters(
        program_id, program_id_length, function_name, function_name_length, function_parameters);
}

int db_get_token_display_info(char                  *program_id,
                              uint8_t                program_id_length,
                              field_t               *token_id,
                              token_display_info_t **token_display_info)
{
    LEDGER_ASSERT(program_id != NULL, "NULL program_id");
    LEDGER_ASSERT(token_display_info != NULL, "NULL token_display_info");

    size_t index = 0;

    // Find program
    for (index = 0; index < NB_OF_TOKENS; index++) {
        if (program_id_length != strlen(PIC(token_parameters[index].program_id))) {
            continue;
        }
        if (memcmp(program_id, PIC(token_parameters[index].program_id), program_id_length)) {
            continue;
        }
        else if (token_id) {
            if (memcmp(token_id, PIC(&token_parameters[index].token_id), sizeof(field_t))) {
                continue;
            }
        }
        break;
    }

    if (index >= NB_OF_TOKENS) {
        return -1;
    }

    *token_display_info = PIC(&token_parameters[index].display_info);

    return 0;
}