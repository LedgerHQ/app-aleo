#!/usr/bin/env python3

import json
import argparse

from algorithms.bhp import BHP_1024
from crypto.bigint_256 import BigInteger256
from crypto.field import Field

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
	tokens = json.load(json_file)

	h_file = open('db_tokens.h', 'w')

	print('#pragma once', file=h_file)
	print('', file=h_file)
	print('#include <stddef.h>  // size_t', file=h_file)
	print('', file=h_file)
	print('#include "types.h"', file=h_file)
	print('#include "db.h"', file=h_file)
	print('', file=h_file)
	print('#define TOKEN_FUNCTION_MAX_COUNT (5)', file=h_file)
	print('', file=h_file)
	print('typedef struct {', file=h_file)
	print('    char                         program_id[PROGRAM_ID_NAME_MAX_LEN + 1];', file=h_file)
	print('    token_display_info_t         display_info;', file=h_file)
	print('    field_t                      token_id;', file=h_file)
	print('    size_t                       nb_of_functions;', file=h_file)
	print('    const function_parameters_t *functions;', file=h_file)
	print('} token_parameter_t;', file=h_file)
	print('', file=h_file)
	print('#define NB_OF_TOKENS ({:d})'.format(len(tokens)), file=h_file)
	print('extern const token_parameter_t token_parameters[NB_OF_TOKENS];', file=h_file)

	h_file.close()

	c_file = open('db_tokens.c', 'w')

	add_c_header(c_file)
	print('', file=c_file)
	print('#include "db_tokens.h"', file=c_file)
	print('', file=c_file)

	for token_name in tokens.keys():
		print(token_name)
		token = tokens[token_name]
		sp_program_id = token['program_name'].split('.')
		program_id_name    = sp_program_id[0]
		program_id_network = sp_program_id[1]
		str_program_id = program_id_name + '_' + program_id_network
		print('#define NB_OF_{}_FUNCTIONS ({:d})'.format(str_program_id.upper(), len(token['functions'])), file=c_file)
		print('const function_parameters_t {}[NB_OF_{}_FUNCTIONS] = {{'.format(str_program_id, str_program_id.upper()), file=c_file)
		for function_name in token['functions'].keys():
			function = token['functions'][function_name]
			print('    ' + function_name)
			print('    {{.name        = "{}",'.format(function_name), file=c_file)
			print('     .tx_type     = {},'.format(function['tx_type']), file=c_file)
			print('     .input_count = {:d},'.format(function['input_count']), file=c_file)
			print('     .bhp_1024_hashes', file=c_file)
			function['hashes'] = []
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
				function['hashes'].append(digest)

				if network_id == 1:
					# Testnet
					print('         = {{0x{:016x}, 0x{:016x}, 0x{:016x}, 0x{:016x}}}}}}}}},'.format(digest.value.value[0], digest.value.value[1], digest.value.value[2], digest.value.value[3]), file=c_file)
				else:
					# Mainnet
					if token['mainnet_availability']:
						print('         = {{0x{:016x}, 0x{:016x}, 0x{:016x}, 0x{:016x}}}}},'.format(digest.value.value[0], digest.value.value[1], digest.value.value[2], digest.value.value[3]), file=c_file)
					else:
						print('         = {{0x{:016x}, 0x{:016x}, 0x{:016x}, 0x{:016x}}}}},'.format(0, 0, 0, 0), file=c_file)

		print('};', file=c_file)
		print('', file=c_file)

	print('const token_parameter_t token_parameters[NB_OF_TOKENS] = {', file=c_file)
	max_len = 0
	for token_name in tokens.keys():
		token = tokens[token_name]
		if len(token['program_name']) > max_len:
			max_len = len(token['program_name'])
	for token_name in tokens.keys():
		token = tokens[token_name]
		str_program_id = token['program_name'].replace('.', '_')
		value = int(token['token_id'].split('field')[0])
		big = BigInteger256(int(value))
		token_id = Field()
		token_id.from_big_int(big)
		print('    {{.program_id   = "{}",'.format(token['program_name']), file=c_file)
		print('     .display_info = {{.type = {}, .ticker = "{}", .decimals = {:d}}},'.format(token['token_type'], token['ticker'], token['decimals']), file=c_file)
		print('     .token_id', file=c_file)
		print('     = {.big.u64', file=c_file)
		print('        = {{0x{:016x}, 0x{:016x}, 0x{:016x}, 0x{:016x}}}}},'.format(token_id.value.value[0], token_id.value.value[1], token_id.value.value[2], token_id.value.value[3]), file=c_file)
		print('     .nb_of_functions = NB_OF_{}_FUNCTIONS,'.format(str_program_id.upper()), file=c_file)
		print('     .functions       = {}{}}},'.format(str_program_id, ' '*(max_len-len(str_program_id))), file=c_file)

	print('};', file=c_file)
	c_file.close()