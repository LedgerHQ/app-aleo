/* bytes_to_lowercase_hex — pure utility function used by bulk_order.c.
 * Copied verbatim from $BOLOS_SDK/src/os.c; no hardware dependencies.
 */
#include <stddef.h>
#include <stdint.h>

/* pic/pic_init — hardware PIC relocation stubs.
 * On host, PIC(x) expands to pic((void*)x). Simply return the pointer unchanged.
 */
void *pic(void *linked_address)
{
    return linked_address;
}

void pic_init(void *pic_flash_start, void *pic_ram_start)
{
    (void) pic_flash_start;
    (void) pic_ram_start;
}

void os_sched_exit(int status)
{
    (void) status;
}
