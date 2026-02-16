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
