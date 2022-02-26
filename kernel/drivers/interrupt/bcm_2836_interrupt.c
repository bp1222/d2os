#include <stdint.h>
#include <kernel/interrupt.h>
#include <kernel/drivers/interrupt/bcm_2836_interrupt.h>

static int32_t interrupt_base = 0;

static void init()
{
    enable_interrupts();
}

static void mask(uint32_t irq)
{
}

static void unmask(uint32_t irq)
{
}

static irq_value_t get_irq(void *ctx)
{
    return 0;
}

static kernel_interrupt_device_t interrupt_manager __attribute__((__section__("__interrupt_devices"))) = {
    .init = init,
    .mask = mask,
    .unmask = unmask,
    .get_irq = get_irq,
};

kernel_interrupt_device_t *init_bcm_2836_interrupt(uint32_t base)
{
    interrupt_base = base;

    set_kernel_interrupt_device(&interrupt_manager);

    return &interrupt_manager;
}