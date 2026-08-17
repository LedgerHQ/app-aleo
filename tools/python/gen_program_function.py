#!/usr/bin/env python3

import argparse
import json

from algorithms.bhp import BHP_1024


def add_c_header(c_file):
    print(
        "/*****************************************************************************",
        file=c_file,
    )
    print(" *   Ledger App Aleo.", file=c_file)
    print(" *   (c) 2025 Ledger SAS.", file=c_file)
    print(" *", file=c_file)
    print(
        ' *  Licensed under the Apache License, Version 2.0 (the "License");',
        file=c_file,
    )
    print(
        " *  you may not use this file except in compliance with the License.",
        file=c_file,
    )
    print(" *  You may obtain a copy of the License at", file=c_file)
    print(" *", file=c_file)
    print(" *      http://www.apache.org/licenses/LICENSE-2.0", file=c_file)
    print(" *", file=c_file)
    print(
        " *  Unless required by applicable law or agreed to in writing, software",
        file=c_file,
    )
    print(
        ' *  distributed under the License is distributed on an "AS IS" BASIS,',
        file=c_file,
    )
    print(
        " *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.",
        file=c_file,
    )
    print(
        " *  See the License for the specific language governing permissions and",
        file=c_file,
    )
    print(" *  limitations under the License.", file=c_file)
    print(
        " *****************************************************************************/",
        file=c_file,
    )


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-f", "--file", help="json file", default=None)
    args = parser.parse_args()

    # Sanity checks
    if args.file is None:
        exit(-1)

    json_file = open(args.file)
    programs = json.load(json_file)

    h_file = open("db_program_function.h", "w")

    print("#pragma once", file=h_file)
    print(file=h_file)
    print("#include <stddef.h>  // size_t", file=h_file)
    print(file=h_file)
    print('#include "types.h"', file=h_file)
    print('#include "db.h"', file=h_file)
    print(file=h_file)

    print("typedef struct {", file=h_file)
    print("    const char                  *program_id;", file=h_file)
    print("    size_t                       nb_of_functions;", file=h_file)
    print("    const function_parameters_t *functions;", file=h_file)
    print("} program_parameter_t;", file=h_file)
    print(file=h_file)

    print(f"#define NB_OF_PROGRAMS ({len(programs):d})", file=h_file)
    print(
        "extern const program_parameter_t program_parameters[NB_OF_PROGRAMS];",
        file=h_file,
    )
    # print('', file=h_file)

    h_file.close()

    c_file = open("db_program_function.c", "w")

    add_c_header(c_file)
    print(file=c_file)
    print('#include "db_program_function.h"', file=c_file)
    print(file=c_file)

    for program_id in programs.keys():
        print(program_id)
        sp_program_id = program_id.split(".")
        program_id_name = sp_program_id[0]
        program_id_network = sp_program_id[1]
        str_program_id = program_id_name + "_" + program_id_network
        print(
            f"#define NB_OF_{str_program_id.upper()}_FUNCTIONS ({len(programs[program_id]):d})",
            file=c_file,
        )
        print(
            f"const function_parameters_t {str_program_id}[NB_OF_{str_program_id.upper()}_FUNCTIONS] = {{",
            file=c_file,
        )
        for item in programs[program_id]:
            function_name = item["function"]
            print("    " + function_name)
            print(f'    {{.name        = "{function_name}",', file=c_file)
            print("     .tx_type     = {},".format(item["tx_type"]), file=c_file)
            print("     .input_count = {:d},".format(item["input_count"]), file=c_file)
            print("     .bhp_1024_hashes", file=c_file)
            item["hashes"] = []
            for network_id in range(2):
                if network_id == 0:
                    print("     = {{.big.u64", file=c_file)
                else:
                    print("        {.big.u64", file=c_file)

                input = network_id.to_bytes(2, byteorder="little")
                input += (len(program_id_name) * 8).to_bytes(1, byteorder="little")
                input += program_id_name.encode("utf-8")
                input += (len(program_id_network) * 8).to_bytes(1, byteorder="little")
                input += program_id_network.encode("utf-8")
                input += (len(function_name) * 8).to_bytes(1, byteorder="little")
                input += function_name.encode("utf-8")
                input = (len(input) * 8).to_bytes(8, byteorder="little") + input
                bhp = BHP_1024()
                digest = bhp.hash(input, len(input) * 8)
                item["hashes"].append(digest)

                if network_id == 1:
                    print(
                        f"         = {{0x{digest.value.value[0]:016x}, 0x{digest.value.value[1]:016x}, 0x{digest.value.value[2]:016x}, 0x{digest.value.value[3]:016x}}}}}}}}},",
                        file=c_file,
                    )
                else:
                    print(
                        f"         = {{0x{digest.value.value[0]:016x}, 0x{digest.value.value[1]:016x}, 0x{digest.value.value[2]:016x}, 0x{digest.value.value[3]:016x}}}}},",
                        file=c_file,
                    )

        print("};", file=c_file)
        print(file=c_file)

    print(
        "const program_parameter_t program_parameters[NB_OF_PROGRAMS] = {", file=c_file
    )
    max_len = 0
    for program_id in programs.keys():
        max_len = max(max_len, len(program_id))
    for program_id in programs.keys():
        str_program_id = program_id.replace(".", "_")
        print(f'    {{.program_id      = "{program_id}",', file=c_file)
        print(
            f"     .nb_of_functions = NB_OF_{str_program_id.upper()}_FUNCTIONS,",
            file=c_file,
        )
        print(
            "     .functions       = {}{}}},".format(
                str_program_id, " " * (max_len - len(str_program_id))
            ),
            file=c_file,
        )
    print("};", file=c_file)
    c_file.close()
