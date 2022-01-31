#include <stdint.h>

#include <kernel/kernel.h>

#include <kernel/drivers/gpio/bcm_2835_gpio.h>
#include <kernel/drivers/interrupt/bcm_2835_interrupt.h>
#include <kernel/drivers/serial/bcm_2835_uart.h>

#include "include/peripheral.h"
#include "boards/bcm_2835.h"
#include "atags/atags.h"

void __attribute__((noreturn)) arch_kernel_main(uint32_t r0, uint32_t r1, uint32_t r2)
{
    atags_detect((uint32_t *)r2);

    init_bcm_2835_gpio(PERIPHERAL_BASE);
    init_bcm_2835_interrupt(PERIPHERAL_BASE);
    init_bcm_2835_uart(PERIPHERAL_BASE, UART0_INTERRUPT);

    kernel_main(atags_get_cmdline());
}

void arch_enable_interrupts() {
    asm volatile("cpsie i");
}

void arch_disable_interrupts() {
    asm volatile("cpsid i");
}
