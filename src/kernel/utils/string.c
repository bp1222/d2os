#include <stdint.h>

#include <kernel/utils/string.h>

/* This is bad, but it works */
void memset(void *dest, uint8_t c, uint32_t l) {
    uint8_t *d = dest;
    while (l--) {
        *d++ = c;
    }
}