#pragma once

#include <stddef.h>   // size_t
#include <stdbool.h>  // bool
#include <stdint.h>   // uint*_t

#include "buffer.h"

#include "types.h"

// Extraction
int tx_extract_prepared_request(const buffer_t *cdata, prepared_request_t *prepared_request);
int tx_extract_intent(buffer_t *cdata);

// Parsing
int tx_parse(sign_transaction_datas_t *data, tx_t *tx);
