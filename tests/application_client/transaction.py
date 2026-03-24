#!/usr/bin/env python3

import sys
sys.path.append('tools/python')
from crypto.codecs.bech32m import BECH32M
from crypto.bigint_256 import BigInteger256
from enum import Enum
from itertools import islice
from typing import Tuple


class Transaction():

	class TlvTypes(Enum):
		UNKNOWN                  = 0x00
		STRUCTURE_TYPE           = 0x01
		VERSION                  = 0x02
		SIGNATURE                = 0x15
		MAX_BASE_FEE             = 0xb0
		MAX_PRIORITY_FEE         = 0xb1
		FEE_FUNCTION_NAME        = 0xb2
		FEE_PROGRAM_ID           = 0xb3
		REQUEST                  = 0xb4
		NETWORK_ID               = 0xc3
		PROGRAM_ID               = 0xb5
		PROGRAM_CHECKSUM         = 0xc4
		FUNCTION_NAME            = 0xb6
		NESTED_CALL_COUNT        = 0xba
		INPUT_COUNT              = 0xb7
		INPUT_TYPES              = 0xb9
		INPUT_VALUES             = 0xb8
		TVK                      = 0xbf
		TPK                      = 0xc0
		GAMMAS_COUNT             = 0xc1
		GAMMAS                   = 0xc2

	literal_type_from_string = {
		'address'   : '00',
		'boolean'   : '01',
		'field'     : '02',
		'group'     : '03',
		'i8'        : '04',
		'i16'       : '05',
		'i32'       : '06',
		'i64'       : '07',
		'i128'      : '08',
		'u8'        : '09',
		'u16'       : '0a',
		'u32'       : '0b',
		'u64'       : '0c',
		'u128'      : '0d',
		'scalar'    : '0e',
		'signature' : '0f',
		'string'    : '10',
	}


	def forge_tlv(t: TlvTypes, v: str) -> str:
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


	@staticmethod
	def extract_tlv(tlv: str) -> Tuple[TlvTypes, int, str, int]:
		offset = 0
		element = int(tlv[offset:offset+2], base=16)
		offset += 2
		if element < 0x80:
			t = Transaction.TlvTypes(element)
		elif element == 0x81:
			t = Transaction.TlvTypes(int(tlv[offset:offset+2], base=16))
			offset += 2
		elif element == 0x82:
			t = Transaction.TlvTypes(int(tlv[offset:offset+4], base=16))
			offset += 4
		else:
			Transaction.TlvTypes(0)

		l = int(tlv[offset:offset+2], base=16)
		offset += 2

		v = tlv[offset:offset+2*l]
		offset += 2*l

		return t, l, v, offset


	@staticmethod
	def gen_chunks(lst: str, n: int) -> list[tuple]:
		it = iter(lst)
		return list(iter(lambda: tuple(islice(it, n)), ()))


	@staticmethod
	def gen_apdu_array(cla: str, ins: str, p1: str, cmd: str) -> list[str]:
		batch_index = 0
		apdus = []
		for chunk in Transaction.gen_chunks(cmd, 255*2):
			chunk = ''.join(chunk)
			if batch_index == 0:
				apdu = cla + ins + p1 + '00'
			else:
				apdu = cla + ins + p1 + '01'
			apdu += '{:02x}'.format(len(chunk)//2)
			apdu += chunk
			apdus.append(apdu)
			batch_index += 1
		return apdus


	@staticmethod
	def get_plaintext_type_from_string(plaintext_type: str) -> str:
		if plaintext_type not in Transaction.literal_type_from_string.keys():
			return '01{:02x}{}'.format(len(plaintext_type), plaintext_type.encode("ascii").hex())

		val = Transaction.literal_type_from_string[plaintext_type]
		return '00'+val


	@staticmethod
	def get_input_type_from_string(input_type: str) -> str:
		val = ''
		sp_input_type = input_type.split('.')
		if sp_input_type[-1] == 'constant':
			val += '00' + Transaction.get_plaintext_type_from_string(sp_input_type[0])
		elif sp_input_type[-1] == 'public':
			val += '01' + Transaction.get_plaintext_type_from_string(sp_input_type[0])
		elif sp_input_type[-1] == 'private':
			val += '02' + Transaction.get_plaintext_type_from_string(sp_input_type[0])
		elif sp_input_type[-1] == 'record':
			val += '03' + '{:02x}{}'.format(len(sp_input_type[0]), sp_input_type[0].encode("ascii").hex())

		return val


	@staticmethod
	def generate_request(request: dict, is_root: bool) -> str:
		val = ''
		# Structure type
		val += Transaction.forge_tlv(Transaction.TlvTypes.STRUCTURE_TYPE, '29')
		# Version
		val += Transaction.forge_tlv(Transaction.TlvTypes.VERSION, '01')
		# Network_id
		if request['network_id'] == 'mainnet':
			val += Transaction.forge_tlv(Transaction.TlvTypes.NETWORK_ID, '0000')
		else:
			val += Transaction.forge_tlv(Transaction.TlvTypes.NETWORK_ID, '0001')
		# Program id
		val += Transaction.forge_tlv(Transaction.TlvTypes.PROGRAM_ID, '{}'.format(request['program_id'].encode().hex()))
		# Function name
		val += Transaction.forge_tlv(Transaction.TlvTypes.FUNCTION_NAME, '{}'.format(request['function_name'].encode().hex()))
		# Input count
		val += Transaction.forge_tlv(Transaction.TlvTypes.INPUT_COUNT, '{:02x}'.format(len(request['inputs'])))
		# Input values & types
		for input in request['inputs']:
			val += Transaction.forge_tlv(Transaction.TlvTypes.INPUT_TYPES, Transaction.get_input_type_from_string(input['type']))
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
				for in_val in input['value']:
					value = int(in_val.split('field')[0])
					big = BigInteger256(int(value))
					input_val += big.to_int().to_bytes(32, 'little').hex()
			elif 'u64' in input['type']:
				input_val += input['value'].to_bytes(8, 'little').hex()
			else:
				input_val += input['value']
			val += Transaction.forge_tlv(Transaction.TlvTypes.INPUT_VALUES, input_val)
		# Nested call count
		if is_root:
			val += Transaction.forge_tlv(Transaction.TlvTypes.NESTED_CALL_COUNT, '{:02x}'.format(request['nested_call_count']))

		if len(request['program_checksum']):
			val += Transaction.forge_tlv(Transaction.TlvTypes.PROGRAM_CHECKSUM, request['program_checksum'])

		return val


	def gen_intent_apdu(self, tx: dict) -> list[str]:
		req_start = ''
		# Derivation path
		sp_path = tx['path'].split('/')
		if len(sp_path) > 1:
			req_start += '{:02x}'.format(len(sp_path)-1)
			for item in sp_path[1:]:
				if "'" in item:
					# hardenned
					req_start += '{:08x}'.format(int(item.replace("'", "")) + 0x80000000)
				else:
					req_start += '{:08x}'.format(int(item))
		else:
			req_start += '00'

		req = ''
		# Structure type
		req += Transaction.forge_tlv(Transaction.TlvTypes.STRUCTURE_TYPE, '28')
		# Version
		req += Transaction.forge_tlv(Transaction.TlvTypes.VERSION, '01')
		# max_base_fee
		req += Transaction.forge_tlv(Transaction.TlvTypes.MAX_BASE_FEE, '{:08x}'.format(tx['max_base_fee']))
		# max_priority_fee
		req += Transaction.forge_tlv(Transaction.TlvTypes.MAX_PRIORITY_FEE, '{:08x}'.format(tx['max_priority_fee']))
		# fee_function_name
		req += Transaction.forge_tlv(Transaction.TlvTypes.FEE_FUNCTION_NAME, tx['fee_function_name'].encode().hex())
		# fee_program_id
		req += Transaction.forge_tlv(Transaction.TlvTypes.FEE_PROGRAM_ID, tx['fee_program_id'].encode().hex())
		# request
		req += Transaction.forge_tlv(Transaction.TlvTypes.REQUEST, Transaction.generate_request(tx['request'], True))

		# Intent length
		req_start += '{:04x}'.format(len(req)//2)

		apdus = Transaction.gen_apdu_array('e0', '06', '00', req_start+req)

		return apdus


	def gen_nested_call(self, tx: dict) -> list[str]:
		req = ''
		# request
		req += self.generate_request(tx['request'], False)

		# Request length
		req_start = '{:04x}'.format(len(req)//2)

		apdus = Transaction.gen_apdu_array('e0', '06', '01', req_start+req)

		return apdus


	def gen_fee_apdu(self, tx: dict) -> list[str]:
		req = ''
		# request
		req += self.generate_request(tx['request'], True)

		# Request length
		req_start = '{:04x}'.format(len(req)//2)

		apdus = Transaction.gen_apdu_array('e0', '06', '02', req_start+req)

		return apdus


	def gen_apdus_tx(self, tx: dict) -> list[str]:
		if tx['type'] == 'intent':
			return self.gen_intent_apdu(tx)
		elif tx['type'] == 'nested_call':
			return self.gen_nested_call(tx)
		elif tx['type'] == 'fee':
			return self.gen_fee_apdu(tx)
		return []


	def unpack_response(self, response: str):
		unpacked = {}
		offset = 0
		index = 0
		while offset < len(response):
			index += 1
			t, _, v, off = Transaction.extract_tlv(response[offset:])
			offset += off
			if t == Transaction.TlvTypes.STRUCTURE_TYPE:
				unpacked['structure_type'] = int(v, base=16)
			elif t == Transaction.TlvTypes.VERSION:
				unpacked['version'] = int(v, base=16)
			elif t == Transaction.TlvTypes.SIGNATURE:
				unpacked['signature'] = {'pk_sig'    : v[128:192],
				                         'pr_sig'    : v[192:256]}
			elif t == Transaction.TlvTypes.TVK:
				unpacked['tvk'] = v
			elif t == Transaction.TlvTypes.TPK:
				unpacked['tpk'] = v
			elif t == Transaction.TlvTypes.GAMMAS_COUNT:
				unpacked['gammas_count'] = int(v, base=16)
			elif t == Transaction.TlvTypes.GAMMAS:
				if 'gammas' not in unpacked.keys():
					unpacked['gammas'] = []
				unpacked['gammas'].append(v)
		return unpacked

