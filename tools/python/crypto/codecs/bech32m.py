

class BECH32M():

	CHARSET = "qpzry9x8gf2tvdw0s3jn54khce6mua7l"

	CHARSET_REVERSED = [
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 15, -1, 10, 17,
		21, 20, 26, 30, 7,  5,  -1, -1, -1, -1, -1, -1, -1, 29, -1, 24, 13, 25, 9,  8,  23, -1, 18, 22, 31, 27,
		19, -1, 1,  0,  3,  16, 11, 28, 12, 14, 6,  4,  2,  -1, -1, -1, -1, -1, -1, 29, -1, 24, 13, 25, 9,  8,
		23, -1, 18, 22, 31, 27, 19, -1, 1,  0,  3,  16, 11, 28, 12, 14, 6,  4,  2,  -1, -1, -1, -1, -1]

	def __init__(self):
		pass


	def polymod_step(pre: int):
		b = pre >> 25
		res = (pre & 0x1FFFFFF) << 5
		res ^= (-((b >> 0) & 1) & 0x3B6A57B2)
		res ^= (-((b >> 1) & 1) & 0x26508E6D)
		res ^= (-((b >> 2) & 1) & 0x1EA119FA)
		res ^= (-((b >> 3) & 1) & 0x3D4233DD)
		res ^= (-((b >> 4) & 1) & 0x2A1462B3)
		return res


	def final_constant(is_m_encoding: bool) -> int:
		if is_m_encoding:
			return 0x2bc830a3
		return 1


	def convert_bits(output: list[int], output_bits: int, input: list[int], input_bits: int, pad: bool) -> bool:
		val = 0
		bits = 0
		max_v = (1 << output_bits) - 1
		for input_offset in range(0, len(input)):
			val = (val << input_bits) | input[input_offset]
			bits += input_bits
			while bits >= output_bits:
				bits -= output_bits
				output.append((val >> bits) & max_v)
		if pad:
			if bits:
				output.append((val << (output_bits-bits)) & max_v)
		elif ((val << (output_bits-bits)) & max_v) or bits >= input_bits:
			return False

		return True


	def encode(output: list[int], hrp: list[int], data: list[int], is_m_encoding: bool) -> bool:
		chk = 1
		for item in hrp:
			ch = item
			if ch < 33 or ch > 126:
				return False
			if ch >= 65 and ch <= 90:
				return False
			
			chk = BECH32M.polymod_step(chk) ^ (ch >> 5)

		if len(hrp)+7+len(data) > 90:
			return False

		chk = BECH32M.polymod_step(chk)
		for item in hrp:
			chk = BECH32M.polymod_step(chk) ^ (item & 0x1f)
			output.append(item)

		output.append(49)

		for i in range(0, len(data)):
			if data[i] >> 5:
				return False
			chk = BECH32M.polymod_step(chk) ^ data[i]
			output.append(ord(BECH32M.CHARSET[data[i]]))

		for i in range(0, 6):
			chk = BECH32M.polymod_step(chk)
		chk ^= BECH32M.final_constant(is_m_encoding)
		for i in range(0, 6):
			output.append(ord(BECH32M.CHARSET[(chk >> ((5-i)*5)) & 0x1f]))

		return True


	def decode(hrp: list[int], data: list[int], input: list[int]) -> bool:
		chk = 1
		have_lower = False
		have_upper = False
		if len(input) < 8 or len(input) > 90:
			return False
		
		data_offset = 0
		while data_offset < len(input) and input[len(input)-1 - data_offset] != 49:
			data_offset += 1

		hrp_len = len(input) - (1+data_offset)
		if 1+data_offset >= len(input) or data_offset < 6:
			return False

		for index in range(0, hrp_len):
			ch = input[index]
			if ch < 33 or ch > 126:
				return False
			if ch >= 97 and ch < 122:
				have_lower = True
			if ch >= 65 and ch <= 90:
				have_upper = True
			hrp.append(ch)
			chk = BECH32M.polymod_step(chk) ^ (ch >> 5);
		chk = BECH32M.polymod_step(chk)
		for index in range(0, hrp_len):
			chk = BECH32M.polymod_step(chk) ^ (input[index] & 0x1f)
		index = hrp_len + 1
		while index < len(input):
			ch = input[index]
			if ch & 0x80:
				v = -1
			else:
				v = BECH32M.CHARSET_REVERSED[ch]
			if ch >= 97 and ch < 122:
				have_lower = True
			if ch >= 65 and ch < 90:
				have_upper = True
			if v == -1:
				return False
			chk = BECH32M.polymod_step(chk) ^ v
			if index+6 < len(input):
				data.append(v)
			index += 1

		return True

