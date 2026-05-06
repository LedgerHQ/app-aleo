from typing import Tuple

class BigInteger256():

    def __init__(self, value):

        if isinstance(value, int):
            self.value = [0]*4
            self.from_int(value)
        elif isinstance(value, list) and len(value) == 4:
            self.value = []
            for el in value:
                self.value.append(el)
        elif isinstance(value, BigInteger256):
            self.value = []
            for el in value.value:
                self.value.append(el)
        else:
            self.value = [0]*4


    def __eq__(self, other):
        return self.value == other.value


    def __ne__(self, other):
        return self.value != other.value


    def __lt__(self, other):
        if self.to_int() < other.to_int():
            return True
        return False


    def __gt__(self, other):
        if self.to_int() > other.to_int():
            return True
        return False


    def from_int(self, num:int = 0):
        for i in range(0, 4):
            self.value[i] = num & 0xFFFFFFFFFFFFFFFF
            num = num >> 64


    def to_int(self) -> int:
        num = 0
        for v in reversed(self.value):
            num <<= 64
            num += v
        return num


    def is_odd(self) -> bool:
        return self.value[0] & 1 == 1


    def is_even(self) -> bool:
        return not self.is_odd()


    def is_zero(self) -> bool:
        return     self.value[0] == 0 \
               and self.value[1] == 0 \
               and self.value[2] == 0 \
               and self.value[3] == 0


    def div2(self):
        t = 0
        for i in reversed(range(0, 4)):
            t2 = self.value[i] << 63
            t2 = t2 % 0x10000000000000000
            self.value[i] >>= 1
            self.value[i] |= t
            t = t2

    @staticmethod
    def add_carry_u64(c:int, a:int, b:int) -> Tuple[int, int]:
        val = c+a+b
        carry = val >> 64
        return val % 0x10000000000000000, carry


    def add_nocarry(self, other) -> bool:
        carry = 0
        self.value[0], carry = BigInteger256.add_carry_u64(carry, self.value[0], other.value[0])
        self.value[1], carry = BigInteger256.add_carry_u64(carry, self.value[1], other.value[1])
        self.value[2], carry = BigInteger256.add_carry_u64(carry, self.value[2], other.value[2])
        self.value[3], carry = BigInteger256.add_carry_u64(carry, self.value[3], other.value[3])
        return carry != 0

    @staticmethod
    def subborrow_u64(c:int, a:int, b:int) -> Tuple[int, int]:
        borrow = 0
        if a >= (b+c):
            val = a - (b+c)
        else:
            val = 0x10000000000000000+a - (b+c)
            borrow = 1
        return val % 0x10000000000000000, borrow


    def sub_noborrow(self, other) -> bool:
        borrow = 0
        self.value[0], borrow = BigInteger256.subborrow_u64(borrow, self.value[0], other.value[0])
        self.value[1], borrow = BigInteger256.subborrow_u64(borrow, self.value[1], other.value[1])
        self.value[2], borrow = BigInteger256.subborrow_u64(borrow, self.value[2], other.value[2])
        self.value[3], borrow = BigInteger256.subborrow_u64(borrow, self.value[3], other.value[3])
        return borrow != 0
