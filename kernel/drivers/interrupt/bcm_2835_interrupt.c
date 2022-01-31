#include <stddef.h>
#include <stdint.h>

#include <kernel/interrupt.h>
#include <kernel/drivers/interrupt/bcm_2835_interrupt.h>

static volatile irq_registers_t *irq_reg;

void interrupt_init()
{
    irq_reg->disable_1 = 0xFFFFFFFF;     // disable all interrupts
    irq_reg->disable_2 = 0xFFFFFFFF;     // disable all interrupts
    irq_reg->disable_basic = 0xFFFFFFFF; // disable all interrupts
}

void mask(uint32_t irq)
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

void unmask(uint32_t irq)
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

void handle(void *ctx)
{
    uint8_t irq;
    for (irq = 0; irq < NUM_INTERRUPTS; irq++)
    {
        uint8_t irq_bit = irq % 32;

        if (irq_reg->pending_1 & (1 << irq_bit))
        {
            /*
            if (handlers[irq])
            {
                handlers[irq](irq, ctx);
            }
            */
            irq_reg->pending_1 &= ~(1 << irq_bit);
        }
        else if (irq_reg->pending_2 & (1 << irq_bit))
        {
            /*
            if (handlers[irq])
            {
                handlers[irq](irq, ctx);
            }
            */
            irq_reg->pending_2 &= ~(1 << irq_bit);
        }
    }
}

static kernel_interrupt_manager_t interrupt_manager = {
    .mask = mask,
    .unmask = unmask,
    .handle = handle
};

void init_bcm_2835_interrupt(uint32_t base) {
    irq_reg = (irq_registers_t*)(base + INTERRUPT_OFFSET);
    set_kernel_interrupt_manager(&interrupt_manager);
}