# Technical Specification

## About

This documentation describes the APDU messages interface to communicate with the Aleo application.

The application covers the following functionalities :

- Retrieve the Aleo app version
- Retrieve the Aleo app name
- Get an Aleo address given a BIP 32 path
- Get the view key given a BIP 32 path
- Sign transactions given a BIP 32 path

The application interface can be accessed over HID or BLE

## APDUs

### GET APP VERSION

#### Description

This command returns Aleo application version

#### Coding

#### Command

| _CLA_ | _INS_ | _P1_ | _P2_ |   _Lc_   | _CData_  |
| ----- | :---: | ---: | ---- | :------: | -------: |
| E0    | 03    | 00   | 00   |    00    |    --    |

##### Input data

None

##### Output data

| _Description_               | _Length_ | _Type_ |
| -------------------------   | :------: |  ----: |
| Application major version   |    1     |    u8  |
| Application minor version   |    1     |    u8  |
| Application patch version   |    1     |    u8  |


### GET APP NAME

#### Description

This command returns Aleo application name

#### Coding

##### Command

| _CLA_ | _INS_ | _P1_ | _P2_ |   _Lc_   | _CData_  |
| ----- | :---: | ---: | ---- | :------: | -------: |
| E0    | 04    | 00   | 00   |    00    |    --    |

##### Input data

None

##### Output data

| _Description_               | _Length_ | _Type_ |
| -------------------------   | :------: |  ----: |
| Application name            | variable | char*  |


### GET ACCOUNT ADDRESS

[Description](doc/ACCOUNT.md#get-account-address)

### GET ACCOUNT VIEW KEY

[Description](doc/ACCOUNT.md#get-account-view-key)

### SIGN TRANSACTION

[Description](doc/SIGN_TRANSACTION.md)

## Supported credits.aleo functions for clear-signing

The device performs clear-signed review screens only for the following
`credits.aleo` functions.  Any other function name causes the signing flow to
return `SWO_INCORRECT_DATA`.

### Transfer functions

| Function name               | Review subtitle                              | Fields shown on device                  |
| --------------------------- | -------------------------------------------- | --------------------------------------- |
| `transfer_public`           | "Public transfer"                            | Amount, To (address), Fees              |
| `transfer_private`          | "Private transfer"                           | Amount, To (address), Fees              |
| `transfer_public_to_private`| "Transfer from public to private address"    | Amount, To (address), Fees              |
| `transfer_private_to_public`| "Transfer from private to public address"    | Amount, To (address), Fees              |

### Fee functions (auto-approved, no interactive review screen)

| Function name  | Behaviour                                            |
| -------------- | ---------------------------------------------------- |
| `fee_public`   | Automatically signed; no screen shown to the user.  |
| `fee_private`  | Automatically signed; no screen shown to the user.  |

### Staking functions

These three functions map to `TX_STAKING` and display the "Review staking
transaction?" screen.

| Function name          | Review subtitle | Fields shown on device                                                |
| ---------------------- | --------------- | --------------------------------------------------------------------- |
| `bond_public`          | "Stake ALEO"    | Validator (address), Payout to / withdrawal (address), Amount, Fees  |
| `unbond_public`        | "Unstake ALEO"  | Staker (address), Amount, Fees                                        |
| `claim_unbond_public`  | "Claim ALEO"    | Staker (address), Fees  *(no Amount field — claim has no amount)*     |

All amounts are displayed in ALEO (microcredits converted by
`EXPONENT_SMALLEST_UNIT` = 6 decimal places).  Fees shown are
`max_base_fee + max_priority_fee`.

### Other functions recognised by the BHP-1024 parameter table

`split` and `join` are present in `bhp_1024_parameters.c` for function-ID
verification purposes but are not dispatched to a review screen by the current
firmware; they return `SWO_INCORRECT_DATA` if presented as a root transaction.

## STATUS WORDS

The following standard Status Words are returned for all APDUs.

| SW       | SW name                        | Description                                                |
| ---      | ---                            | ---                                                        |
|   0x6985   | SWO_CONDITIONS_NOT_SATISFIED | The app is not in the right state to execute the cmd       |
|   0x69f0   | SWO_PERMISSION_DENIED        | Rejected by user                                           |
|   0x6a80   | SWO_INCORRECT_DATA           | Given data is incorrect                                    |
|   0x6a84   | SWO_INSUFFICIENT_MEMORY      | The cmd length is exceeding the internal buffer max length |
|   0x6a86   | SWO_INCORRECT_P1_P2          | Either P1 or P2 is incorrect                               |
|   0x6a87   | SWO_WRONG_DATA_LENGTH        | Lc or minimum APDU length is incorrect                     |
|   0x6d00   | SWO_INVALID_INS              | No command exists with INS                                 |
|   0x6e00   | SWO_INVALID_CLA              | Bad CLA used for this application                          |
|   0x9000   | SWO_SUCCESS                  | Success                                                    |
|   0xb001   | SW_DISPLAY_BIP32_PATH_FAIL   | BIP32 path conversion to string failed                     |
