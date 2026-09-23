# Aleo application : Get account information APDU specification

## Overview

This document describes the APDU messages interface to get account address & view account view key.

Note that the BIP32 path must use the mandatory format : `m/44'/683'/${account_index}'/0'`
(exactly 4 derivations, all hardened). Any other path is rejected with `0x6A87` (wrong data length).

### GET ACCOUNT ADDRESS

#### Description

_This commands return the account address generated from a given BIP 32 path_
_The address is returned as an ascii bech32m encoded representation (aleo1.....)_

##### Command without user consent

| _CLA_ | _INS_ | _P1_ | _P2_ |   _Lc_   | _CData_  |
| ----- | :---: | ---: | ---- | :------: | -------: |
| 0xe0  | 0x05  | 0x00 | 0x00 | variable | variable |

##### Command with user consent

| _CLA_ | _INS_ | _P1_ | _P2_ |   _Lc_   | _CData_  |
| ----- | :---: | ---: | ---- | :------: | -------: |
| 0xe0  | 0x05  | 0x01 | 0x00 | variable | variable |

##### Input data (CData)

| _Description_                                    | _Length_ | _Type_ |
| ------------------------------------------------ | :------: |  ----: |
| Number of BIP 32 derivations (must be 4)         |    1     |     u8 |
| Purpose: 44' (`0x8000002C`, big endian)          |    4     |    u32 |
| Coin type: 683' (`0x800002AB`, big endian)       |    4     |    u32 |
| Account index, hardened (big endian)             |    4     |    u32 |
| Change: 0' (`0x80000000`, big endian)            |    4     |    u32 |

##### Output data (RData)

| _Description_  | _Length_ | _Type_ |
| -------------- | :------: |  ----: |
| Address length |     1    |     u8 |
| Address        | variable |  bytes |

#### Example

Getting account address for account number 0 (`m/44'/683'/0'/0'`):
```shell
> e005010011048000002c800002ab8000000080000000
< 3f616c656f316b373334396e616b78373268337133676d387a67366630646b736e6b70397039686139336c7278367a7532676e766a7a707972736a32386a35349000
Address length : 63
Address        : aleo1k7349nakx72h3q3gm8zg6f0dksnkp9p9ha93lrx6zu2gnvjzpyrsj28j54
```

### GET ACCOUNT VIEW KEY

#### Description

_This commands return the account view key generated from a given BIP 32 path_
_The view key is returned as an ascii base58 encoded representation (AViewKey...)_
_The user consent is mandatory here_

##### Command

| _CLA_ | _INS_ | _P1_ | _P2_ |   _Lc_   | _CData_  |
| ----- | :---: | ---: | ---- | :------: | -------: |
| 0xe0  | 0x07  | 0x01 | 0x00 | variable | variable |

##### Input data (CData)

| _Description_                                    | _Length_ | _Type_ |
| ------------------------------------------------ | :------: |  ----: |
| Number of BIP 32 derivations (must be 4)         |    1     |     u8 |
| Purpose: 44' (`0x8000002C`, big endian)          |    4     |    u32 |
| Coin type: 683' (`0x800002AB`, big endian)       |    4     |    u32 |
| Account index, hardened (big endian)             |    4     |    u32 |
| Change: 0' (`0x80000000`, big endian)            |    4     |    u32 |

##### Output data (RData)

| _Description_  | _Length_ | _Type_ |
| -------------- | :------: |  ----: |
| Address length |     1    |     u8 |
| Address        | variable |  bytes |

#### Example

Getting account view key for account number 0 (`m/44'/683'/0'/0'`):
```shell
> e007010011048000002c800002ab8000000080000000
< 3541566965774b657931666e584474444a7a315672386852465861375a78775741333745335458394d72514a656936393167534a6b419000
Address length : 53
Address        : AViewKey1fnXDtDJz1Vr8hRFXa7ZxwWA37E3TX9MrQJei691gSJkA
```
