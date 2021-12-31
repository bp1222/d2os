#include <stdint.h>
#include <stddef.h>

#include <kernel/kernel.h>
#include <kernel/drivers/gpio/gpio.h>

#define GET_GPFSEL(pin) (pin < 10 ? GPFSEL0 : (pin < 20 ? GPFSEL1 : (pin < 30 ? GPFSEL2 : (pin < 40 ? GPFSEL3 : (pin < 50 ? GPFSEL4 : GPFSEL5)))))
#define GET_GPPUDCLK(pin) (pin < 32 ? GPPUDCLK0 : GPPUDCLK1)

static volatile gpio_registers_t *gpio_reg = (gpio_registers_t *)GPIO_BASE;

static gpio_reg_t *get_gpfsel(gpio_pin_t pin)
{
    switch (pin / 10)
    {
    case 0:
        return &gpio_reg->GPFSEL0;
    case 1:
        return &gpio_reg->GPFSEL1;
    case 2:
        return &gpio_reg->GPFSEL2;
    case 3:
        return &gpio_reg->GPFSEL3;
    case 4:
        return &gpio_reg->GPFSEL4;
    case 5:
        return &gpio_reg->GPFSEL5;
    }

    return NULL;
}

static gpio_reg_t *get_gppudclk(gpio_pin_t pin)
{
    return pin < 32 ? &gpio_reg->GPPUDCLK0 : &gpio_reg->GPPUDCLK0;
}

void gpio_set_direction(gpio_pin_t pin, gpio_direction_t direction)
{
    gpio_reg_t *gpfsel = get_gpfsel(pin);

    switch (direction)
    {
    case GPIO_INPUT:
        *gpfsel |= (0b000 << ((pin % 10) * 3));
        break;
    case GPIO_OUTPUT:
        *gpfsel |= (0b001 << ((pin % 10) * 3));
        break;
    }
}

void gpio_set_pull(gpio_pin_t pin, gpio_pull_t pull)
{
    gpio_reg_t *gppudclk = get_gppudclk(pin);

    gpio_reg->GPPUD = pull;
    delay(150);
    *gppudclk |= pin % 32;
    delay(150);

    gpio_reg->GPPUD = 0x0;
    *gppudclk = 0x0;
}

void gpio_set_edge(gpio_pin_t pin, gpio_edge_t edge)
{

}