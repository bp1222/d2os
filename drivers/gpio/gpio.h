#ifndef __GPIO_H__
#define __GPIO_H__

#define GPIO_BASE 0x20200000

#define GPFSEL0   (GPIO_BASE + 0x00)
#define GPFSEL1   (GPIO_BASE + 0x04)
#define GPFSEL2   (GPIO_BASE + 0x08)
#define GPFSEL3   (GPIO_BASE + 0x0C)
#define GPFSEL4   (GPIO_BASE + 0x10)
#define GPFSEL5   (GPIO_BASE + 0x14)

#define GPSET0    (GPIO_BASE + 0x1C)
#define GPSET1    (GPIO_BASE + 0x20)

#define GPCLR0    (GPIO_BASE + 0x28)
#define GPCLR1    (GPIO_BASE + 0x2C)

#define GPLEV0    (GPIO_BASE + 0x34)

#define GPPUD     (GPIO_BASE + 0x94)
#define GPPUDCLK0 (GPIO_BASE + 0x98)
#define GPPUDCLK1 (GPIO_BASE + 0x9C)

inline void gpio_set_register(uint32_t reg, uint32_t set)
{
    *(volatile uint32_t*)reg = set;
}

inline void gpio_set_register_bits(uint32_t reg, uint32_t set)
{
    *(volatile uint32_t*)reg |= set;
}

inline uint32_t gpio_get_register(uint32_t reg)
{
    return *(volatile uint32_t*)reg;
}

inline uint32_t gpio_get_register_bit(uint32_t reg, uint32_t bit)
{
    return (*(volatile uint32_t*)reg & (1 << bit)) >> bit;
}
#endif
