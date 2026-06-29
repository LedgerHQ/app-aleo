# Aleo application : Sign transaction APDU specification

## Overview

This document describes the APDU messages interface to perform transaction signing.

Note that the BIP32 path should use the mandatory format : m/44'/683'/${account_index}

### SIGN TRANSACTION INTENT

#### Description

_This commands initiate the signature process, expecting a derivation path, fee constraint and a signature request_

#### Command

| _CLA_ | _INS_ | _P1_ | _P2_ |   _Lc_   | _CData_  |
| ----- | :---: | ---: | ---- | :------: | -------: |
| E0    | 06    | 00   | 00   | variable | variable |

#### Input data (CData)

| _Description_                                    | _Length_ | _Type_ |
| ------------------------------------------------ | :------: |  ----: |
| Number of BIP 32 derivations to perform (max 10) |    1     |     u8 |
| First derivation index (big endian)              |    4     |    u32 |
| ...                                              |    4     |    u32 |
| Last derivation index (big endian)               |    4     |    u32 |
| Transaction intent length (big endian)           |    2     |    u16 |
| Transaction intent                               | variable | [Transaction intent](#transaction-intent) |

##### Transaction intent
Serialized TLV data:

| _Name_                | _Tag_ | _Length_ | _Type_ | _Description_ |
| -----                 | :---: |    :--:  | -:     | -             |
| `structure_type`      |  0x01 |      1   |     u8 | The structure type (0x28) |
| `structure_version`   |  0x02 |      1   |     u8 | The structure format version (0x01) |
| `max_base_fee`        |  0xb0 |      4   |    u32 | Maximum value for the base fee |
| `max_priority_fee`    |  0xb1 |      4   |    u32 | Maximum value for the priority fee |
| `fee_function_name`   |  0xb2 | variable |  bytes | Function name to use to pay the fees |
| `fee_program_id`      |  0xb3 | variable |  bytes | Program ID to use to pay the fees |
| `request`             |  0xb4 | variable |  [request](#request) | Intent prepared request |


#### Output data (RData)

| _Description_     | _Length_ | _Type_ |
| ----------------- | :------: |  ----: |
| Request signature | variable | [signature](#signature) |

### SIGN NESTED CALL

#### Description

_This commands can be called several time after initialization on the signature process, expecting signature request_
_The device uses tvk from the intent's signature response as 'root_tvk'_

#### Command

| _CLA_ | _INS_ | _P1_ | _P2_ |   _Lc_   | _CData_  |
| ----- | :---: | ---: | ---- | :------: | -------: |
| E0    | 06    | 01   | 00   | variable | variable |

#### Input data (CData)

| _Description_               | _Length_ | _Type_ |
| -------------------------   | :------: |  ----: |
| Request length (big endian) |    2     |    u16 |
| request                     | variable | [request](#request) |

#### Output data (RData)

| _Description_     | _Length_ | _Type_ |
| ----------------- | :------: |  ----: |
| Request signature | variable | [request signature](#request-signature) |

### SIGN FEE

#### Description

_This commands is called at the end of the signature process, expecting signature request_
_The given fee values in the signature request will be verified to matche the intent's fee constraints and the program/function as well_

#### Command

| _CLA_ | _INS_ | _P1_ | _P2_ |   _Lc_   | _CData_  |
| ----- | :---: | ---: | ---- | :------: | -------: |
| E0    | 06    | 02   | 00   | variable | variable |

#### Input data (CData)

| _Description_               | _Length_ | _Type_ |
| -------------------------   | :------: |  ----: |
| Request length (big endian) |    2     |    u16 |
| request                     | variable | [request](#request) |

#### Output data (RData)

| _Description_     | _Length_ | _Type_ |
| ----------------- | :------: |  ----: |
| Request signature | variable | [request signature](#request-signature) |

## Common structures / types

### Request
Serialized TLV data:

| _Name_               | _Tag_ | _Length_ | _Type_ | _Description_ |
| -----                | :---: |    :--:  | -:     | -             |
| `structure_type`     |  0x01 |      1   |     u8 | The structure type (0x29) |
| `structure_version`  |  0x02 |      1   |     u8 | The structure format version (0x01) |
| `network_id`         |  0xc3 |      2   |    u16 | Network ID (big endian) (0 : mainnet, 1 : testnet) |
| `program_id`         |  0xb5 | variable |  bytes | Program ID to use |
| `program_checksum`   |  0xc4 |     32   |  field | Program checksum (OPTIONAL) |
| `function_name`      |  0xb6 | variable |  bytes | Function name to call |
| `nested_calls_count` |  0xba |      1   |     u8 | The number of nested calls (INTENT ONLY) |
| `input_count`        |  0xb7 |      1   |     u8 | The number of inputs |
| `input type`         |  0xb9 | variable |  bytes | [Input type](#input-type) (must be present `input_count` time) |
| `input value`        |  0xb8 | variable |  bytes | [Input value](#input-value) (must be present `input_count` time) |

### Input type
| _Name_       | _Length_ | _Type_  | _Description_ |
| -----        |    :--:  | -:      | -             |
| `value type` |     1    |      u8 | 0x00 : [constant](#constant-type) |
|                                 ||| 0x01 : [public](#public-type)  |
|                                 ||| 0x02 : [private](#private-type)  |
|                                 ||| 0x03 : [record](#record-type) |
|                                 ||| 0x04 : [external_record](#external-record-type) |

#### Constant type

| _Name_           | _Length_ | _Type_  | _Description_ |
| -----            |    :--:  | -:      | -             |
| `value type`     |     1    |      u8 | 0x00 |
| `plaintext type` |     1    |   bytes | [plaintext type](#plaintext-type) |

#### Public type

| _Name_           | _Length_ | _Type_  | _Description_ |
| -----            |    :--:  | -:      | -             |
| `value type`     |     1    |      u8 | 0x01 |
| `plaintext type` |     1    |   bytes | [plaintext type](#plaintext-type) |

#### Private type

| _Name_           | _Length_ | _Type_  | _Description_ |
| -----            |    :--:  | -:      | -             |
| `value type`     |     1    |      u8 | 0x02 |
| `plaintext type` |     1    |   bytes | [plaintext type](#plaintext-type) |

#### Record type

| _Name_           | _Length_ | _Type_  | _Description_ |
| -----            |    :--:  | -:      | -             |
| `value type`     |     1    |      u8 | 0x03 |
| `identifier`     | variable |   bytes | [identifier](#identifier) |

#### External record type

| _Name_           | _Length_ | _Type_  | _Description_ |
| -----            |    :--:  | -:      | -             |
| `value type`     |     1    |      u8 | 0x04 |

#### Plaintext type
| _Name_           | _Length_ | _Type_  | _Description_ |
| -----            |    :--:  | -:      | -             |
| `plaintext type` |     1    |      u8 | 0x00 : [literal](#plaintext-literal) |
|                                     ||| 0x01 : struct
|                                     ||| 0x02 : array

##### Plaintext literal
| _Name_           | _Length_ | _Type_  | _Description_ |
| -----            |    :--:  | -:      | -             |
| `plaintext type` |     1    |      u8 | 0x00 :Literal |
| `literal type`   |     1    |      u8 | 0x00 : Address |
|                                     ||| 0x01 : Boolean |
|                                     ||| 0x02 : Field |
|                                     ||| 0x03 : Group |
|                                     ||| 0x04 : i8 |
|                                     ||| 0x05 : i16 |
|                                     ||| 0x06 : i32 |
|                                     ||| 0x07 : i64 |
|                                     ||| 0x08 : i128 |
|                                     ||| 0x09 : u8 |
|                                     ||| 0x0a : u16 |
|                                     ||| 0x0b : u32 |
|                                     ||| 0x0c : u64 |
|                                     ||| 0x0d : u128 |
|                                     ||| 0x0e : scalar |
|                                     ||| 0x0f : signature |
|                                     ||| 0x10 : string |

#### Identifier
| _Name_           | _Length_ | _Type_  | _Description_ |
| -----            |    :--:  | -:      | -             |
| `length`         |     1    |      u8 | Length of the identifier |
| `value`          | variable |   bytes | identifier |


### Input value

#### Address
| _Name_           | _Length_ | _Type_  | _Description_ |
| -----            |    :--:  | -:      | -             |
| `value`          |       32 |   field | address converted in field |

#### Field
| _Name_           | _Length_ | _Type_  | _Description_ |
| -----            |    :--:  | -:      | -             |
| `value`          |       32 |   field | field value |

#### U64
| _Name_           | _Length_ | _Type_  | _Description_ |
| -----            |    :--:  | -:      | -             |
| `value`          |        8 |     u64 | int coded in 64 bits (little endian) |

#### U128
| _Name_           | _Length_ | _Type_  | _Description_ |
| -----            |    :--:  | -:      | -             |
| `value`          |        8 |    u128 | int coded in 128 bits (little endian) |

#### Record
| _Name_           | _Length_ | _Type_   | _Description_ |
| -----            |    :--:  | -:       | -             |
| `commitment`     |       32 |   field  | pre computed [commitment](https://github.com/ProvableHQ/snarkVM/blob/staging/console/program/src/request/sign.rs#L186) of the record |
| `h.x`            |       32 |   field  | pre computed [H generator](https://github.com/ProvableHQ/snarkVM/blob/staging/console/program/src/request/sign.rs#L189) x coordinate |
| `h.y`            |       32 |   field  | pre computed [H generator](https://github.com/ProvableHQ/snarkVM/blob/staging/console/program/src/request/sign.rs#L189) y coordinate |

#### External record
| _Name_           | _Length_ | _Type_   | _Description_ |
| -----            |    :--:  | -:       | -             |
| `record_fields`  |   N × 32 |  field[] | `record.to_fields()` — N little-endian 256-bit field elements pre-computed by the host from the record plaintext ([snarkVM ref](https://github.com/ProvableHQ/snarkVM/blob/staging/console/program/src/request/sign.rs#L156)). N must satisfy N × 32 ≤ 128 (`INPUT_VALUE_MAX_LEN`). |

### Input example

#### Public plaintext literal address
Type:
| _Name_           | _Length_ | _Type_  | _Description_ |
| -----            |    :--:  | -:      | -             |
| `tag`            |        2 |   bytes | 0x81 0xb9 |
| `length`         |        1 |      u8 | 0x03 |
| `value type`     |        1 |      u8 | 0x01 : Public |
| `plaintext type` |        1 |      u8 | 0x00 : Literal |
| `literal type`   |        1 |      u8 | 0x00 : Address |

Value
| _Name_           | _Length_ | _Type_  | _Description_ |
| -----            |    :--:  | -:      | -             |
| `tag`            |        2 |   bytes | 0x81 0xb8 |
| `length`         |        1 |      u8 | 0x20 |
| `value`          |       32 |   field | address converted in field |

```shell
address   : aleo1sfydt6z6cnqjx3hcgk9ajw03ecj6uqlfcm9u3p3gdhckzcc2w5xqv3v3pe
field_hex : 8248D5E85AC4C12346F8458BD939F1CE25AE03E9C6CBC886286DF161630A750C
```

#### Public plaintext literal field
Type:
| _Name_           | _Length_ | _Type_  | _Description_ |
| -----            |    :--:  | -:      | -             |
| `tag`            |        2 |   bytes | 0x81 0xb9 |
| `length`         |        1 |      u8 | 0x03 |
| `value type`     |        1 |      u8 | 0x01 : Public |
| `plaintext type` |        1 |      u8 | 0x00 : Literal |
| `literal type`   |        1 |      u8 | 0x02 : Field |

Value
| _Name_           | _Length_ | _Type_  | _Description_ |
| -----            |    :--:  | -:      | -             |
| `tag`            |        2 |   bytes | 0x81 0xb8 |
| `length`         |        1 |      u8 | 0x20 |
| `value`          |       32 |   field | field |

```shell
field_decimal : 7266375125414209082394925781071362722506946030314916664133746682226945366259field
field_hex     : f318210b176031b8ab190743243a66f3a93c69af9f9c2b7946777915619f1010
```

#### Public plaintext literal U64
Type:
| _Name_           | _Length_ | _Type_  | _Description_ |
| -----            |    :--:  | -:      | -             |
| `tag`            |        2 |   bytes | 0x81 0xb9 |
| `length`         |        1 |      u8 | 0x03 |
| `value type`     |        1 |      u8 | 0x01 : Public |
| `plaintext type` |        1 |      u8 | 0x00 : Literal |
| `literal type`   |        1 |      u8 | 0x0c : U64 |

Value
| _Name_           | _Length_ | _Type_  | _Description_ |
| -----            |    :--:  | -:      | -             |
| `tag`            |        2 |   bytes | 0x81 0xb8 |
| `length`         |        1 |      u8 | 0x08 |
| `value`          |        8 |     u64 | u64 (big endian) |

```shell
u64     : 500
u64_hex : f401000000000000
```

#### Public plaintext literal U128
Type:
| _Name_           | _Length_ | _Type_  | _Description_ |
| -----            |    :--:  | -:      | -             |
| `tag`            |        2 |   bytes | 0x81 0xb9 |
| `length`         |        1 |      u8 | 0x03 |
| `value type`     |        1 |      u8 | 0x01 : Public |
| `plaintext type` |        1 |      u8 | 0x00 : Literal |
| `literal type`   |        1 |      u8 | 0x0d : U128 |

Value
| _Name_           | _Length_ | _Type_  | _Description_ |
| -----            |    :--:  | -:      | -             |
| `tag`            |        2 |   bytes | 0x81 0xb8 |
| `length`         |        1 |      u8 | 0x10 |
| `value`          |       16 |    u128 | u128 (big endian) |

```shell
u128     : 500
u128_hex : f4010000000000000000000000000000
```

#### Record
Type:
| _Name_             | _Length_ | _Type_  | _Description_ |
| -----              |    :--:  | -:      | -             |
| `tag`              |        2 |   bytes | 0x81 0xb9 |
| `length`           |        1 |      u8 | 0x03 |
| `value type`       |        1 |      u8 | 0x03 : Record |
| `identifier type`  |        1 |      u8 | length : Identifier length |
| `identifier value` |        1 |   bytes | length bytes |

```shell
record     : type 'microcredits'
record_hex : 63726564697473 ('credits')
```

Value
| _Name_           | _Length_ | _Type_   | _Description_ |
| -----            |    :--:  | -:       | -             |
| `tag`            |        2 |   bytes  | 0x81 0xb8 |
| `length`         |        1 |      u8  | 0x60 |
| `commitment`     |       32 |   field  | pre computed [commitment](https://github.com/ProvableHQ/snarkVM/blob/staging/console/program/src/request/sign.rs#L186) of the record |
| `h.x`            |       32 |   field  | pre computed [H generator](https://github.com/ProvableHQ/snarkVM/blob/staging/console/program/src/request/sign.rs#L189) x coordinate |
| `h.y`            |       32 |   field  | pre computed [H generator](https://github.com/ProvableHQ/snarkVM/blob/staging/console/program/src/request/sign.rs#L189) y coordinate |

```shell
commitment_decimal : 863610808258624400420097447175628057252915910178381065669036707326826591539field
commitment_hex     : 33351bec87c509195035d5aab478b19728bfbc8974acb7a1de46ca3751c9e801
h_decimal : [2426895214035216932245297778850989035038538961658726507442215877484415082794field, 220642863446832956019507279394572297489712696240584424406852292692897199577field]
h_hex     : [2ae118b52d46d60b96630672733d252ca93fbb8d56d5261c0c4cbb8cf2925d05, d975643e43245c7af28db2a85b5459b4b77b382a0c74330c1d537aa922e17c00]
value_hex : 33351bec87c509195035d5aab478b19728bfbc8974acb7a1de46ca3751c9e8012ae118b52d46d60b96630672733d252ca93fbb8d56d5261c0c4cbb8cf2925d05d975643e43245c7af28db2a85b5459b4b77b382a0c74330c1d537aa922e17c00
```

### Request Signature
Serialized TLV data:

| _Name_              | _Tag_ | _Length_ | _Type_  | _Description_ |
| -----               | :---: |    :--:  | -:      | -             |
| `structure_type`    |  0x01 |      1   |     u8  | The structure type (0x2a) |
| `structure_version` |  0x02 |      1   |     u8  | The structure format version (0x01) |
| `signature`         |  0x15 |    128   | [signature](#signature) | The signature |
| `tvk`               |  0xbf |     32   |   field | Transition View Key |
| `tpk`               |  0xc0 |     32   |   field | Transition Public Key |
| `gammas_count`      |  0xc1 |      1   |      u8 | The number of gamma values. Must match the `record_commitments_count` of the request |
| `gamma`             |  0xc2 |     32   |   group | Gamma value (must be present `gammas_count` time) |


### Signature

| _Name_      | _Length_ | _Type_  | _Description_ |
| -----       |    :--:  | -:      | -             |
| `challenge` |     32   |  scalar | The signature's challenge |
| `response`  |     32   |  scalar | The signature's response |
| `pk_sig`    |     32   |   field | Compute key's signature public key x coordinate |
| `pr_sig`    |     32   |   field | Compute key's signature public randomizer x coordinate |

---

## GET TVK

### Overview

The `GET_TVK` command pre-computes Transition View Keys (TVKs) before initiating a signing flow.
This allows the host to obtain TVKs for building transaction data (e.g. encrypting records) prior to submitting the sign transaction request.

The command operates in two modes:
- **Seed mode** (`P1=0x00`): Derives the account from the BIP32 path, initializes the internal `r_list`, computes `r[0]`, and returns the corresponding TVK. This must be called first.
- **Derived mode** (`P1=0x01`): Computes `r[index]` from the previously seeded `r_list` and returns the corresponding TVK. Indices must be requested sequentially (1, 2, 3, ...).

The `r_list` has a maximum capacity of 32 entries and expires after 5 seconds of inactivity.

The device must be in `SIGNING_STATE_WAIT_INTENT` state; otherwise the command is rejected.

### GET TVK SEED

#### Description

_Initializes the r\_list from a BIP32 derivation path, computes r\[0\], and returns its TVK._

#### Command

| _CLA_ | _INS_ | _P1_ | _P2_ |   _Lc_   | _CData_  |
| ----- | :---: | ---: | ---- | :------: | -------: |
| E0    | 08    | 00   | 00   | variable | variable |

#### Input data (CData)

| _Description_                                    | _Length_ | _Type_ |
| ------------------------------------------------ | :------: |  ----: |
| Number of BIP 32 derivations to perform (max 10) |    1     |     u8 |
| First derivation index (big endian)               |    4     |    u32 |
| ...                                              |    4     |    u32 |
| Last derivation index (big endian)               |    4     |    u32 |

#### Output data (RData)

| _Description_ | _Length_ | _Type_ |
| ------------- | :------: |  ----: |
| TVK length    |    1     |     u8 |
| TVK           |   32     |  field |

### GET TVK DERIVED

#### Description

_Computes r\[index\] from the previously seeded r\_list and returns its TVK. The index must equal the current r\_list count (sequential access only)._

#### Command

| _CLA_ | _INS_ | _P1_ | _P2_ |   _Lc_   | _CData_  |
| ----- | :---: | ---: | ---- | :------: | -------: |
| E0    | 08    | 01   | 00   | variable | variable |

#### Input data (CData)

| _Description_                                    | _Length_ | _Type_ |
| ------------------------------------------------ | :------: |  ----: |
| Number of BIP 32 derivations to perform (max 10) |    1     |     u8 |
| First derivation index (big endian)               |    4     |    u32 |
| ...                                              |    4     |    u32 |
| Last derivation index (big endian)               |    4     |    u32 |
| r\_list index                                    |    1     |     u8 |

#### Output data (RData)

| _Description_ | _Length_ | _Type_ |
| ------------- | :------: |  ----: |
| TVK length    |    1     |     u8 |
| TVK           |   32     |  field |

### Status words

| _SW_   | _Description_ |
| ------ | ------------- |
| 0x9000 | Success |
| 0x6985 | Conditions not satisfied (wrong signing state) |
| 0x6A80 | Incorrect data (invalid index, r\_list not initialized, computation error) |
| 0x6A86 | Incorrect P1/P2 (P1 is neither 0x00 nor 0x01) |
| 0x6A87 | Wrong data length (empty data or malformed BIP32 path) |
| 0xB001 | BIP32 path derivation failure |