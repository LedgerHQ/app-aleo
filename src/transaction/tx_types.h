#pragma once

#include <stddef.h>   // size_t
#include <stdbool.h>  // bool
#include <stdint.h>   // uint*_t

typedef enum {
    INPUT_ID_CONSTANT        = 0,
    INPUT_ID_PUBLIC          = 1,
    INPUT_ID_PRIVATE         = 2,
    INPUT_ID_RECORD          = 3,
    INPUT_ID_EXTERNAL_RECORD = 4,
    INPUT_ID_FUTURE          = 5,
    INPUT_ID_DYNAMIC_RECORD  = 6,
} input_id_e;

typedef enum {
    INPUT_VALUE_TYPE_PLAINTEXT_LITERAL = 0,
    INPUT_VALUE_TYPE_PLAINTEXT_STRUCT  = 1,
    INPUT_VALUE_TYPE_PLAINTEXT_ARRAY   = 2,
} input_value_type_e;

typedef enum {
    PLAINTEXT_TYPE_LITERAL_ADDRESS = 0,
    PLAINTEXT_TYPE_LITERAL_FIELD   = 2,
    PLAINTEXT_TYPE_LITERAL_U64     = 12,
    PLAINTEXT_TYPE_LITERAL_U128    = 13,
    // The `identifier` literal (snarkVM `LiteralType::Identifier`): a fixed 31-byte null-padded
    // ASCII name, used by `call.dynamic` programs to select their callee at runtime (the ARC-20
    // generic batcher takes the target token's program name as its first input).
    PLAINTEXT_TYPE_LITERAL_IDENTIFIER = 17,
} plaintext_type_literal_e;

// Size of an `identifier` literal, in bits and bytes. snarkVM's `IdentifierLiteral` is a fixed
// `Field::SIZE_IN_DATA_BITS / 8` = 31-byte null-padded array, hashed over all
// `SIZE_IN_BITS` = 248 bits (byte-derived, unlike address/field which use FIELD_MODULUS_BITS).
#define IDENTIFIER_LITERAL_VALUE_LENGTH (31)
#define IDENTIFIER_LITERAL_BITS         (IDENTIFIER_LITERAL_VALUE_LENGTH * 8)