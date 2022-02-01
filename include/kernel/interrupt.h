#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__
#include <stdint.h>

typedef uint32_t irq_value_t;

typedef void (*interrupt_handler_t)(irq_value_t irq, void *ctx);

void set_interrupt_handler(irq_value_t irq, interrupt_handler_t handler);
interrupt_handler_t get_interrupt_handler(irq_value_t irq);
void remove_interrupt_handler(irq_value_t irq);

typedef struct {
    void (*mask)(irq_value_t irq);
    void (*unmask)(irq_value_t irq);
    irq_value_t (*handle)(void *ctx);
} kernel_interrupt_device_t;

void set_kernel_interrupt_device(kernel_interrupt_device_t *manager);

// Entry Point from ASM
void kernel_irq_handler(uint32_t irq, void *ctx);

#define enable_interrupts arch_enable_interrupts
#define disable_interrupts arch_disable_interrupts

#endif
