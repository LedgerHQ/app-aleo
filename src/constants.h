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
 */
#define INPUT_VALUE_MAX_LEN (128)

/**
 * Maximum length of input type.
 */
#define INPUT_TYPE_MAX_LEN (128)

/**
 * OS big number length.
 */
#define BN_LENGTH (32)
