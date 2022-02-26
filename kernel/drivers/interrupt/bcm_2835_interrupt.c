#include <stddef.h>
#include <stdint.h>

#include <kernel/interrupt.h>
#include <kernel/drivers/interrupt/bcm_2835_interrupt.h>

static volatile irq_registers_t *irq_reg;

static void bcm2835_interrupt_init()
{
    irq_reg->disable_1 = 0xFFFFFFFF;     // mask all interrupts
    irq_reg->disable_2 = 0xFFFFFFFF;     // mask all interrupts
    irq_reg->disable_basic = 0xFFFFFFFF; // mask all interrupts
    enable_interrupts();
}

static void bcm2835_interrupt_mask(uint32_t irq)
{
    if (IRQ_GPU_1(irq))
    {
        irq_reg->disable_1 |= 1 << irq;
    }
    else if (IRQ_GPU_2(irq))
    {
        irq_reg->disable_2 |= 1 << irq % 32;
    }
}

static void bcm2835_interrupt_unmask(uint32_t irq)
{
    if (IRQ_GPU_1(irq))
    {
        irq_reg->enable_1 |= 1 << irq;
    }
    else if (IRQ_GPU_2(irq))
    {
        irq_reg->enable_2 |= 1 << irq % 32;
    }
}

static irq_value_t bcm2835_interrupt_get_irq(void *ctx)
{
    uint8_t irq;
    for (irq = 0; irq < NUM_INTERRUPTS; irq++)
    {
        uint8_t irq_bit = irq % 32;

        if (IRQ_GPU_1(irq) && (irq_reg->pending_1 & (1 << irq_bit)))
        {
            irq_reg->pending_1 &= ~(1 << irq_bit);
            return irq;
        }
        else if (IRQ_GPU_2(irq) && (irq_reg->pending_2 & (1 << irq_bit)))
        {
            irq_reg->pending_2 &= ~(1 << irq_bit);
            return irq;
        }
    }

    return 0;
}

static kernel_interrupt_device_t interrupt_manager __attribute__((__section__("__interrupt_devices"))) = {
    .init = bcm2835_interrupt_init,
    .mask = bcm2835_interrupt_mask,
    .unmask = bcm2835_interrupt_unmask,
    .get_irq = bcm2835_interrupt_get_irq,
};

kernel_interrupt_device_t *init_bcm_2835_interrupt(uint32_t base)
{
    irq_reg = (irq_registers_t *)(base + INTERRUPT_OFFSET);
    set_kernel_interrupt_device(&interrupt_manager);

    return &interrupt_manager;
}