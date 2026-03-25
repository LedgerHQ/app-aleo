#include <stdint.h>
#include <stddef.h>
#include "globals.h"

global_ctx_t G_context;

static int fuzz_sign_transaction(const uint8_t *data, size_t size)
{
    buffer_t buf = {
        .ptr    = (uint8_t *) data,
        .size   = size,
        .offset = 0,
    };

    sign_transaction_init();
    handler_sign_transaction(&buf, SIGN_MODE_ROOT, false);
    handler_sign_transaction(&buf, SIGN_MODE_ROOT, true);
    handler_sign_transaction(&buf, SIGN_MODE_NESTED_CALL, false);
    handler_sign_transaction(&buf, SIGN_MODE_NESTED_CALL, true);
    handler_sign_transaction(&buf, SIGN_MODE_FEE, false);
    handler_sign_transaction(&buf, SIGN_MODE_FEE, true);
	return 0;
}

/* Main fuzzing handler called by libfuzzer */
int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
    explicit_bzero(&G_context, sizeof(G_context));
    // Run the harness
    fuzz_sign_transaction(data, size);
    return 0;
}
