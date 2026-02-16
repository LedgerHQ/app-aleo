# Aleo application : Get account information APDU specification

## Overview

This document describes the APDU messages interface to get account address & view account view key.

Note that the BIP32 path should use the mandatory format : m/44'/683'/${account_index}

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
| Number of BIP 32 derivations to perform (max 10) |    1     |     u8 |
| First derivation index (big endian)              |    4     |    u32 |
| ...                                              |    4     |    u32 |
| Last derivation index (big endian)               |    4     |    u32 |

##### Output data (RData)

| _Description_  | _Length_ | _Type_ |
| -------------- | :------: |  ----: |
| Address length |     1    |     u8 |
| Address        | variable |  bytes |

#### Example

Getting account address for account number 0:
```shell
> e005010015058000002c800002ab800000000000000000000000
< 3f616c656f316d77393832647a6c6773737372367a6d75686d383575796576767063657672326a677861736b76386e36647570686e6a39677273636a77346a6d9000
Address length : 63
Address        : aleo1mw982dzlgsssr6zmuhm85uyevvpcevr2jgxaskv8n6duphnj9grscjw4jm
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
| Number of BIP 32 derivations to perform (max 10) |    1     |     u8 |
| First derivation index (big endian)              |    4     |    u32 |
| ...                                              |    4     |    u32 |
| Last derivation index (big endian)               |    4     |    u32 |

##### Output data (RData)

| _Description_  | _Length_ | _Type_ |
| -------------- | :------: |  ----: |
| Address length |     1    |     u8 |
| Address        | variable |  bytes |

#### Example

Getting account view key for account number 0:
```shell
> e007010015058000002c80000001800000000000000000000000
< 3541566965774b65793171654b4a316e6f634b704536746231637073745a6533427a774770646939616a7350504363533263547952489000
Address length : 53
Address        : AViewKey1qeKJ1nocKpE6tb1cpstZe3BzwGpdi9ajsPPCcS2cTyRH
```
