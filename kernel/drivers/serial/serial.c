#include <stdint.h>
#include <kernel/drivers/serial.h>

static kernel_serial_device_t *kernel_serial_device;

void serial_init(kernel_serial_device_t *dev) {
    kernel_serial_device = dev;
}

void kernel_write(const char *c, uint32_t len) {
    kernel_serial_device->write(c, len);
}
