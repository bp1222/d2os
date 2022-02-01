#include <stdint.h>

#include <kernel/kernel.h>

#include <kernel/drivers/gpio/bcm_2835_gpio.h>
#include <kernel/drivers/interrupt/bcm_2835_interrupt.h>
#include <kernel/drivers/timer/bcm_2835_timer.h>
#include <kernel/drivers/serial/bcm_2835_uart.h>

#include "include/peripheral.h"
#include "boards/bcm_2835.h"
#include "atags/atags.h"

void arch_enable_interrupts() {
    asm volatile("cpsie i");
}

void arch_disable_interrupts() {
    asm volatile("cpsid i");
}

kernel_interrupt_device_t *interrupt_manager;

void __attribute__((noreturn)) arch_kernel_main(uint32_t r0, uint32_t r1, uint32_t r2)
{
    atags_detect((uint32_t *)r2);

    init_bcm_2835_gpio(PERIPHERAL_BASE);
    interrupt_manager = init_bcm_2835_interrupt(PERIPHERAL_BASE);
    init_bcm_2835_timer(PERIPHERAL_BASE, INTERRUPT_TIMER3);
    init_bcm_2835_uart(PERIPHERAL_BASE, INTERRUPT_UART0);

    enable_interrupts();

    kernel_main(atags_get_cmdline());
}

void arch_kernel_irq_handler(void *ctx) {
    irq_value_t irq = interrupt_manager->handle(ctx);
    kernel_irq_handler(irq, ctx);
}
