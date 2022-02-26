#include <stdint.h>
#include <stddef.h>

#include <kernel/drivers/gpio.h>
#include <kernel/drivers/gpio/bcm_2835_gpio.h>

static volatile gpio_registers_t *gpio_reg;

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

static void set_direction(gpio_pin_t pin, gpio_direction_t direction)
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

static void set_pull(gpio_pin_t pin, gpio_pull_t pull)
{
    gpio_reg_t *gppudclk = get_gppudclk(pin);

    gpio_reg->GPPUD = pull;
    delay(150);

    *gppudclk |= pin % 32;
    delay(150);

    gpio_reg->GPPUD = 0x0;
    *gppudclk = 0x0;
}

static void set_edge(gpio_pin_t pin, gpio_edge_t edge)
{
}

static kernel_gpio_device_t gpio_device = {
    .set_direction = set_direction,
    .set_pull = set_pull,
    .set_edge = set_edge,
};

void init_bcm_2835_gpio(uint32_t peripheral_base)
{
    gpio_reg = (gpio_registers_t *)(peripheral_base + GPIO_OFFSET);
    gpio_init(&gpio_device);
}
