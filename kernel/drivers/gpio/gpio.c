#include <stdint.h>
#include <kernel/drivers/gpio.h>

static kernel_gpio_device_t *kernel_gpio_device;

void gpio_set_direction(gpio_pin_t pin, gpio_direction_t direction) {
    kernel_gpio_device->set_direction(pin, direction);
}

void gpio_set_pull(gpio_pin_t pin, gpio_pull_t pull) {
    kernel_gpio_device->set_pull(pin, pull);
}

void gpio_set_edge(gpio_pin_t pin, gpio_edge_t edge) {
    kernel_gpio_device->set_edge(pin, edge);
}

void gpio_init(kernel_gpio_device_t *dev) {
    kernel_gpio_device = dev;
}
