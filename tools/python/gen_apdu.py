#!/usr/bin/env python3


from crypto.codecs.bech32m import BECH32M
from crypto.bigint_256 import BigInteger256
import argparse
import json
from enum import Enum
from itertools import islice

class TlvTypes(Enum):
	STRUCTURE_TYPE           = 0x01
	VERSION                  = 0x02
	SIGNATURE                = 0x15
	MAX_BASE_FEE             = 0xb0
	MAX_PRIORITY_FEE         = 0xb1
	FEE_FUNCTION_NAME        = 0xb2
	FEE_PROGRAM_ID           = 0xb3
	REQUEST                  = 0xb4
	PROGRAM_ID               = 0xb5
	FUNCTION_NAME            = 0xb6
	INPUT_COUNT              = 0xb7
	INPUT_VALUES             = 0xb8
	INPUT_TYPES              = 0xb9
	NESTED_CALL_COUNT        = 0xba
	TVK                      = 0xbf
	TPK                      = 0xc0
	GAMMAS_COUNT             = 0xc1
	GAMMAS                   = 0xc2
	NETWORK_ID               = 0x5b
	PROGRAM_CHECKSUM         = 0x3b


def get_tlv(t, v):
	val = ''
	# type
	if t.value <= 127:
		val += '{:02x}'.format(t.value)
	elif t.value <= 255:
		val += '81{:02x}'.format(t.value)
	else:
		val += '82{:04x}'.format(t.value)

	# length
	length = len(v)//2
	if length <= 127:
		val += '{:02x}'.format(length)
	elif length <= 255:
		val += '81{:02x}'.format(length)
	else:
		val += '82{:04x}'.format(length)

	# value
	val += v

	return val

def gen_chunks(lst, n):
	it = iter(lst)
	return iter(lambda: tuple(islice(it, n)), ())

