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

void set_led(uint32_t state)
{
    if (state) {
        gpio_set_register_bit(GPSET0, (1<<24));
    } else {
        gpio_set_register_bit(GPCLR0, (1<<24));
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

    // Set GPIO 24 for output LED
    gpio_set_register_bit(GPFSEL2, (0 << 14) | (0 << 13) | (1 << 12));

    // Set GPIO 23 for input button
    gpio_set_register_bit(GPFSEL2, (0 << 9) | (0 << 10) | (1 << 11));
    gpio_set_register_bit(GPFEN0, (1 << 23));
    gpio_set_register_bit(GPEDS0, (1 << 23));

    irq_registers->Enable_IRQs_2 |= (1 << 17);

    _enable_interrupts();

    while (1)
        ;
    /*
    // Set our button and led pins
    gpio_set_register(GPPUD, (1<<1));
    delay(150);

    gpio_set_register(GPPUDCLK0, (1<<23));
    delay(150);

    gpio_set_register(GPPUD, 0x00000000);
    gpio_set_register(GPPUDCLK0, 0x00000000);

    while (1) {
        if (!gpio_get_register_bit(GPLEV0, 23)) {
        } else {
            set_led(0);
        }
    }
    */
}

void __attribute__ ((interrupt ("IRQ"))) irq_handler (void) {
    if (gpio_get_register(GPEDS0) & (1 << 23)) {
        gpio_set_register_bit(GPEDS0, (1 << 23));
        printf("hello\n\r");
        set_led(!(gpio_get_register(GPLEV0) & (1 << 24)));
    }
}
