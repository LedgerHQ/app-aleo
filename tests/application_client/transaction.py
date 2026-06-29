#!/usr/bin/env python3

from enum import Enum
from itertools import islice
from typing import Tuple, Dict, Any
from .bech32m import BECH32M
from .bigint_256 import BigInteger256

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



    @staticmethod
    def forge_tlv(t: TlvTypes, v: str) -> str:
        val = ''
        # type
        if t.value <= 127:
            val += f"{t.value:02x}"
        elif t.value <= 255:
            val += f"81{t.value:02x}"
        else:
            val += f"82{t.value:04x}"

        # length
        length = len(v)//2
        if length <= 127:
            val += f"{length:02x}"
        elif length <= 255:
            val += f"81{length:02x}"
        else:
            val += f"82{length:04x}"

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
            t = Transaction.TlvTypes(0)

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
        for item in Transaction.gen_chunks(cmd, 255*2):
            chunk = ''.join(item)
            if batch_index == 0:
                apdu = cla + ins + p1 + '00'
            else:
                apdu = cla + ins + p1 + '01'
            apdu += f"{(len(chunk)//2):02x}"
            apdu += chunk
            apdus.append(apdu)
            batch_index += 1
        return apdus


    @staticmethod
    def get_plaintext_type_from_string(plaintext_type: str) -> str:
        if plaintext_type not in Transaction.literal_type_from_string:
            return f"01{len(plaintext_type):02x}{plaintext_type.encode('ascii').hex()}"

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
            val += f"03{len(sp_input_type[0]):02x}{sp_input_type[0].encode('ascii').hex()}"
        elif sp_input_type[-1] == 'external_record':
            val += '04'
        elif sp_input_type[-1] == 'merkle_proof':
            val += '020100'

        return val

    @staticmethod
    def generate_input(inputs: dict):
        val = ''
        for input_item in inputs:
            val += Transaction.forge_tlv(Transaction.TlvTypes.INPUT_TYPES, Transaction.get_input_type_from_string(input_item['type']))
            input_val = ''
            if 'address' in input_item['type']:
                hrp : list[int] = []
                data : list[int] = []
                BECH32M.decode(hrp, data, [ord(n) for n in input_item['value']])
                res : list[int] = []
                BECH32M.convert_bits(res, 8, data, 5, False)
                for item in res:
                    input_val += f"{item:02x}"
            elif 'field' in input_item['type']:
                value = int(input_item['value'].split('field')[0])
                big = BigInteger256(int(value))
                input_val += big.to_int().to_bytes(32, 'little').hex()
            elif 'external_record' in input_item['type']:
                input_val = input_item['value']
            elif 'record' in input_item['type']:
                for in_val in input_item['value']:
                    value = int(in_val.split('field')[0])
                    big = BigInteger256(int(value))
                    input_val += big.to_int().to_bytes(32, 'little').hex()
            elif 'u64' in input_item['type']:
                input_val += input_item['value'].to_bytes(8, 'little').hex()
            elif 'u128' in input_item['type']:
                input_val += input_item['value'].to_bytes(16, 'little').hex()
            elif 'merkle_proof' in input_item['type']:
                for in_val in input_item['value']:
                    value = int(in_val.split('field')[0])
                    big = BigInteger256(int(value))
                    input_val += big.to_int().to_bytes(32, 'little').hex()
            else:
                input_val += input_item['value']
            val += Transaction.forge_tlv(Transaction.TlvTypes.INPUT_VALUES, input_val)
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
        val += Transaction.forge_tlv(Transaction.TlvTypes.PROGRAM_ID, f"{request['program_id'].encode().hex()}")
        # Function name
        val += Transaction.forge_tlv(Transaction.TlvTypes.FUNCTION_NAME, f"{request['function_name'].encode().hex()}")
        # Input count
        val += Transaction.forge_tlv(Transaction.TlvTypes.INPUT_COUNT, f"{len(request['inputs']):02x}")
        # Input values & types
        val += Transaction.generate_input(request['inputs'])
        # Nested call count
        if is_root:
            val += Transaction.forge_tlv(Transaction.TlvTypes.NESTED_CALL_COUNT, f"{request['nested_call_count']:02x}")

        if 'program_checksum' in request and len(request['program_checksum']):
            val += Transaction.forge_tlv(Transaction.TlvTypes.PROGRAM_CHECKSUM, request['program_checksum'])

        return val


    def gen_intent_apdu(self, tx: dict) -> list[str]:
        req_start = ''
        # Derivation path
        sp_path = tx['path'].split('/')
        if len(sp_path) > 1:
            req_start += f"{(len(sp_path)-1):02x}"
            for item in sp_path[1:]:
                if "'" in item:
                    # hardenned
                    num = int(item.replace("'", "")) + 0x80000000
                    req_start += f"{num:08x}"
                else:
                    req_start += f"{int(item):08x}"
        else:
            req_start += '00'

        req = ''
        # Structure type
        req += Transaction.forge_tlv(Transaction.TlvTypes.STRUCTURE_TYPE, '28')
        # Version
        req += Transaction.forge_tlv(Transaction.TlvTypes.VERSION, '01')
        # max_base_fee
        req += Transaction.forge_tlv(Transaction.TlvTypes.MAX_BASE_FEE, f"{tx['max_base_fee']:08x}")
        # max_priority_fee
        req += Transaction.forge_tlv(Transaction.TlvTypes.MAX_PRIORITY_FEE, f"{tx['max_priority_fee']:08x}")
        # fee_function_name
        req += Transaction.forge_tlv(Transaction.TlvTypes.FEE_FUNCTION_NAME, tx['fee_function_name'].encode().hex())
        # fee_program_id
        req += Transaction.forge_tlv(Transaction.TlvTypes.FEE_PROGRAM_ID, tx['fee_program_id'].encode().hex())
        # request
        req += Transaction.forge_tlv(Transaction.TlvTypes.REQUEST, Transaction.generate_request(tx['request'], True))

        # Intent length
        req_start += f"{(len(req)//2):04x}"

        apdus = Transaction.gen_apdu_array('e0', '06', '00', req_start+req)

        return apdus


    def gen_nested_call(self, tx: dict) -> list[str]:
        req = ''
        # request
        req += self.generate_request(tx['request'], False)

        # Request length
        req_start = f"{(len(req)//2):04x}"

        apdus = Transaction.gen_apdu_array('e0', '06', '01', req_start+req)

        return apdus


    def gen_fee_apdu(self, tx: dict) -> list[str]:
        req = ''
        # request
        req += self.generate_request(tx['request'], True)

        # Request length
        req_start = f"{(len(req)//2):04x}"

        apdus = Transaction.gen_apdu_array('e0', '06', '02', req_start+req)

        return apdus


    def gen_get_tvk(self, tx: dict) -> list[str]:
        req = ''
        # Derivation path
        sp_path = tx['path'].split('/')
        if len(sp_path) > 1:
            req += f"{(len(sp_path)-1):02x}"
            for item in sp_path[1:]:
                if "'" in item:
                    # hardenned
                    num = int(item.replace("'", "")) + 0x80000000
                    req += f"{num:08x}"
                else:
                    req += f"{int(item):08x}"

        index = tx['index']
        if index == 0:
            apdus = Transaction.gen_apdu_array('e0', '08', '00', req)
        else:
            req += f"{index:02x}"
            apdus = Transaction.gen_apdu_array('e0', '08', '01', req)

        return apdus


    def gen_apdus_tx(self, tx: dict) -> list[str]:
        if tx['type'] == 'intent':
            return self.gen_intent_apdu(tx)
        if tx['type'] == 'nested_call':
            return self.gen_nested_call(tx)
        if tx['type'] == 'fee':
            return self.gen_fee_apdu(tx)
        if tx['type'] == 'get_tvk':
            return self.gen_get_tvk(tx)
        return []


    def unpack_response(self, response: str):
        unpacked : Dict[str, Any] = {}
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
                if 'gammas' not in unpacked:
                    unpacked['gammas'] = []
                unpacked['gammas'].append(v)
        return unpacked


    def unpack_get_tvk_response(self, response: str):
        unpacked : Dict[str, Any] = {}
        if len(response) >= 33*2:
            unpacked['tvk'] = response[2:33*2]
        return unpacked
