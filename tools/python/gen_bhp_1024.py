#!/usr/bin/env python3

import json
import argparse

from algorithms.bhp import BHP_1024

def add_c_header(c_file):
	print('/*****************************************************************************', file=c_file)
	print(' *   Ledger App Aleo.', file=c_file)
	print(' *   (c) 2025 Ledger SAS.', file=c_file)
	print(' *', file=c_file)
	print(' *  Licensed under the Apache License, Version 2.0 (the "License");', file=c_file)
	print(' *  you may not use this file except in compliance with the License.', file=c_file)
	print(' *  You may obtain a copy of the License at', file=c_file)
	print(' *', file=c_file)
	print(' *      http://www.apache.org/licenses/LICENSE-2.0', file=c_file)
	print(' *', file=c_file)
	print(' *  Unless required by applicable law or agreed to in writing, software', file=c_file)
	print(' *  distributed under the License is distributed on an "AS IS" BASIS,', file=c_file)
	print(' *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.', file=c_file)
	print(' *  See the License for the specific language governing permissions and', file=c_file)
	print(' *  limitations under the License.', file=c_file)
	print(' *****************************************************************************/', file=c_file)


if __name__ == "__main__":

	parser = argparse.ArgumentParser()
	parser.add_argument("-f", "--file", help="json file", default=None)
	args = parser.parse_args()

	# Sanity checks
	if args.file == None:
		exit(-1)

	json_file = open(args.file)
	programs = json.load(json_file)

	h_file = open('bhp_1024_parameters.h', 'w')

	print('#include <stddef.h>  // size_t', file=h_file)
	print('#include "field.h"', file=h_file)
	print('', file=h_file)

	print('typedef enum {', file=h_file)
	print('    NETWORK_ID_MAINNET = 0,', file=h_file)
	print('    NETWORK_ID_TESTNET = 1,', file=h_file)
	print('    NETWORK_ID_COUNT   = 2,', file=h_file)
	print('} network_id_e;', file=h_file)
	print('', file=h_file)

	print('typedef struct {', file=h_file)
	print('    const char *string;', file=h_file)
	print('    field_t     hashes[NETWORK_ID_COUNT];', file=h_file)
	print('} function_hashes_t;', file=h_file)
	print('', file=h_file)

	print('typedef struct {', file=h_file)
	print('    const char              *program_id;', file=h_file)
	print('    const char              *program_id_network;', file=h_file)
	print('    size_t                   nb_of_functions;', file=h_file)
	print('    const function_hashes_t *functions;', file=h_file)
	print('} bhp_1024_parameter_t;', file=h_file)
	print('', file=h_file)

	print('#define NB_OF_PROGRAMS ({:d})'.format(len(programs)), file=h_file)
	print('extern const bhp_1024_parameter_t bhp_1024_parameters[NB_OF_PROGRAMS];', file=h_file)
	#print('', file=h_file)

	h_file.close()

	c_file = open('bhp_1024_parameters.c', 'w')

	add_c_header(c_file)
	print('', file=c_file)
	print('#include "bhp_1024_parameters.h"', file=c_file)
	print('', file=c_file)

	for program_id in programs.keys():
		print(program_id)
		sp_program_id = program_id.split('.')
		program_id_name    = sp_program_id[0]
		program_id_network = sp_program_id[1]
		str_program_id = program_id_name + '_' + program_id_network
		print('#define NB_OF_{}_FUNCTIONS ({:d})'.format(str_program_id.upper(), len(programs[program_id])), file=c_file)
		print('const function_hashes_t {}[NB_OF_{}_FUNCTIONS] = {{'.format(str_program_id, str_program_id.upper()), file=c_file)
		for item in programs[program_id]:
			function_name = item['function']
			print('    ' + function_name)
			print('    {{.string = "{}",'.format(function_name), file=c_file)
			print('     .hashes', file=c_file)
			item['hashes'] = []
			for network_id in range(0, 2):
				if network_id == 0:
					print('     = {{.big.u64', file=c_file)
				else:
					print('        {.big.u64', file=c_file)
				
				input = network_id.to_bytes(2, byteorder='little')
				input += (len(program_id_name)*8).to_bytes(1, byteorder='little')
				input += program_id_name.encode("utf-8")
				input += (len(program_id_network)*8).to_bytes(1, byteorder='little')
				input += program_id_network.encode("utf-8")
				input += (len(function_name)*8).to_bytes(1, byteorder='little')
				input += function_name.encode("utf-8")
				input = (len(input)*8).to_bytes(8, byteorder='little')+input
				bhp = BHP_1024()
				digest = bhp.hash(input, len(input)*8)
				item['hashes'].append(digest)

				if network_id == 1:
					print('         = {{0x{:016x}, 0x{:016x}, 0x{:016x}, 0x{:016x}}}}}}}}},'.format(digest.value.value[0], digest.value.value[1], digest.value.value[2], digest.value.value[3]), file=c_file)
				else:
					print('         = {{0x{:016x}, 0x{:016x}, 0x{:016x}, 0x{:016x}}}}},'.format(digest.value.value[0], digest.value.value[1], digest.value.value[2], digest.value.value[3]), file=c_file)

		print('};', file=c_file)
		print('', file=c_file)

	print('const bhp_1024_parameter_t bhp_1024_parameters[NB_OF_PROGRAMS] = {', file=c_file)
	for program_id in programs.keys():
		sp_program_id = program_id.split('.')
		program_id_name    = sp_program_id[0]
		program_id_network = sp_program_id[1]
		str_program_id = program_id_name + '_' + program_id_network
		print('    {{.program_id         = "{}",'.format(program_id_name), file=c_file)
		print('     .program_id_network = "{}",'.format(program_id_network), file=c_file)
		print('     .nb_of_functions    = NB_OF_{}_FUNCTIONS,'.format(str_program_id.upper()), file=c_file)
		print('     .functions          = {}}},'.format(str_program_id), file=c_file)
	print('};', file=c_file)
	c_file.close()
