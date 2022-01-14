#ifndef __STRING_H__
#define __STRING_H__

void memset(void*, uint8_t, uint32_t);
void *memcpy(void *dest, const void *src, uint32_t n);
uint32_t strlen (const char *str);

#endif