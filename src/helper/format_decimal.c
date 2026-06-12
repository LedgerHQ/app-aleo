#include <string.h>

#include "format_decimal.h"

// Worst case: 32 bytes (256 bits) → 78 decimal digits.
#define MAX_DECIMAL_DIGITS (78)

bool format_decimal_le(char *dst, size_t dst_len, const uint8_t *bytes, size_t byte_len)
{
    if (dst == NULL || bytes == NULL) {
        return false;
    }
    if (byte_len == 0 || byte_len > FORMAT_DECIMAL_MAX_BYTES) {
        return false;
    }
    if (dst_len == 0) {
        return false;
    }

    // Reverse into big-endian working buffer for MSB→LSB long division.
    uint8_t work[FORMAT_DECIMAL_MAX_BYTES];
    for (size_t i = 0; i < byte_len; i++) {
        work[i] = bytes[byte_len - 1 - i];
    }

    char   tmp[MAX_DECIMAL_DIGITS];
    size_t tmp_len = 0;

    // Repeated divide-by-10; emit one digit per iteration, LSB-first.
    for (;;) {
        bool all_zero = true;
        for (size_t i = 0; i < byte_len; i++) {
            if (work[i] != 0) {
                all_zero = false;
                break;
            }
        }
        if (all_zero) {
            break;
        }
        uint16_t rem = 0;
        for (size_t i = 0; i < byte_len; i++) {
            uint16_t cur = (uint16_t) ((rem << 8) | work[i]);
            work[i]      = (uint8_t) (cur / 10);
            rem          = (uint16_t) (cur % 10);
        }
        // tmp_len < MAX_DECIMAL_DIGITS is guaranteed because byte_len ≤ 32.
        tmp[tmp_len++] = (char) ('0' + rem);
    }

    if (tmp_len == 0) {
        if (dst_len < 2) {
            return false;
        }
        dst[0] = '0';
        dst[1] = '\0';
        return true;
    }

    if (tmp_len + 1 > dst_len) {
        return false;
    }
    for (size_t i = 0; i < tmp_len; i++) {
        dst[i] = tmp[tmp_len - 1 - i];
    }
    dst[tmp_len] = '\0';
    return true;
}
