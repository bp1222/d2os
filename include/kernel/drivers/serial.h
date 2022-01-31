#ifndef __SERIAL_H__
#define __SERIAL_H__

typedef struct {
    void (*write)(const char*, uint32_t);
} kernel_serial_device_t;

void serial_init(kernel_serial_device_t *dev);

#endif