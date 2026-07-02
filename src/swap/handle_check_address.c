#include "swap.h"
#include "buffer.h"
#include "bip32.h"
#include "crypto_helpers.h"
#include "cx.h"
#include "os.h"

#include "types.h"
#include "format.h"
#include "account.h"

#include <string.h>

/* Check that the address used to receive funds is owned by the device
 * check_address_parameters_t is defined in C SDK as:
 * struct {
 *   // IN
 *   uint8_t *coin_configuration;
 *   uint8_t  coin_configuration_length;
 *   // serialized path, segwit, version prefix, hash used, dictionary etc.
 *   // fields and serialization format depends on specific coin app
 *   uint8_t *address_parameters;
 *   uint8_t  address_parameters_length;
 *   char    *address_to_check;
 *   char    *extra_id_to_check;
 *   // OUT
 *   int result;
 * } check_address_parameters_t;
 */
void swap_handle_check_address(check_address_parameters_t *params)
{
    PRINTF("Inside swap_handle_check_address\n");
    params->result = 0;

    if (params->address_parameters == NULL) {
        PRINTF("derivation path expected\n");
        return;
    }
    PRINTF(
        "address_parameters %.*H\n", params->address_parameters_length, params->address_parameters);

    if (params->address_to_check == NULL) {
        PRINTF("Address to check expected\n");
        return;
    }
    PRINTF("Address to check %s\n", params->address_to_check);
    if (strlen(params->address_to_check) != (ADDRESS_LEN)) {
        PRINTF("Address to check expected length %d, not %d\n",
               ADDRESS_LEN,
               strlen(params->address_to_check));
        return;
    }

    buffer_t buf = {
        .ptr = params->address_parameters, .size = params->address_parameters_length, .offset = 0};
    uint8_t  bip32_path_len;
    uint32_t bip32_path[MAX_BIP32_PATH];
    char     address[ADDRESS_LEN + 1] = {0};

    if (!buffer_read_u8(&buf, &bip32_path_len)
        || !buffer_read_bip32_path(&buf, bip32_path, (size_t) bip32_path_len)) {
        return;
    }

    int status = account_get_address_string(bip32_path, bip32_path_len, address);

    if (status < 0) {
        return;
    }

    PRINTF("Derived address %s\n", address);
    PRINTF("Checked address %s\n", params->address_to_check);

    if (strncmp(address, params->address_to_check, sizeof(address)) != 0) {
        PRINTF("Addresses do not match\n");
    }
    else {
        PRINTF("Addresses match\n");
        params->result = 1;
    }
}
