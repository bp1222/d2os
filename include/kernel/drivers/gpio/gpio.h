#ifndef __GPIO_H__
#define __GPIO_H__
#include <kernel/peripheral.h>

#define GPIO_OFFSET 0x200000
#define GPIO_BASE (PERIPHERAL_BASE + GPIO_OFFSET)

typedef uint8_t gpio_pin_t;
typedef volatile uint32_t gpio_reg_t;

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

typedef enum
{
    GPIO_INPUT = 0,
    GPIO_OUTPUT = 1,
} gpio_direction_t;

typedef enum
{
    GPIO_PULL_DISABLE = 0b00,
    GPIO_PULL_DOWN = 0b01,
    GPIO_PULL_UP = 0b11,
} gpio_pull_t;

typedef enum
{
    GPIO_EDGE_RISING = 0,
    GPIO_EDGE_FALLING = 1,
} gpio_edge_t;

void gpio_set_direction(gpio_pin_t pin, gpio_direction_t direction);
void gpio_set_pull(gpio_pin_t pin, gpio_pull_t pull);
void gpio_set_edge(gpio_pin_t pin, gpio_edge_t edge);

#endif
