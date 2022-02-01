#include <kernel/drivers/timer.h>

static kernel_timer_device_t *kernel_timer_device;

void timer_init(kernel_timer_device_t *dev) {
    kernel_timer_device = dev;
}
