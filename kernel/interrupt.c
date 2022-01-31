#include <stdint.h>
#include <stddef.h>

#include <kernel/interrupt.h>
#include <kernel/kernel.h>

static kernel_interrupt_manager_t *interrupt_manager = NULL;
static interrupt_handler_t handlers[128];

void set_interrupt_handler(irq_value_t irq, interrupt_handler_t handler)
{
    handlers[irq] = handler;
    interrupt_manager->unmask(irq);
}

interrupt_handler_t get_interrupt_handler(irq_value_t irq) {
    return handlers[irq];
}

void remove_interrupt_handler(irq_value_t irq)
{
    handlers[irq] = NULL;
    interrupt_manager->mask(irq);
}

void set_kernel_interrupt_manager(kernel_interrupt_manager_t *manager) {
    interrupt_manager = manager;
}

void kernel_irq_handler(void *ctx) {
    if (interrupt_manager == NULL) {
        panic();
    }

    //uint32_t irq = interrupt_manager(ctx);
}