def gen_apdu_array(cla, ins, p1, cmd):
	batch_index = 0
	apdus = []
	for chunk in list(gen_chunks(cmd, 255*2)):
		chunk = ''.join(chunk)
		if batch_index == 0:
			apdu = cla + ins + p1 + '00'
		else:
			apdu = cla + ins + p1 + '80'
		apdu += '{:02x}'.format(len(chunk)//2)
		apdu += chunk
		apdus.append(apdu)
	return apdus


def get_literal_type_from_string(literal_type):
	if literal_type == 'address':
		return '00'
	elif literal_type == 'boolean':
		return '01'
	elif literal_type == 'field':
		return '02'
	elif literal_type == 'group':
		return '03'
	elif literal_type == 'i8':
		return '04'
	elif literal_type == 'i16':
		return '05'
	elif literal_type == 'i32':
		return '06'
	elif literal_type == 'i64':
		return '07'
	elif literal_type == 'i128':
		return '08'
	elif literal_type == 'u8':
		return '09'
	elif literal_type == 'u16':
		return '0a'
	elif literal_type == 'u32':
		return '0b'
	elif literal_type == 'u64':
		return '0c'
	elif literal_type == 'u128':
		return '0d'
	elif literal_type == 'scalar':
		return '0e'
	elif literal_type == 'signature':
		return '0f'
	elif literal_type == 'string':
		return '10'
	return None


def get_plaintext_type_from_string(plaintext_type):
	val = get_literal_type_from_string(plaintext_type)
	if val:
		return '00'+val

	return '01{:02x}{}'.format(len(plaintext_type), plaintext_type.encode("ascii").hex())


def get_input_type_from_string(input_type):
	val = ''
	sp_input_type = input_type.split('.')
	if sp_input_type[-1] == 'constant':
		val += '00' + get_plaintext_type_from_string(sp_input_type[0])
	elif sp_input_type[-1] == 'public':
		val += '01' + get_plaintext_type_from_string(sp_input_type[0])
	elif sp_input_type[-1] == 'private':
		val += '02' + get_plaintext_type_from_string(sp_input_type[0])
	elif sp_input_type[-1] == 'record':
		val += '03' + '{:02x}{}'.format(len(sp_input_type[0]), sp_input_type[0].encode("ascii").hex())

	return val



def generate_request_apdu(request, is_root, nested_call_count=0):
	val = ''
	# Structure type
	val += get_tlv(TlvTypes.STRUCTURE_TYPE, '29')
	# Version
	val += get_tlv(TlvTypes.VERSION, '01')
	# Network_id
	if request['network_id'] == 'mainnet':
		val += get_tlv(TlvTypes.NETWORK_ID, '0000')
	else:
		val += get_tlv(TlvTypes.NETWORK_ID, '0001')
	# Program id
	val += get_tlv(TlvTypes.PROGRAM_ID, '{}'.format(request['program_id'].encode().hex()))
	# Function name
	val += get_tlv(TlvTypes.FUNCTION_NAME, '{}'.format(request['function_name'].encode().hex()))
	# Input count
	val += get_tlv(TlvTypes.INPUT_COUNT, '{:02x}'.format(len(request['inputs'])))
	# Input values & types
	for input in request['inputs']:
		val += get_tlv(TlvTypes.INPUT_TYPES, get_input_type_from_string(input['type']))
		input_val = ''
		if 'address' in input['type']:
			hrp = []
			data = []
			BECH32M.decode(hrp, data, [ord(n) for n in input['value']])
			res = []
			BECH32M.convert_bits(res, 8, data, 5, False)
			for item in res:
				input_val += '{:02x}'.format(item)
		elif 'field' in input['type']:
			value = int(input['value'].split('field')[0])
			big = BigInteger256(int(value))
			input_val += big.to_int().to_bytes(32, 'little').hex()
		elif 'record' in input['type']:
			value = int(input['value'].split('field')[0])
			big = BigInteger256(int(value))
			input_val += big.to_int().to_bytes(32, 'little').hex()
		elif 'u64' in input['type']:
			input_val += input['value'].to_bytes(8, 'little').hex()
		else:
			input_val += input['value']
		val += get_tlv(TlvTypes.INPUT_VALUES, input_val)
	# Nested call count
	if is_root:
		val += get_tlv(TlvTypes.NESTED_CALL_COUNT, '{:02x}'.format(nested_call_count))

	if len(request['program_checksum']):
		val += get_tlv(TlvTypes.PROGRAM_CHECKSUM, request['program_checksum'])

	return val


def generate_root_apdu(cmd, nested_call_count):

	req = ''
	# Structure type
	req += get_tlv(TlvTypes.STRUCTURE_TYPE, '28')
	# Version
	req += get_tlv(TlvTypes.VERSION, '01')
	# max_base_fee
	req += get_tlv(TlvTypes.MAX_BASE_FEE, '{:08x}'.format(cmd['max_base_fee']))
	# max_priority_fee
	req += get_tlv(TlvTypes.MAX_PRIORITY_FEE, '{:08x}'.format(cmd['max_priority_fee']))
	# fee_function_name
	req += get_tlv(TlvTypes.FEE_FUNCTION_NAME, cmd['fee_function_name'].encode().hex())
	# fee_program_id
	req += get_tlv(TlvTypes.FEE_PROGRAM_ID, cmd['fee_program_id'].encode().hex())
	# request
	req += get_tlv(TlvTypes.REQUEST, generate_request_apdu(cmd['request'], 1, nested_call_count))

	# Derivation path
	req_start = '058000002c800002ab800000000000000000000000'
	# Intent length
	req_start += '{:04x}'.format(len(req)//2)

	apdus = gen_apdu_array('e0', '06', '00', req_start+req)

	return apdus


def generate_fee_apdu(cmd):

	req = ''
	# request
	req += generate_request_apdu(cmd['request'], 1, 0)

	# Request length
	req_start = '{:04x}'.format(len(req)//2)

	apdus = gen_apdu_array('e0', '06', '02', req_start+req)

	return apdus


if __name__ == "__main__":

	parser = argparse.ArgumentParser()
	parser.add_argument("--file", help="json file", default=None)
	args = parser.parse_args()

	if args.file == None:
		exit(-1)

	json_file = open(args.file)
	datas = json.load(json_file)
	cmds = datas['cmds']

	for cmd in cmds:
		print(cmd)
		if cmd['type'] == 'root':
			apdu = generate_root_apdu(cmd, 0)
			for item in apdu:
				print('echo {} | python3 -m ledgerblue.runScript --apdu'.format(item))
		elif cmd['type'] == 'nested_call':
			pass
		elif cmd['type'] == 'fee':
			apdu = generate_fee_apdu(cmd)
			for item in apdu:
				print('echo {} | python3 -m ledgerblue.runScript --apdu'.format(item))
			pass

	exit(0)
