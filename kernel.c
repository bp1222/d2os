#include <include/kernel.h>
#include <include/printf.h>
#include <drivers/uart/uart.h>
#include <drivers/gpio/gpio.h>

extern void printf_putc (void* p, char c)
{
    uart_putc(c);
}

void set_led(uint32_t state)
{
    if (state) {
        gpio_set_register_bits(GPSET0, (1<<24));
    } else {
        gpio_set_register_bits(GPCLR0, (1<<24));
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

    // Set GPIO 24 for output
    gpio_set_register_bits(GPFSEL2, (0 << 14) | (0 << 13) | (1 << 12));

    // Set our button and led pins
    gpio_set_register(GPPUD, (1<<1));
    delay(150);

    gpio_set_register(GPPUDCLK0, (1<<23));
    delay(150);

    gpio_set_register(GPPUD, 0x00000000);
    gpio_set_register(GPPUDCLK0, 0x00000000);

    while (1) {
        if (!gpio_get_register_bit(GPLEV0, 23)) {
            set_led(1);
        } else {
            set_led(0);
        }
    }

    while (1)
        ;
}
