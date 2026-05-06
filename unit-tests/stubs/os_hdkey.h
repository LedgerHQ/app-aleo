#pragma once

#include <stdio.h>

#include "os.h"
#include "cx.h"

typedef enum HDKEY_derive_mode_e {
    HDKEY_DERIVE_MODE_NORMAL           = 0x00u,  // BIP32 derivation
    HDKEY_DERIVE_MODE_ED25519_SLIP10   = 0x01u,  // SLIP10 derivation with ED25519
    HDKEY_DERIVE_MODE_SLIP21           = 0x02u,  // SLIP21 derivation
    HDKEY_DERIVE_MODE_RESERVED         = 0x04u,  // Derivation mode reserved to the OS
    HDKEY_DERIVE_MODE_BLS12377_ALEO    = 0x08u,  // BIP32-like derivation with BLS12-377
    HDKEY_DERIVE_MODE_ZIP32_SAPLING    = 0x10u,  // ZIP32 Sapling hardened derivation
    HDKEY_DERIVE_MODE_ZIP32_ORCHARD    = 0x20u,  // ZIP32 Orchard derivation
    HDKEY_DERIVE_MODE_ZIP32_REGISTERED = 0x40u,  // ZIP32 registered derivation
} HDKEY_derive_mode_t;

bolos_err_t sys_hdkey_derive(HDKEY_derive_mode_t derivation_mode,
                             cx_curve_t          curve,
                             const uint32_t     *path,
                             size_t              path_len,
                             uint8_t            *private_key,
                             size_t              private_key_len,
                             uint8_t            *chain_code,
                             size_t              chain_code_len,
                             uint8_t            *seed,
                             size_t              seed_len);
