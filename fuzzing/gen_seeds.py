#!/usr/bin/env python3
"""
Generate the libFuzzer seed corpus for fuzz_sign_transaction.

Each seed is the raw APDU DATA stream (5-byte header stripped from every chunk
and concatenated) for one sign-transaction interaction. Seeds are written one
binary file per case into fuzzing/harness/fuzz_sign_transaction/; build.sh zips
that directory into the seed corpus that clusterfuzzlite loads on first run.

Requires Python >= 3.9 (the reused test client uses PEP 585 generics):
    cd fuzzing && python3 gen_seeds.py
"""

import os
import sys

# Reuse the functional-test client so seeds match real APDU encoding.
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
TESTS_DIR = os.path.join(os.path.dirname(SCRIPT_DIR), "tests")
sys.path.insert(0, TESTS_DIR)

from application_client.transaction import Transaction  # noqa: E402

OUT_DIR = os.path.join(SCRIPT_DIR, "harness", "fuzz_sign_transaction")
os.makedirs(OUT_DIR, exist_ok=True)

# Test vectors (mirror tests/standalone/test_sign_transaction.py).
PATH = "m/44'/683'/0'/0'"
ADDR = "aleo1sfydt6z6cnqjx3hcgk9ajw03ecj6uqlfcm9u3p3gdhckzcc2w5xqv3v3pe"
ADDR_WITHDRAW = "aleo1rhgdu77hgyqd3xjj8ucu3jj9r2krwz6mnzyd80gncr5fxcwlh5rsvzp9px"
RECORD = [
    "3614797564276936744957924747041031196891698846785520060979425601577054464500field",
    "2426895214035216932245297778850989035038538961658726507442215877484415082794field",
    "0220642863446832956019507279394572297489712696240584424406852292692897199577field",
]
EXEC_ID = "7266375125414209082394925781071362722506946030314916664133746682226945366259field"


def _request(function_name: str, inputs: list) -> dict:
    return {
        'network_id': 'mainnet', 'program_id': 'credits.aleo',
        'function_name': function_name, 'inputs': inputs,
        'nested_call_count': 0, 'program_checksum': '',
    }


def intent(function_name: str, inputs: list, fee_function_name: str) -> dict:
    return {
        'type': 'intent', 'path': PATH,
        'max_base_fee': 500, 'max_priority_fee': 100,
        'fee_program_id': 'credits.aleo', 'fee_function_name': fee_function_name,
        'request': _request(function_name, inputs),
    }


def fee(function_name: str, inputs: list) -> dict:
    return {'type': 'fee', 'request': _request(function_name, inputs)}


CASES = {
    'bond_public': intent('bond_public', [
        {'type': 'address.public', 'value': ADDR},
        {'type': 'address.public', 'value': ADDR_WITHDRAW},
        {'type': 'u64.public', 'value': 10000000000},
    ], 'fee_public'),
    'unbond_public': intent('unbond_public', [
        {'type': 'address.public', 'value': ADDR},
        {'type': 'u64.public', 'value': 5000000000},
    ], 'fee_public'),
    'claim_unbond_public': intent('claim_unbond_public', [
        {'type': 'address.public', 'value': ADDR},
    ], 'fee_public'),
    'transfer_public': intent('transfer_public', [
        {'type': 'address.public', 'value': ADDR},
        {'type': 'u64.public', 'value': 1000},
    ], 'fee_public'),
    'transfer_private': intent('transfer_private', [
        {'type': 'credits.record', 'value': RECORD},
        {'type': 'address.private', 'value': ADDR},
        {'type': 'u64.private', 'value': 1000},
    ], 'fee_private'),
    'fee_public': fee('fee_public', [
        {'type': 'u64.public', 'value': 500},
        {'type': 'u64.public', 'value': 100},
        {'type': 'field.public', 'value': EXEC_ID},
    ]),
    'fee_private': fee('fee_private', [
        {'type': 'credits.record', 'value': RECORD},
        {'type': 'u64.public', 'value': 500},
        {'type': 'u64.public', 'value': 100},
        {'type': 'field.public', 'value': EXEC_ID},
    ]),
}

HEADER_HEX_CHARS = 10  # CLA INS P1 P2 Lc = 5 bytes


def apdus_to_seed(apdus: list) -> bytes:
    """Strip the 5-byte header from each APDU chunk and concatenate the payload."""
    return bytes.fromhex("".join(apdu[HEADER_HEX_CHARS:] for apdu in apdus))


for name, tx in CASES.items():
    seed = apdus_to_seed(Transaction().gen_apdus_tx(tx))
    with open(os.path.join(OUT_DIR, f"seed_{name}"), "wb") as f:
        f.write(seed)
    print(f"seed_{name}: {len(seed)} bytes")

print(f"\n{len(CASES)} seeds written to {OUT_DIR}")
