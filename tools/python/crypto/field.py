
from crypto.bigint_256 import BigInteger256
from crypto.fp_256     import Fp256, Fp256Parameters


class Field(Fp256):

	MODULUS_BITS = 253
	CAPACITY     = MODULUS_BITS-1

	# GENERATOR = 22
	# Encoded in Montgomery form, so the value is
	# (22 * R) % q = 5642976643016801619665363617888466827793962762719196659561577942948671127251
	GENERATOR = BigInteger256([ 2984901390528151251,
	                           10561528701063790279,
	                            5476750214495080041,
	                             898978044469942640])

	INV = 725501752471715839

	# MODULUS = 8444461749428370424248824938781546531375899335154063827935233455917409239041
	MODULUS = BigInteger256([ 725501752471715841,
	                         6461107452199829505,
	                         6968279316240510977,
	                         1345280370688173398])

	R = BigInteger256([ 9015221291577245683,
	                    8239323489949974514,
	                    1646089257421115374,
	                     958099254763297437])

	R2 = BigInteger256([ 2726216793283724667,
	                    14712177743343147295,
	                    12091039717619697043,
	                       81024008013859129])

	def __init__(self, val=None):
		param = Fp256Parameters(self.MODULUS_BITS,
		                        self.CAPACITY,
		                        self.GENERATOR,
		                        self.INV,
		                        self.MODULUS,
		                        self.R,
		                        self.R2)
		Fp256.__init__(self, param, val)

