#include <include/kernel.h>
#include <include/interrupt.h>
#include <include/printf.h>
#include <drivers/uart/uart.h>
#include <drivers/gpio/gpio.h>

void _enable_interrupts();

volatile irq_registers_t* irq_registers = (irq_registers_t*)(INTERRUPT_BASE + 0x200);

extern void printf_putc (void* p, char c)
{
    uart_putc(c);
}

void toggle_led(uint32_t pin)
{
    if (!(*GPIO_REG(GPLEV0) & (1 << pin))) {
        *GPIO_REG(GPSET0) |= (1 << pin);
    } else {
        *GPIO_REG(GPCLR0) |= (1 << pin);
    }
}

void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags)
{
    (void) r0;
    (void) r1;
    (void) atags;

    init_printf(NULL, printf_putc);
    uart_init();

    printf("Welcome to D2os!\n\r");

    // Set GPIO 25 for output LED
    SET_OUTPUT_PIN(25);

    // Set GPIO 23 for input button
    SET_INPUT_PIN(23);
    SET_PIN_PULL(PULL_UP, 23);
    *GPIO_REG(GPFEN0) |= (1 << 23);
    *GPIO_REG(GPEDS0) |= (1 << 23);

    irq_registers->Enable_IRQs_2 |= (1 << 17);

    _enable_interrupts();

    while (1)
        ;
}

void __attribute__((interrupt ("IRQ"))) irq_handler (void) {
    if (*GPIO_REG(GPEDS0) & (1 << 23)) {
        *GPIO_REG(GPEDS0) |= (1 << 23);
        toggle_led(25);
    }
}
