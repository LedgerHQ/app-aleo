
from algorithms.poseidon_default import PoseidonDefault

if __name__ == "__main__":

	c_file = open('poseidon_parameters.c', 'w')
	print('', file=c_file)
	print('#include <stdint.h>   // uint*_t', file=c_file)
	print('#include <stddef.h>   // size_t', file=c_file)
	print('#include <stdbool.h>  // bool', file=c_file)
	print('#include <string.h>   // memmove', file=c_file)
	print('', file=c_file)
	print('#include "poseidon_parameters.h"', file=c_file)
	print('', file=c_file)

	for rate in [2, 4, 8]:
		pos = PoseidonDefault(rate)
		ark, mds = pos.find_poseidon_ark_and_mds()

		print('const field_t ark_rate_{:d}[{:d}] = {{'.format(rate, len(ark) * len(ark[0])), file=c_file)
		for x in ark:
			for y in x:
				str = '    {' + '.big.u64 = ' + '{'
				for u in y.value.value:
					str += '0x{:016x}, '.format(u)
				str = str[:-1]
				str += '}},'
				print(str, file=c_file)
			print('', file=c_file)
		print('};', file=c_file)
		print('', file=c_file)

		print('const field_t mds_rate_{:d}[{:d}]={{'.format(rate, len(mds) * len(mds[0])), file=c_file)
		for x in mds:
			for y in x:
				str = '    {' + '.big.u64 = ' + '{'
				for u in y.value.value:
					str += '0x{:016x}, '.format(u)
				str = str[:-1]
				str += '}},'
				print(str, file=c_file)
			print('', file=c_file)
		print('};', file=c_file)
		print('', file=c_file)

	print('\
void poseidon_parameters_init(poseidon_parameters_t *parameters, uint8_t rate)\n\
{\n\
	switch (rate) {\n\
		case 2:\n\
			parameters->full_rounds    = 8;\n\
			parameters->partial_rounds = 31;\n\
			parameters->alpha          = 17;\n\
			parameters->ark            = (field_t*)ark_rate_2;\n\
			parameters->mds            = (field_t*)mds_rate_2;\n\
			break;\n\
		case 4:\n\
			parameters->full_rounds    = 8;\n\
			parameters->partial_rounds = 31;\n\
			parameters->alpha          = 17;\n\
			parameters->ark            = (field_t*)ark_rate_4;\n\
			parameters->mds            = (field_t*)mds_rate_4;\n\
			break;\n\
		case 8:\n\
			parameters->full_rounds    = 8;\n\
			parameters->partial_rounds = 31;\n\
			parameters->alpha          = 17;\n\
			parameters->ark            = (field_t*)ark_rate_8;\n\
			parameters->mds            = (field_t*)mds_rate_8;\n\
			break;\n\
		default:\n\
			parameters->full_rounds    = 8;\n\
			parameters->partial_rounds = 31;\n\
			parameters->alpha          = 17;\n\
			parameters->ark            = (field_t*)ark_rate_2;\n\
			parameters->mds            = (field_t*)mds_rate_2;\n\
			break;\n\
	}\n\
}\n', file=c_file)

	c_file.close()


