#pragma once

#include "os.h"
#include "macros.h"

/**
 * Length of public key.
 */
#define PUBKEY_LEN (MEMBER_SIZE(pubkey_ctx_t, raw_public_key))
/**
 * Length of chain code.
 */
#define CHAINCODE_LEN (MEMBER_SIZE(pubkey_ctx_t, chain_code))

/**
 * Helper to send APDU response with address.
 *
 * response = ADDRESS_LEN (1) ||
 *            G_context.address (ADDRESS_LEN)
 *
 * @return zero or positive integer if success, -1 otherwise.
 *
 */
int helper_send_response_get_address(void);

/**
 * Helper to send APDU response with view key.
 *
 * response = VIEW_KEY_LEN (1) ||
 *            G_context.view_key (VIEW_KEY_LEN)
 *
 * @return zero or positive integer if success, -1 otherwise.
 *
 */
int helper_send_response_get_view_key(void);

/**
 * Helper to send APDU response with private key. For dev purpose only!
 *
 * response = PRIVATE_KEY_LEN (1) ||
 *            G_context.private_key (PRIVATE_KEY_LEN)
 *
 * @return zero or positive integer if success, -1 otherwise.
 *
 */
int helper_send_response_get_private_key(void);

int helper_send_response_sign_transaction(void);
