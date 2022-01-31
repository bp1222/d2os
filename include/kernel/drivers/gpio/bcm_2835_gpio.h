#ifndef __BCM_2385_GPIO_H__
#define __BCM_2385_GPIO_H__
#include <kernel/drivers/gpio.h>

#define GPIO_OFFSET 0x200000

#define GET_GPFSEL(pin) (pin < 10 ? GPFSEL0 : (pin < 20 ? GPFSEL1 : (pin < 30 ? GPFSEL2 : (pin < 40 ? GPFSEL3 : (pin < 50 ? GPFSEL4 : GPFSEL5)))))
#define GET_GPPUDCLK(pin) (pin < 32 ? GPPUDCLK0 : GPPUDCLK1)

typedef struct
{
    gpio_reg_t GPFSEL0;
    gpio_reg_t GPFSEL1;
    gpio_reg_t GPFSEL2;
    gpio_reg_t GPFSEL3;
    gpio_reg_t GPFSEL4;
    gpio_reg_t GPFSEL5;
    gpio_reg_t reserved1;
    gpio_reg_t GPSET0;
    gpio_reg_t GPSET1;
    gpio_reg_t reserved2;
    gpio_reg_t GPCLR0;
    gpio_reg_t GPCLR1;
    gpio_reg_t reserved3;
    gpio_reg_t GPLEV0;
    gpio_reg_t GPLEV1;
    gpio_reg_t reserved4;
    gpio_reg_t GPEDS0;
    gpio_reg_t GPEDS1;
    gpio_reg_t reserved5;
    gpio_reg_t GPREN0;
    gpio_reg_t GPREN1;
    gpio_reg_t reserved6;
    gpio_reg_t GPFEN0;
    gpio_reg_t GPFEN1;
    gpio_reg_t reserved7;
    gpio_reg_t GPHEN0;
    gpio_reg_t GPHEN1;
    gpio_reg_t reserved8;
    gpio_reg_t GPLEN0;
    gpio_reg_t GPLEN1;
    gpio_reg_t reserved9;
    gpio_reg_t GPAREN0;
    gpio_reg_t GPAREN1;
    gpio_reg_t reserved10;
    gpio_reg_t GPAFEN0;
    gpio_reg_t GPAFEN1;
    gpio_reg_t reserved11;
    gpio_reg_t GPPUD;
    gpio_reg_t GPPUDCLK0;
    gpio_reg_t GPPUDCLK1;
} gpio_registers_t;

void init_bcm_2835_gpio(uint32_t base);

#endif