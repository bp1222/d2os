#include <stdint.h>

#include <kernel/interrupt.h>
#include <kernel/kernel.h>
#include <kernel/utils/printk.h>
#include <kernel/drivers/gpio/gpio.h>
#include <kernel/drivers/uart/uart.h>

static void keyboard_interrupt_handler(irq_value_t irq, void *ctx) {
    printk("Handling Keyboard Event");
}

void keyboard_init() {
    gpio_set_direction(23, GPIO_PULL_DISABLE);
    gpio_set_direction(24, GPIO_PULL_DISABLE);

    gpio_set_direction(23, GPIO_INPUT);
    gpio_set_direction(24, GPIO_INPUT);

    gpio_set_edge(23, GPIO_EDGE_FALLING);

    interrupt_register(61, keyboard_interrupt_handler);
}