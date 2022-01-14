#include <stdint.h>
#include <stddef.h>

#include <kernel/interrupt.h>
#include <kernel/kernel.h>
#include <kernel/smp.h>
#include <kernel/drivers/uart/uart.h>
#include <kernel/utils/printk.h>

#include <kernel/processes/process.h>

static volatile irq_registers_t *irq_reg = (irq_registers_t *)INTERRUPT_BASE;
static interrupt_handler_t handlers[NUM_INTERRUPTS] = {0};
registers_t irq_current_registers = {0};

void enable_interrupts() {
    asm volatile("cpsie i");
}

void disable_interrupts() {
    asm volatile("cpsid i");
}

void interrupt_init()
{
    irq_reg->disable_1 = 0xFFFFFFFF;     // disable all interrupts
    irq_reg->disable_2 = 0xFFFFFFFF;     // disable all interrupts
    irq_reg->disable_basic = 0xFFFFFFFF; // disable all interrupts
}

void interrupt_unregister(uint32_t irq)
{
    handlers[irq] = NULL;

    if (IRQ_GPU_1(irq))
    {
        irq_reg->disable_1 |= 1 << irq;
    }
    else if (IRQ_GPU_2(irq))
    {
        irq_reg->disable_2 |= 1 << irq % 32;
    }
}

void interrupt_register(uint32_t irq, interrupt_handler_t handler)
{
    handlers[irq] = handler;

    if (IRQ_GPU_1(irq))
    {
        irq_reg->enable_1 |= 1 << irq;
    }
    else if (IRQ_GPU_2(irq))
    {
        irq_reg->enable_2 |= 1 << irq % 32;
    }
}

void __attribute__((interrupt("UNDEF"))) _undefined_instruction_handler(void)
{
    printk("UNDEFINED at \n\r");
    while (1)
        ;
}
void __attribute__((interrupt("SWI"))) _software_interrupt_handler(void)
{
    printk("SWI at \n\r");
    while (1)
        ;
}
void __attribute__((interrupt("ABORT"))) _prefetch_abort_handler(void)
{
    printk("PREFETCH at \n\r");
    while (1)
        ;
}
void __attribute__((interrupt("ABORT"))) _data_abort_handler(void)
{
    printk("MEMORY ABORT\n\r");
    while (1)
        ;
}
void __attribute__((interrupt("UNUSED"))) _unused_handler(void)
{
    printk("UNUSED at \n\r");
    while (1)
        ;
}
void _irq_handler(void)
{
    for (uint8_t irq = 0; irq < NUM_INTERRUPTS; irq++)
    {
        uint8_t irq_bit = irq % 32;

        if (irq_reg->pending_1 & (1 << irq_bit))
        {
            if (handlers[irq])
            {
                handlers[irq](irq);
            }
            irq_reg->pending_1 &= ~(1 << irq_bit);
        }
        else if (irq_reg->pending_2 & (1 << irq_bit))
        {
            if (handlers[irq])
            {
                handlers[irq](irq);
            }
            irq_reg->pending_2 &= ~(1 << irq_bit);
        }
    }
}

void __attribute__((interrupt("FIQ"))) _fiq_handler(void)
{
    printk("FIQ at \n\r");
    while (1)
        ;
}