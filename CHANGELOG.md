# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.3.3] - 2026-09-01

### Added

- ARC20 tokens support: ETH, SOL, WBTC and USDT
- ARC20 tokens batch private transfer support
- Logic to forbid test/temporary programs on mainnet
- Unit tests, Ragger tests and swap tests for ARC20 tokens and batch transfers

### Fixed

- ARC20 batch transfer parsing out-of-band issue
- Out-of-bounds issue with `network_id`
- Unit tests after rebasing
- Ruff format issues and some CodeQL findings

## [1.3.1] - 2026-09-01

### Fixed

- Fee timeout blocking for touch devices

## [1.3.0] - 2026-08-28

### Fixed

- V-063: View key remains in global UI context after confirmation or rejection
- V-180: Precomputed Schnorr nonces retained in global `r_list` after use
- V-205: Malformed nested APDUs can keep a user approval alive indefinitely
- V-041: Key derivation and signing leave secret intermediates uncleared on stack
- V-036: Private scalar material left on stack in `group_scalar_multiply`
- V-152: Out-of-bounds write in public plaintext hashing for struct/array inputs
- V-123: Sensitive signing intermediates persist in static buffers after successful operations
- V-186: Terminal swap return skips erasure of account private keys and signing nonces
- V-135: `GET_TVK` leaves derived account private keys in global context after `r_list` expiry
- V-125: Poseidon global sponge retains secret-derived hash state across APDU calls
- V-118: Signing state advances after failed transaction signature in review callback
- V-117: Fee prepared requests can be signed through nested-call APDU without fee-limit validation
- Audit-2-3-5: Overlapping root `SIGN_TRANSACTION` APDUs
- Audit-6: Fee prepared requests could be signed through a nested-call APDU; root tx processing is now blocked when not in the right state, and a 15s delay is enforced before the next nested call
- Audit-4: Premature execution signature release before fee finalization
- Ruff format issues

## [1.2.1] - 2026-08-06

### Added

- Swap feature support (Exchange app integration)
- Swap unit tests and Ragger tests, including tests for token swaps

### Fixed

- Device getting stuck on the loading transaction screen when no fees are present
- Fuzzing test issue
- Clang format issues
- Build issue after rebasing with `develop` branch
- Ragger swap test for the Fake token
- Ragger tests vanished during merge

### Changed

- CI: added swap tests

## [1.2.0] - 2026-08-04

### Fixed

- Audit-5: Allow pre-generated TVK only for private batch transfers
- Audit-8: Internal state not cleared before the end of the stackframe
- Audit-9: Missing error handling
- Audit-10: `hash_input` static variable not zeroed immediately after use
- Audit-11: Local variables not cleared on successful return path
- Audit-12: Repeated error processing in `sign_root_tx`
- Ruff linting issues
- Minor fixes

### Changed

- Migrate Python linting/formatting tooling to ruff

## [1.1.1] - 2026-06-26

### Added

- Support of static ARC22 tokens USAD & USDCx
- Token private batch transfer support for USAD & USDCx
- Plaintext struct & array input support
- Plaintext literal U128 input support
- Automatic generation of static tokens
- Unit tests & Ragger tests for new features

### Fixed

- Poseidon hash for large output
- Handle large private inputs (increased constants, removed poseidon output length arbitrary limit)
- U128 format implementation
- CI issues

## [1.1.0] - 2026-06-23

### Added

- Private batch transfer for ALEO token (based on smart contracts)
- Smart contract program's function support
- BHP1024 hash generator
- Unified database for BHP1024 function hash & parsing
- External record hashing support (`hash_external_record_input`)
- GET_TVK APDU support
- `r_list` management
- `account_erase` function
- Smart contract Ragger tests
- Unit tests for batch transactions and external records

### Fixed

- V-003: Out-of-bounds reads while signing unvalidated transaction inputs
- V-005/V-093: Out-of-bounds input array access from unbounded nested-call `inputs_count`
- V-011: `tx_extract_intent` fail-opens when nested prepared-request parsing fails
- V-013/V-024: Off-by-one buffers
- V-017: Fee cap display can overflow and understate approved fees
- V-027: Transaction validation sends a signature response even when signing fails
- V-028: Fragmentation buffer pointer advanced permanently, causing later overflows
- V-030: Root signing mode auto-signs fee requests without user confirmation
- V-033: Out-of-bounds read when bit-encoding short transaction input values
- V-038: Unchecked record inputs reach out-of-bounds signature hashing
- V-085: Host-controlled `r_hint` in nested SIGN TRANSACTION leaks signing private key
- V-087: Fragmented SIGN_TRANSACTION does not bind continuation APDUs to initial signing mode
- V-090: Out-of-bounds gamma writes when signing more than 8 record inputs
- V-101: Misleading approval text for batch private-to-public Aleo transfers
- ALEO-#01: Transaction signing fee function name can overflow
- Misleading default success initialization
- Missing parameter specification
- Missing header guard
- Magic numbers
- Use `r_hint` only for non-root requests
- Check consistency for fee program/function with the intent
- Wrong display progression with nested calls
- Clang format issues
- Get TVK review: domain name, nonce checks, index for Rx computation
- Update private batch transfer program name (switch to mainnet)

### Changed

- Increase signature message length
- Increase Rx transaction buffer
- Update function database with new smart contracts

## [1.0.1] - 2026-04-24

### Changed

- First delivery with basic ALEO account, send & receive support
