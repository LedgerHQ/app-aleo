from crypto.bigint_256 import BigInteger256
from crypto.field import Field


class Group():

	EDWARDS_A = BigInteger256([0x8cf500000000000e,
	                           0xe75281ef6000000e,
	                           0x49dc37a90b0ba012,
	                            0x55f8b2c6e710ab9])
	EDWARDS_D = BigInteger256([0xd047ffffffff5e30,
	                           0xf0a91026ffff57d2,
	                            0x9013f560d102582,
	                            0x9fd242ca7be5700])

	def __init__(self, x=None, y=None):
		self.x = x
		self.y = y


	def add_assign(self, g):

		one = Field(0)
		one.from_big_int(BigInteger256(1))

		a = Field(self.EDWARDS_A)
		d = Field(self.EDWARDS_D)

		u1 = Field(0)
		u1.sub_assign(a)
		u1.mul_assign(self.x)
		u1.add_assign(self.y)

		u2 = Field(0)
		u2.add_assign(g.x)
		u2.add_assign(g.y)

		u = Field(0)
		u.add_assign(u1)
		u.mul_assign(u2)

		v0 = Field(0)
		v0.add_assign(self.x)
		v0.mul_assign(g.y)

		v1 = Field(0)
		v1.add_assign(g.x)
		v1.mul_assign(self.y)

		v2 = Field(0)
		v2.add_assign(d)
		v2.mul_assign(v0)
		v2.mul_assign(v1)

		q1 = Field(0)
		q1.add_assign(v2)
		q1.add_assign(one)

		q2 = Field(0)
		q2.add_assign(one)
		q2.div_assign(q1)

		x3 = Field(0)
		x3.add_assign(v0)
		x3.add_assign(v1)
		x3.mul_assign(q2)

		q1 = Field(0)
		q1.add_assign(one)
		q1.sub_assign(v2)

		q2 = Field(0)
		q2.add_assign(one)
		q2.div_assign(q1)

		y3 = Field(0)
		y3.add_assign(a)
		y3.mul_assign(v0)
		y3.add_assign(u)
		y3.sub_assign(v1)
		y3.mul_assign(q2)

		self.x = x3
		self.y = y3


	def scalar_multiply(self, scalar):
		pass


	def print(self):
		print('x = ', end='')
		self.x.print()
		print(', y = ', end='')
		self.y.println()