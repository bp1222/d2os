#ifndef __GPIO_H__
#define __GPIO_H__
#include <stdint.h>

typedef uint8_t gpio_pin_t;
typedef volatile uint32_t gpio_reg_t;

typedef enum
{
    GPIO_INPUT,
    GPIO_OUTPUT,
} gpio_direction_t;

typedef enum
{
    GPIO_PULL_DISABLE,
    GPIO_PULL_DOWN,
    GPIO_PULL_UP,
} gpio_pull_t;

typedef enum
{
    GPIO_EDGE_RISING,
    GPIO_EDGE_FALLING,
} gpio_edge_t;

typedef struct {
    void (*set_direction)(gpio_pin_t, gpio_direction_t);
    void (*set_pull)(gpio_pin_t, gpio_pull_t);
    void (*set_edge)(gpio_pin_t, gpio_edge_t);
} kernel_gpio_device_t;

void gpio_init(kernel_gpio_device_t *dev);

void gpio_set_direction(gpio_pin_t pin, gpio_direction_t direction);
void gpio_set_pull(gpio_pin_t pin, gpio_pull_t pull);
void gpio_set_edge(gpio_pin_t pin, gpio_edge_t edge);

#endif