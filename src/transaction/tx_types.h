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
} plaintext_type_literal_e;