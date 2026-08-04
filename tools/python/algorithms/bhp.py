import json

from crypto.bigint_256 import BigInteger256
from crypto.field import Field
from crypto.group import Group

from typing import NamedTuple


class BHPParameters(NamedTuple):
    NUM_WINDOWS: int
    WINDOW_SIZE: int
    LOOKUP_BASE: list[Group]
    DOMAIN: bytes
    DOMAIN_SIZE_IN_BITS: int
    MAX_BITS_PER_ITERATION: int


class BHP:
    def __init__(self, F):
        self.BHP_CHUNK_SIZE = 3
        self.F = F

    def get_lookup_base(self, path):
        json_file = open(path, "r")
        lookup_content = json.load(json_file)
        json_file.close

        lookup_base = []

        for item in lookup_content:
            list_x = []
            for x in item["x"]:
                list_x.append(int(x, 16))
            list_y = []
            for y in item["y"]:
                list_y.append(int(y, 16))

            g = Group(Field(BigInteger256(list_x)), Field(BigInteger256(list_y)))
            lookup_base.append(g)

        return lookup_base

    def buffer_to_boolean(self, buffer, buffer_bit_len):
        result = []
        for bit_index in range(0, buffer_bit_len):
            if buffer[bit_index // 8] & (1 << (bit_index % 8)):
                result.append(True)
            else:
                result.append(False)
        return result

    def hash(
        self,
        input,
        input_total_bit_len,
        sum=None,
        buff_init=[],
        base_start=0,
        input_start=0,
    ):
        input_nb_of_block = input_total_bit_len + self.F.MAX_BITS_PER_ITERATION - 1
        input_nb_of_block = input_nb_of_block // self.F.MAX_BITS_PER_ITERATION

        bhp_buffer = []
        for input_block_index in range(0, input_nb_of_block):
            if input_block_index == 0:
                if sum is None:
                    bhp_buffer += self.buffer_to_boolean(
                        self.F.DOMAIN, self.F.DOMAIN_SIZE_IN_BITS
                    )
                bhp_buffer += self.buffer_to_boolean(input, input_total_bit_len)

        bhp_buffer = buff_init + bhp_buffer[input_start:]
        input_total_bit_len -= input_start
        if len(bhp_buffer) % self.BHP_CHUNK_SIZE:
            for index in range(
                0, self.BHP_CHUNK_SIZE - (len(bhp_buffer) % self.BHP_CHUNK_SIZE)
            ):
                bhp_buffer.append(False)

        if sum is None:
            one = Field(0)
            one.from_big_int(BigInteger256(1))
            sum = Group(Field(0), one)
        # print('sum ', end='')
        # sum.print()
        # print()

        for index in range(0, len(bhp_buffer), self.BHP_CHUNK_SIZE):
            base_offset = 0
            for i in range(0, self.BHP_CHUNK_SIZE):
                if bhp_buffer[index + i]:
                    base_offset += 1 << i
            # print(base_offset)
            base_offset += base_start * 8 + (index // self.BHP_CHUNK_SIZE) * 8
            # print('base ', end='')
            # self.F.LOOKUP_BASE[base_offset].print()
            sum.add_assign(self.F.LOOKUP_BASE[base_offset])
            # print('sum ', end='')
            # sum.print()
            # print()

        return sum.x

    def hash_bis(self, input, input_total_bit_len):
        g = Group(
            Field(
                BigInteger256(
                    [
                        0xADC1DEBCCEF1EDC2,
                        0x75798E0C7E3E4517,
                        0x28CBCB148B41E651,
                        0x06BE6C247F339F53,
                    ]
                )
            ),
            Field(
                BigInteger256(
                    [
                        0x9B0D612D008B4895,
                        0xD629BF3065662D3A,
                        0x88CBEE7D2B1E3CE7,
                        0x1121FEE88D131F7A,
                    ]
                )
            ),
        )
        # buff = self.buffer_to_boolean(self.F.DOMAIN, self.F.DOMAIN_SIZE_IN_BITS)[-2:]
        # buff = [False, True]
        # self.hash(input, input_total_bit_len, g, buff, 62)

        g = Group(
            Field(
                BigInteger256(
                    [
                        0x5BB24A5E91A9CA8C,
                        0x9B715027001C0763,
                        0xC82F98226AC8E338,
                        0x013C39372F71A98C,
                    ]
                )
            ),
            Field(
                BigInteger256(
                    [
                        0x2F62910DD470B44A,
                        0x2E7BF3D229F8EAD5,
                        0x87AA43D956908927,
                        0x08E58F9FE96B972C,
                    ]
                )
            ),
        )
        buff = self.buffer_to_boolean(input, 317)[-1:]
        buff = [False]
        self.hash(input, input_total_bit_len, g, buff, 168, 317)
        buff = self.buffer_to_boolean(input, input_total_bit_len)
        buff = buff[317:]
        print(len(buff))
        print(buff)
        val = 0
        for i in range(0, 253):
            if buff[i]:
                val += 128
            if i % 8 == 7:
                print("{:02x}".format(val))
                val = 0
            else:
                val >>= 1


class BHP_512(BHP):
    NUM_WINDOWS = 8
    WINDOW_SIZE = 54
    DOMAIN = bytes(
        [
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
            0xC0,
            0xC4,
            0xC8,
            0xAA,
            0x20,
            0x21,
            0x64,
            0x6F,
            0x6A,
            0x23,
            0x08,
        ]
    )
    DOMAIN_SIZE_IN_BITS = 188
    MAX_BITS_PER_ITERATION = 522

    def __init__(self):
        self.LOOKUP_BASE = self.get_lookup_base("algorithms/BHP_512_lookup.json")
        param = BHPParameters(
            self.NUM_WINDOWS,
            self.WINDOW_SIZE,
            self.LOOKUP_BASE,
            self.DOMAIN,
            self.DOMAIN_SIZE_IN_BITS,
            self.MAX_BITS_PER_ITERATION,
        )
        BHP.__init__(self, param)


class BHP_1024(BHP):
    NUM_WINDOWS = 6
    WINDOW_SIZE = 43
    DOMAIN = bytes(
        [
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
            0xC0,
            0xC2,
            0xC4,
            0xC0,
            0xA8,
            0x20,
            0x21,
            0x64,
            0x6F,
            0x6A,
            0x23,
            0x08,
        ]
    )
    DOMAIN_SIZE_IN_BITS = 188
    MAX_BITS_PER_ITERATION = 1044

    def __init__(self):
        self.LOOKUP_BASE = self.get_lookup_base("algorithms/BHP_1024_lookup.json")
        param = BHPParameters(
            self.NUM_WINDOWS,
            self.WINDOW_SIZE,
            self.LOOKUP_BASE,
            self.DOMAIN,
            self.DOMAIN_SIZE_IN_BITS,
            self.MAX_BITS_PER_ITERATION,
        )
        BHP.__init__(self, param)
