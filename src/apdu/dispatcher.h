#pragma once

#include "parser.h"
#include "types.h"

/**
 * Parameter 1 for APDU get without confirmation.
 */
#define P1_GET_WITHOUT_CONFIRMATION (0x00)

/**
 * Parameter 1 for APDU get with confirmation.
 */
#define P1_GET_WITH_CONFIRMATION (0x01)

/**
 * Parameter 1 for APDU sign mode root (intent).
 */
#define P1_SIGN_MODE_ROOT SIGN_MODE_ROOT

/**
 * Parameter 1 for APDU sign mode nester call (following a root).
 */
#define P1_SIGN_MODE_NESTED_CALL SIGN_MODE_NESTED_CALL

/**
 * Parameter 1 for APDU sign mode fee.
 */
#define P1_SIGN_MODE_FEE SIGN_MODE_FEE

/**
 * Parameter 2 for APDU begin chunk.
 */
#define P2_BEGIN (0x00)
/**
 * Parameter 2 for APDU next chunk.
 */
#define P2_CONTINUE (0x01)

/**
 * Parameter 1 for APDU get root tvk.
 */
#define P1_GET_TVK_SEED R_LIST_MODE_TVK_SEED

/**
 * Parameter 1 for APDU get nested call & fee tvk.
 */
#define P1_GET_TVK_DERIVED R_LIST_MODE_TVK_DERIVED

/**
 * Dispatch APDU command received to the right handler.
 *
 * @param[in] cmd
 *   Structured APDU command (CLA, INS, P1, P2, Lc, Command data).
 *
 * @return zero or positive integer if success, negative integer otherwise.
 *
 */
int apdu_dispatcher(const command_t *cmd);
