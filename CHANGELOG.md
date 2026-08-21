# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

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
