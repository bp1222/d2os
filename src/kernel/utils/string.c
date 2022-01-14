#include <stdint.h>

#include <kernel/utils/string.h>

/* This is bad, but it works */
void memset(void *dest, uint8_t c, uint32_t l)
{
    uint8_t *d = dest;
    while (l--)
    {
        *d++ = c;
    }
}

void *memcpy(void *dest, const void *src, uint32_t n)
{
    int i;

    char *d = dest;
    const char *s = src;

    for (i = 0; i < n; i++)
    {
        *d = *s;
        d++;
        s++;
    }

    return dest;
}

uint32_t strlen (const char *str)
{
    return (*str) ? strlen(++str) + 1 : 0;
}