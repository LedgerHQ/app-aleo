import json

from crypto.bigint_256 import BigInteger256
from crypto.field import Field
from crypto.group import Group

from typing import NamedTuple



class BHPParameters(NamedTuple):

	NUM_WINDOWS:int
	WINDOW_SIZE:int
	LOOKUP_BASE:list[Group]
	DOMAIN:bytes
	DOMAIN_SIZE_IN_BITS:int
	MAX_BITS_PER_ITERATION:int


class BHP():

	def __init__(self, F):
		self.BHP_CHUNK_SIZE = 3
		self.F = F


	def get_lookup_base(self, path):
		json_file = open(path, 'r')
		lookup_content = json.load(json_file)
		json_file.close

		lookup_base = []

		for item in lookup_content:
			list_x = []
			for x in item['x']:
				list_x.append(int(x, 16))
			list_y = []
			for y in item['y']:
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


	def hash(self, input, input_total_bit_len, sum = None, buff_init = [], base_start=0, input_start=0):
		input_nb_of_block = input_total_bit_len + self.F.MAX_BITS_PER_ITERATION -1
		input_nb_of_block = input_nb_of_block // self.F.MAX_BITS_PER_ITERATION

		bhp_buffer = []
		for input_block_index in range(0, input_nb_of_block):
			if input_block_index == 0:
				if sum == None:
					bhp_buffer += self.buffer_to_boolean(self.F.DOMAIN, self.F.DOMAIN_SIZE_IN_BITS)
				bhp_buffer += self.buffer_to_boolean(input, input_total_bit_len)

		bhp_buffer = buff_init+bhp_buffer[input_start:]
		input_total_bit_len -= input_start
		if len(bhp_buffer) % self.BHP_CHUNK_SIZE:
			for index in range(0, self.BHP_CHUNK_SIZE-(len(bhp_buffer) % self.BHP_CHUNK_SIZE)):
				bhp_buffer.append(False)

		if sum == None:
			one = Field(0)
			one.from_big_int(BigInteger256(1))
			sum = Group(Field(0), one)
		#print('sum ', end='')
		#sum.print()
		#print()

		for index in range(0, len(bhp_buffer), self.BHP_CHUNK_SIZE):
			base_offset = 0
			for i in range(0, self.BHP_CHUNK_SIZE):
				if bhp_buffer[index +i]:
					base_offset += 1 << i
			#print(base_offset)
			base_offset += base_start*8+(index // self.BHP_CHUNK_SIZE) * 8
			#print('base ', end='')
			#self.F.LOOKUP_BASE[base_offset].print()
			sum.add_assign(self.F.LOOKUP_BASE[base_offset])
			#print('sum ', end='')
			#sum.print()
			#print()

		return sum.x


	def hash_bis(self, input, input_total_bit_len):
		g = Group(Field(BigInteger256([0xadc1debccef1edc2, 0x75798e0c7e3e4517, 0x28cbcb148b41e651, 0x06be6c247f339f53])),
		          Field(BigInteger256([0x9b0d612d008b4895, 0xd629bf3065662d3a, 0x88cbee7d2b1e3ce7, 0x1121fee88d131f7a])))
		#buff = self.buffer_to_boolean(self.F.DOMAIN, self.F.DOMAIN_SIZE_IN_BITS)[-2:]
		#buff = [False, True]
		#self.hash(input, input_total_bit_len, g, buff, 62)

		g = Group(Field(BigInteger256([0x5bb24a5e91a9ca8c, 0x9b715027001c0763, 0xc82f98226ac8e338, 0x013c39372f71a98c])),
		          Field(BigInteger256([0x2f62910dd470b44a, 0x2e7bf3d229f8ead5, 0x87aa43d956908927, 0x08e58f9fe96b972c])))
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
			if i%8 == 7:
				print("{:02x}".format(val))
				val = 0
			else:
				val >>= 1


class BHP_512(BHP):

	NUM_WINDOWS = 8
	WINDOW_SIZE = 54
	DOMAIN = bytes([0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	                0x00, 0xc0, 0xc4, 0xc8, 0xaa, 0x20, 0x21, 0x64, 0x6f, 0x6a, 0x23, 0x08])
	DOMAIN_SIZE_IN_BITS = 188
	MAX_BITS_PER_ITERATION = 522

	def __init__(self):
		self.LOOKUP_BASE =  self.get_lookup_base('algorithms/BHP_512_lookup.json')
		param = BHPParameters(self.NUM_WINDOWS,
		                      self.WINDOW_SIZE,
		                      self.LOOKUP_BASE,
		                      self.DOMAIN,
		                      self.DOMAIN_SIZE_IN_BITS,
		                      self.MAX_BITS_PER_ITERATION)
		BHP.__init__(self, param)


class BHP_1024(BHP):

	NUM_WINDOWS = 6
	WINDOW_SIZE = 43
	DOMAIN = bytes([0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	                0xc0, 0xc2, 0xc4, 0xc0, 0xa8, 0x20, 0x21, 0x64, 0x6f, 0x6a, 0x23, 0x08])
	DOMAIN_SIZE_IN_BITS = 188
	MAX_BITS_PER_ITERATION = 1044

	def __init__(self):
		self.LOOKUP_BASE =  self.get_lookup_base('algorithms/BHP_1024_lookup.json')
		param = BHPParameters(self.NUM_WINDOWS,
		                      self.WINDOW_SIZE,
		                      self.LOOKUP_BASE,
		                      self.DOMAIN,
		                      self.DOMAIN_SIZE_IN_BITS,
		                      self.MAX_BITS_PER_ITERATION)
		BHP.__init__(self, param)





