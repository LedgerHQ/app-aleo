#!/usr/bin/env python3


from crypto.codecs.bech32m import BECH32M
import argparse
import json
from enum import Enum
from itertools import islice

class TlvTypes(Enum):
	SIGNATURE                = 0x15
	MAX_BASE_FEE             = 0x40
	MAX_PRIORITY_FEE         = 0x41
	FEE_FUNCTION_NAME        = 0x42
	FEE_PROGRAM_ID           = 0x43
	REQUEST                  = 0x44
	PROGRAM_ID               = 0x45
	FUNCTION_NAME            = 0x46
	INPUT_COUNT              = 0x47
	INPUT_VALUES             = 0x48
	INPUT_TYPES              = 0x49
	NESTED_CALL_COUNT        = 0x4a
	RECORD_COMMITMENTS_COUNT = 0x4c
	RECORD_COMMITMENT        = 0x4d
	TVK                      = 0x4f
	TPK                      = 0x50
	GAMMAS_COUNT             = 0x51
	GAMMAS                   = 0x52
	IS_ROOT                  = 0x5a
	NETWORK_ID               = 0x5b


def get_tlv(t, v):
	val = ''
	# type
	val += '{:02x}'.format(t.value)
	# length
	val += '{:02x}'.format(len(v)//2)
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
	# Is root
	val += get_tlv(TlvTypes.IS_ROOT, '{:02x}'.format(is_root))
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
		if len(input['value']) == 63 and input['value'][0:5] == 'aleo1':
			hrp = []
			data = []
			BECH32M.decode(hrp, data, [ord(n) for n in input['value']])
			res = []
			BECH32M.convert_bits(res, 8, data, 5, False)
			for item in res:
				input_val += '{:02x}'.format(item)
		else:
			input_val += input['value']
		val += get_tlv(TlvTypes.INPUT_VALUES, input_val)
	# Nested call count
	if is_root:
		val += get_tlv(TlvTypes.NESTED_CALL_COUNT, '{:02x}'.format(nested_call_count))
	# Record commitments count
	val += get_tlv(TlvTypes.RECORD_COMMITMENTS_COUNT, '{:02x}'.format(len(request['commitments'])))
	# Commitments
	for commitment in request['commitments']:
		val += commitment

	return val


def generate_root_apdu(cmd, nested_call_count):

	req = ''
	# Structure type
	req += '28'
	# Version
	req += '01'
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


if __name__ == "__main__":
	HOST = '127.0.0.1'

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
				print('m_tool.sh --nanox --calvados --apdu ' + item)
		elif cmd['type'] == 'nested_call':
			pass
		elif cmd['type'] == 'fee':
			pass

	exit(0)
