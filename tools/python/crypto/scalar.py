from crypto.bigint_256 import BigInteger256
from crypto.fp_256     import Fp256, Fp256Parameters


class Scalar(Fp256):

	MODULUS_BITS = 251
	CAPACITY     = MODULUS_BITS-1

	# 70865795004005329077606947863872807680085016823885970091001235374859923341923
	GENERATOR = BigInteger256([ 11289572479685143826,
	                            11383637369941080925,
	                             2288212753973340071,
	                               82014976407880291])

	INV = 9659935179256617473

	# MODULUS = 2111115437357092606062206234695386632838870926408408195193685246394721360383
	MODULUS = BigInteger256([13356249993388743167,
	                          5950279507993463550,
	                         10965441865914903552,
	                           336320092672043349])

	R = BigInteger256([16632263305389933622,
	                   10726299895124897348,
	                   16608693673010411502,
	                     285459069419210737])

	R2 = BigInteger256([ 3987543627614508126,
	                    17742427666091596403,
	                    14557327917022607905,
	                      322810149704226881])

	def __init__(self, val=None):
		param = Fp256Parameters(self.MODULUS_BITS,
		                        self.CAPACITY,
		                        self.GENERATOR,
		                        self.INV,
		                        self.MODULUS,
		                        self.R,
		                        self.R2)
		Fp256.__init__(self, param, val)