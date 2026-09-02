#pragma once

/**
 * Instruction class of the Aleo application.
 */
#define CLA (0xE0)

/**
 * Length of APPNAME variable in the Makefile.
 */
#define APPNAME_LEN (sizeof(APPNAME) - 1)

/**
 * Maximum length of MAJOR_VERSION || MINOR_VERSION || PATCH_VERSION.
 */
#define APPVERSION_LEN (3)

/**
 * Maximum length of application name.
 */
#define MAX_APPNAME_LEN (64)

/**
 * Maximum transaction length (bytes).
 */
#define MAX_TRANSACTION_LEN (510)

/**
 * Maximum signature length (bytes).
 */
#define MAX_DER_SIG_LEN (72)

/**
 * Exponent used to convert µALEO to ALEO unit (N ALEO = N * 10^6 µALEO).
 */
#define EXPONENT_SMALLEST_UNIT (6)

/**
 * Length of address.
 */
#define ADDRESS_LEN (63)

/**
 * Length of view key.
 */
#define VIEW_KEY_LEN (53)

/**
 * Length of private key.
 */
#define PRIVATE_KEY_LEN (59)

/**
 * Maximum number of inputs for a single signature.
 */
#define MAX_NB_OF_INPUTS (16)

/**
 * Maximum number of records for a single signature.
 */
#define MAX_NB_OF_RECORDS (8)

/**
 * Maximum length of program id name.
 */
#define PROGRAM_ID_NETWORK_MAX_LEN (64)

/**
 * Maximum length of program id name.
 */
#define PROGRAM_ID_NAME_MAX_LEN (64)

/**
 * Maximum length of function name.
 */
#define FUNCTION_NAME_MAX_LEN (64)

/**
 * Maximum length of input value.
 *
 * Sized for the largest input we sign: the stablecoin freeze-list exclusion proof
 * `[MerkleProof; 2u32]`, whose `to_fields()` encoding is 41 fields = 1312 bytes. Set to
 * 48 * 32 = 1536 for headroom, matching PLAINTEXT_FIELDS_MAX_SIZE (signature.c).
 */
#define INPUT_VALUE_MAX_LEN (1536)

/**
 * Maximum length of input type.
 */
#define INPUT_TYPE_MAX_LEN (128)

/**
 * OS big number length.
 */
#define BN_LENGTH (32)

/**
 * Maximum length of the R list used to compute TVK.
 */
#define R_LIST_MAX_LENGTH (32)

/**
 * R_list keep maximum time validity in ms.
 */
#define R_LIST_MAX_VALIDITY_TIME_MS (5 * 1000)

/**
 * Maximum size for amount formatting (39 chars for u128 + 2 for '0.').
 */
#define MAX_AMOUNT_SIZE (41)

#define ALEO_DECIMALS (6)
#define ALEO_TICKER   "ALEO"
