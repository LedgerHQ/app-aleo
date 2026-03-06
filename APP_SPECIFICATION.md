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

## STATUS WORDS

The following standard Status Words are returned for all APDUs.

| SW       | SW name                        | Description                                                |
| ---      | ---                            | ---                                                        |
|   0x6985   | SWO_CONDITIONS_NOT_SATISFIED | The app is not in the right state to execute the cmd       |
|   0x69f0   | SWO_PERMISSION_DENIED        | Rejected by user                                           |
|   0x6a80   | SWO_INCORRECT_DATA           | Given data is incorrect                                    |
|   0x6a84   | SWO_INSUFFICIENT_MEMORY      | The cmd lentgh is exceeding the internel buffer max length |
|   0x6a86   | SWO_INCORRECT_P1_P2          | Either P1 or P2 is incorrect                               |
|   0x6a87   | SWO_WRONG_DATA_LENGTH        | Lc or minimum APDU length is incorrect                     |
|   0x6d00   | SWO_INVALID_INS              | No command exists with INS                                 |
|   0x6e00   | SWO_INVALID_CLA              | Bad CLA used for this application                          |
|   0x9000   | SWO_SUCCESS                  | Success                                                    |
|   0xb001   | SW_DISPLAY_BIP32_PATH_FAIL   | BIP32 path conversion to string failed                     |
