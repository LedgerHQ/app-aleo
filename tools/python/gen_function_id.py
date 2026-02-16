#!/usr/bin/env python3


import argparse

from algorithms.bhp import BHP_1024

if __name__ == "__main__":

	parser = argparse.ArgumentParser()
	parser.add_argument('-n', '--network-id',    help="Network id : mainnet or testnet", default="mainnet")
	parser.add_argument("-p", "--program-id",    help="Program ID (name.network)", default=None)
	parser.add_argument("-f", "--function-name", help="Function name", default=None)
	args = parser.parse_args()

	# Sanity checks
	if args.program_id == None:
		print("Program ID is missing")
		exit(-1)

	sp_program_id = args.program_id.split('.')
	if len(sp_program_id) != 2:
		print("Program ID not well formated : " + args.program_id)
		exit(-1)

	if args.function_name == None:
		print("Function name is missing")
		exit(-1)

	if len(args.function_name) == 0:
		print("Empty function name")
		exit(-1)

	program_id_name    = sp_program_id[0]
	program_id_network = sp_program_id[1]
	function_name = args.function_name
	if args.network_id == "mainnet":
		network_id = 0
	else:
		network_id = 1

	input = network_id.to_bytes(2, byteorder='little')
	input += (len(program_id_name)*8).to_bytes(1, byteorder='little')
	input += program_id_name.encode("utf-8")
	input += (len(program_id_network)*8).to_bytes(1, byteorder='little')
	input += program_id_network.encode("utf-8")
	input += (len(function_name)*8).to_bytes(1, byteorder='little')
	input += function_name.encode("utf-8")

	print('input len : ' + str(len(input)))
	print('input: ' + ''.join('{:02x}'.format(x) for x in input))
	print('    else if ((offset == {:d})'.format(len(input)))
	print('        && (!memcmp(bhp_buffer,')
	print('                    "'+ ''.join('\\x{:02x}'.format(x) for x in input) + '", offset))) {')
	print('        // {:d}/{}.{}/{}'.format(network_id, program_id_name, program_id_network, function_name))
	input = (len(input)*8).to_bytes(8, byteorder='little')+input
	bhp = BHP_1024()
	digest = bhp.hash(input, len(input)*8)

	print('        hash->big.u64[0] = 0x{:016x};'.format(digest.value.value[0]))
	print('        hash->big.u64[1] = 0x{:016x};'.format(digest.value.value[1]))
	print('        hash->big.u64[2] = 0x{:016x};'.format(digest.value.value[2]))
	print('        hash->big.u64[3] = 0x{:016x};'.format(digest.value.value[3]))
	print('        return 0;')
	print('    }')
