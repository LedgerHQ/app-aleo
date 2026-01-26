#pragma once

#include <stdint.h>   // uint*_t
#include <stddef.h>   // size_t
#include <stdbool.h>  // bool

#include "types.h"
#include "account.h"

int sign_prepared_request(account_t *account, prepared_request_t *request);

