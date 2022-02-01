#ifndef __SERIAL_H__
#define __SERIAL_H__
#include <stdint.h>

typedef struct {
    void (*write)(const char*, uint32_t);
} kernel_serial_device_t;

void serial_init(kernel_serial_device_t *dev);
void kernel_write(void *p, char);

#endif