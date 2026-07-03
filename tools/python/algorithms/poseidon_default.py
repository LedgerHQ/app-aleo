from crypto.field import Field


class PoseidonGrainLFSR:
    def __init__(
        self,
        is_sbox_an_inverse,
        field_size_in_bits,
        state_len,
        num_full_rounds,
        num_partial_rounds,
    ):
        self.field_size_in_bits = field_size_in_bits
        self.state = [False] * 80

        self.state = [False] * 80
        self.state[1] = True
        self.state[5] = is_sbox_an_inverse

        cur = field_size_in_bits
        for i in reversed(range(6, 18)):
            self.state[i] = (cur & 1) == 1
            cur >>= 1

        cur = state_len
        for i in reversed(range(18, 30)):
            self.state[i] = (cur & 1) == 1
            cur >>= 1

        cur = num_full_rounds
        for i in reversed(range(30, 40)):
            self.state[i] = (cur & 1) == 1
            cur >>= 1

        cur = num_partial_rounds
        for i in reversed(range(40, 50)):
            self.state[i] = (cur & 1) == 1
            cur >>= 1

        for i in range(50, 80):
            self.state[i] = True

        self.head = 0

        for i in range(0, 160):
            self.next_bit()

    def next_bit(self):
        next_bit = self.state[(self.head + 62) % 80]
        next_bit ^= self.state[(self.head + 51) % 80]
        next_bit ^= self.state[(self.head + 38) % 80]
        next_bit ^= self.state[(self.head + 23) % 80]
        next_bit ^= self.state[(self.head + 13) % 80]
        next_bit ^= self.state[self.head]
        self.state[self.head] = next_bit
        self.head += 1
        self.head = self.head % 80
        return next_bit

    def it_init(self, num_bits):
        self.it_num_bits = num_bits
        self.it_current_bit = 0

    def it_next(self):
        if self.it_current_bit < self.it_num_bits:
            new_bit = self.next_bit()
            while not new_bit:
                self.next_bit()
                new_bit = self.next_bit()
            self.it_current_bit += 1
            return self.next_bit()
        else:
            return None

    def bits_to_int(self, bits):
        val = 0
        for i in range(len(bits)):
            val <<= 1
            if bits[i]:
                val += 1
        return val


class PoseidonDefault:
    params = {
        "2": {"alpha": 17, "full_rounds": 8, "partial_rounds": 31},
        "3": {"alpha": 17, "full_rounds": 8, "partial_rounds": 31},
        "4": {"alpha": 17, "full_rounds": 8, "partial_rounds": 31},
        "5": {"alpha": 17, "full_rounds": 8, "partial_rounds": 31},
        "6": {"alpha": 17, "full_rounds": 8, "partial_rounds": 31},
        "7": {"alpha": 17, "full_rounds": 8, "partial_rounds": 31},
        "8": {"alpha": 17, "full_rounds": 8, "partial_rounds": 31},
    }

    def __init__(self, rate):
        param = self.params[str(rate)]
        self.rate = rate
        self.alpha = param["alpha"]
        self.full_rounds = param["full_rounds"]
        self.partial_rounds = param["partial_rounds"]
        self.ark, self.mds = self.find_poseidon_ark_and_mds()

    def get_field_elements_rejection_sampling(self, num_elements):
        output = []
        for i in range(0, num_elements):
            while True:
                bits = []
                self.lfsr.it_init(Field.MODULUS_BITS)
                for j in range(0, Field.MODULUS_BITS):
                    bits.append(self.lfsr.it_next())
                val = self.lfsr.bits_to_int(bits)
                if val < Field.MODULUS.to_int():
                    f = Field()
                    f.from_big_int(val)
                    output.append(f)
                    break
        return output

    def get_field_elements_mod_p(self, num_elems):
        output = []
        num_bits = Field.MODULUS_BITS
        for i in range(0, num_elems):
            bits = []
            self.lfsr.it_init(num_bits)
            for j in range(0, Field.MODULUS_BITS):
                bits.append(self.lfsr.it_next())
            val = self.lfsr.bits_to_int(bits)
            val = val.to_bytes(32, byteorder="big")
            val = int.from_bytes(val, byteorder="big") % Field.MODULUS.to_int()
            f = Field()
            f.from_big_int(val)
            output.append(f)
        return output

    def serial_batch_inversion_and_mul(self, v, coeff):
        prod = []
        tmp = Field(Field.R.to_int())  # one
        for f in v:
            if f == 0:
                continue
            tmp.mul_assign(f)
            e = Field(tmp.value.to_int())
            prod.append(e)
        tmp = tmp.inverse()
        tmp.mul_assign(coeff)

        index = 0
        for index in reversed(range(0, len(v))):
            f = v[index]
            if index == 0:
                s = Field(Field.R)
            else:
                s = prod[index - 1]
            new_tmp = Field(tmp.value.to_int())
            new_tmp.mul_assign(f)
            v[index] = Field(tmp.value.to_int())
            v[index].mul_assign(s)
            tmp = new_tmp

    def find_poseidon_ark_and_mds(self):
        self.lfsr = PoseidonGrainLFSR(
            False,
            Field.MODULUS_BITS,
            self.rate + 1,
            self.full_rounds,
            self.partial_rounds,
        )
        ark = []
        for i in range(0, self.full_rounds + self.partial_rounds):
            output = self.get_field_elements_rejection_sampling(self.rate + 1)
            ark.append(output)

        xs = self.get_field_elements_mod_p(self.rate + 1)
        ys = self.get_field_elements_mod_p(self.rate + 1)

        mds_flattened = []
        i = 0
        for x in xs:
            j = 0
            for y in ys:
                e = Field(x.value.to_int())
                e.add_assign(y)
                j += 1
                mds_flattened.append(e)
            i += 1

        self.serial_batch_inversion_and_mul(mds_flattened, Field(Field.R.to_int()))
        mds = []
        tmp = []
        for i in range(0, len(mds_flattened)):
            tmp.append(mds_flattened[i])
            if i % (self.rate + 1) == self.rate:
                mds.append(tmp)
                tmp = []

        return ark, mds
