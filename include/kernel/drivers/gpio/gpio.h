#ifndef __GPIO_H__
#define __GPIO_H__

#define GPIO_BASE 0x20200000

#define GPFSEL0   (GPIO_BASE)
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

#define GPEDS0    (GPIO_BASE + 0x40)
#define GPEDS1    (GPIO_BASE + 0x44)

#define GPFEN0    (GPIO_BASE + 0x58)
#define GPFEN1    (GPIO_BASE + 0x5C)

#define GPLEN0    (GPIO_BASE + 0x70)
#define GPLEN1    (GPIO_BASE + 0x74)

#define GPPUD     (GPIO_BASE + 0x94)
#define GPPUDCLK0 (GPIO_BASE + 0x98)
#define GPPUDCLK1 (GPIO_BASE + 0x9C)

#define GPIO_REG(r)    *(volatile uint32_t*)r
#define GPIO_REG_PTR(r) (volatile uint32_t*)r

#define GET_GPFSEL(reg, pin) {                  \
    switch(pin / 10) {                          \
        case 0:                                 \
            reg = GPIO_REG_PTR(GPFSEL0);        \
            break;                              \
        case 1:                                 \
            reg = GPIO_REG_PTR(GPFSEL1);        \
            break;                              \
        case 2:                                 \
            reg = GPIO_REG_PTR(GPFSEL2);        \
            break;                              \
    }                                           \
}

#define SET_INPUT_PIN(pin) {                    \
    volatile uint32_t *reg;                     \
    GET_GPFSEL(reg, pin);                       \
    *reg |= (0b000 << ((pin % 10) * 3));        \
}

#define SET_OUTPUT_PIN(pin) {                   \
    volatile uint32_t *reg;                     \
    GET_GPFSEL(reg, pin);                       \
    *reg |= (0b001 << ((pin % 10) * 3));        \
}


#define PULL_DISABLE 0x00000000
#define PULL_DOWN    0x00000001
#define PULL_UP      0x00000002

#define SET_PIN_PULL(type, pin) {               \
    GPIO_REG(GPPUD) = type;                     \
    delay(150);                                 \
    GPIO_REG(GPPUDCLK0) |= (1 << pin);          \
    delay(150);                                 \
    GPIO_REG(GPPUD) = 0x00000000;               \
    GPIO_REG(GPPUDCLK0) = 0x00000000;           \
}

#endif
