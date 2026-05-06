
from crypto.bigint_256 import BigInteger256 as BigInteger

from typing import NamedTuple

class Fp256Parameters(NamedTuple):

	MODULUS_BITS:int
	CAPACITY:int
	GENERATOR:BigInteger
	INV:int
	MODULUS:BigInteger
	R:BigInteger
	R2:BigInteger


class Fp256():

	def __init__(self, F, val=None):
		self.F = F
		if type(val) is BigInteger:
			self.value = BigInteger(val)
		elif type(val) is int:
			self.value = BigInteger(val)
		else:
			self.value = BigInteger(0)



	def is_valid(self) -> bool:
		return self.value.to_int() < self.F.MODULUS.to_int()


	def reduce(self):
		if not self.is_valid():
			self.value.sub_noborrow(self.F.MODULUS)


	def is_zero(self):
		return self.value.is_zero()


	def mac_with_carry(self, a, b, c, carry):
		val = a + (b*c) + carry
		carry = val >> 64
		return val % 0x10000000000000000, carry


	def add_assign(self, other):
		self.value.add_nocarry(other.value)
		self.reduce()


	def sub_assign(self, other):
		if self.value < other.value:
			self.value.add_nocarry(self.F.MODULUS)
		self.value.sub_noborrow(other.value)


	def mul_assign(self, other):
		r = [0]*4
		carry1 = 0
		carry2 = 0

		# Iteration 0
		r[0], carry1 = self.mac_with_carry(r[0], self.value.value[0], other.value.value[0], 0)
		k = (r[0] * self.F.INV) % 0x10000000000000000
		_,    carry2 = self.mac_with_carry(r[0], k, self.F.MODULUS.value[0], 0)

		r[1], carry1 = self.mac_with_carry(r[1], self.value.value[1], other.value.value[0], carry1)
		r[0], carry2 = self.mac_with_carry(r[1], k, self.F.MODULUS.value[1], carry2)

		r[2], carry1 = self.mac_with_carry(r[2], self.value.value[2], other.value.value[0], carry1)
		r[1], carry2 = self.mac_with_carry(r[2], k, self.F.MODULUS.value[2], carry2)

		r[3], carry1 = self.mac_with_carry(r[3], self.value.value[3], other.value.value[0], carry1)
		r[2], carry2 = self.mac_with_carry(r[3], k, self.F.MODULUS.value[3], carry2)
		r[3] = carry1 + carry2

		# Iteration 2
		r[0], carry1 = self.mac_with_carry(r[0], self.value.value[0], other.value.value[1], 0)
		k = (r[0] * self.F.INV) % 0x10000000000000000
		_,    carry2 = self.mac_with_carry(r[0], k, self.F.MODULUS.value[0], 0)

		r[1], carry1 = self.mac_with_carry(r[1], self.value.value[1], other.value.value[1], carry1)
		r[0], carry2 = self.mac_with_carry(r[1], k, self.F.MODULUS.value[1], carry2)

		r[2], carry1 = self.mac_with_carry(r[2], self.value.value[2], other.value.value[1], carry1)
		r[1], carry2 = self.mac_with_carry(r[2], k, self.F.MODULUS.value[2], carry2)

		r[3], carry1 = self.mac_with_carry(r[3], self.value.value[3], other.value.value[1], carry1)
		r[2], carry2 = self.mac_with_carry(r[3], k, self.F.MODULUS.value[3], carry2)
		r[3] = carry1 + carry2

		# Iteration 2
		r[0], carry1 = self.mac_with_carry(r[0], self.value.value[0], other.value.value[2], 0)
		k = (r[0] * self.F.INV) % 0x10000000000000000
		_,    carry2 = self.mac_with_carry(r[0], k, self.F.MODULUS.value[0], 0)

		r[1], carry1 = self.mac_with_carry(r[1], self.value.value[1], other.value.value[2], carry1)
		r[0], carry2 = self.mac_with_carry(r[1], k, self.F.MODULUS.value[1], carry2)

		r[2], carry1 = self.mac_with_carry(r[2], self.value.value[2], other.value.value[2], carry1)
		r[1], carry2 = self.mac_with_carry(r[2], k, self.F.MODULUS.value[2], carry2)

		r[3], carry1 = self.mac_with_carry(r[3], self.value.value[3], other.value.value[2], carry1)
		r[2], carry2 = self.mac_with_carry(r[3], k, self.F.MODULUS.value[3], carry2)
		r[3] = carry1 + carry2

		# Iteration 3
		r[0], carry1 = self.mac_with_carry(r[0], self.value.value[0], other.value.value[3], 0)
		k = (r[0] * self.F.INV) % 0x10000000000000000
		_,    carry2 = self.mac_with_carry(r[0], k, self.F.MODULUS.value[0], 0)

		r[1], carry1 = self.mac_with_carry(r[1], self.value.value[1], other.value.value[3], carry1)
		r[0], carry2 = self.mac_with_carry(r[1], k, self.F.MODULUS.value[1], carry2)

		r[2], carry1 = self.mac_with_carry(r[2], self.value.value[2], other.value.value[3], carry1)
		r[1], carry2 = self.mac_with_carry(r[2], k, self.F.MODULUS.value[2], carry2)

		r[3], carry1 = self.mac_with_carry(r[3], self.value.value[3], other.value.value[3], carry1)
		r[2], carry2 = self.mac_with_carry(r[3], k, self.F.MODULUS.value[3], carry2)
		r[3] = carry1 + carry2

		self.value.value = r
		self.reduce()


	def inverse(self):
		one = BigInteger(1)
		u = self.value
		v = BigInteger(0)
		v.add_nocarry(self.F.MODULUS)
		b = Fp256(self.F, self.F.R2.to_int())
		c = Fp256(self.F, 0)

		while u != one and v != one:
			while u.is_even():
				u.div2()

				if b.value.is_even():
					b.value.div2()
				else:
					b.value.add_nocarry(self.F.MODULUS)
					b.value.div2()

			while v.is_even():
				v.div2()

				if c.value.is_even():
					c.value.div2()
				else:
					c.value.add_nocarry(self.F.MODULUS)
					c.value.div2()

			if v < u:
				u.sub_noborrow(v)
				b.sub_assign(c)
			else:
				v.sub_noborrow(u)
				c.sub_assign(b)

		if u == one:
			return b
		else:
			return c


	def div_assign(self, other):
		self.mul_assign(other.inverse())


	def from_big_int(self, bigint):
		r = Fp256(self.F, bigint)
		if r.is_zero():
			self.value = r.value
		elif r.is_valid():
			u = Fp256(self.F, self.F.R2)
			r.mul_assign(u)
			self.value = r.value


	def to_big_int(self):
		modulus = self.F.MODULUS
		r = BigInteger(self.value)

		k = r.value[0] * self.F.INV
		k = k % 0x10000000000000000
		carry = 0
		_,          carry = self.mac_with_carry(r.value[0], k, modulus.value[0], carry)
		r.value[1], carry = self.mac_with_carry(r.value[1], k, modulus.value[1], carry)
		r.value[2], carry = self.mac_with_carry(r.value[2], k, modulus.value[2], carry)
		r.value[3], carry = self.mac_with_carry(r.value[3], k, modulus.value[3], carry)
		r.value[0] = carry

		k = r.value[1] * self.F.INV
		k = k % 0x10000000000000000
		carry = 0
		_,          carry = self.mac_with_carry(r.value[1], k, modulus.value[0], carry)
		r.value[2], carry = self.mac_with_carry(r.value[2], k, modulus.value[1], carry)
		r.value[3], carry = self.mac_with_carry(r.value[3], k, modulus.value[2], carry)
		r.value[0], carry = self.mac_with_carry(r.value[0], k, modulus.value[3], carry)
		r.value[1] = carry

		k = r.value[2] * self.F.INV
		k = k % 0x10000000000000000
		carry = 0
		_,          carry = self.mac_with_carry(r.value[2], k, modulus.value[0], carry)
		r.value[3], carry = self.mac_with_carry(r.value[3], k, modulus.value[1], carry)
		r.value[0], carry = self.mac_with_carry(r.value[0], k, modulus.value[2], carry)
		r.value[1], carry = self.mac_with_carry(r.value[1], k, modulus.value[3], carry)
		r.value[2] = carry

		k = r.value[3] * self.F.INV
		k = k % 0x10000000000000000
		carry = 0
		_,          carry = self.mac_with_carry(r.value[3], k, modulus.value[0], carry)
		r.value[0], carry = self.mac_with_carry(r.value[0], k, modulus.value[1], carry)
		r.value[1], carry = self.mac_with_carry(r.value[1], k, modulus.value[2], carry)
		r.value[2], carry = self.mac_with_carry(r.value[2], k, modulus.value[3], carry)
		r.value[3] = carry

		return r


	def pow(self, alpha):
		r = Fp256(self.F, self.value.to_int())
		for i in range(0, alpha-1):
			self.mul_assign(r)


	def sum_of_products(self, b):
		sum = Fp256(self.F, 0)
		sum.from_big_int(0)
		print('sum ' + str(sum.to_big_int().to_int()))
		print('a ' + str(self.to_big_int().to_int()))
		for i in range(0, len(b)):
			print('b ' + str(b[i].to_big_int().to_int()))
			r = Fp256(self.F, self.value.to_int())
			r.mul_assign(b[i])
			sum.add_assign(r)
		print('sum ' + str(sum.to_big_int().to_int()))


	def print_u8(self):
		v = self.value.to_int().to_bytes(32, 'big')
		str = ''
		for x in v:
			str += '{:02x} '.format(x)
		print(str)


	def print(self):
		self.value.print_u64()


	def println(self):
		self.value.println_u64()
