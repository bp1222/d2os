#include <stdint.h>
#include <stddef.h>

#include <kernel/interrupt.h>
#include <kernel/kernel.h>
#include <kernel/smp.h>
#include <kernel/drivers/uart/uart.h>
#include <kernel/utils/printk.h>

static volatile irq_registers_t *irq_reg = (irq_registers_t *)INTERRUPT_BASE;

static interrupt_process_t process[NUM_INTERRUPTS] = {0};
static interrupt_clearer_t clearer[NUM_INTERRUPTS] = {0};

void interrupt_init()
{
    irq_reg->disable_1 = 0xFFFFFFFF;     // disable all interrupts
    irq_reg->disable_2 = 0xFFFFFFFF;     // disable all interrupts
    irq_reg->disable_basic = 0xFFFFFFFF; // disable all interrupts
}

void interrupt_unregister(uint32_t irq)
{
    process[irq] = NULL;
    clearer[irq] = NULL;

    if (IRQ_GPU_1(irq))
    {
        irq_reg->disable_1 |= 1 << irq;
    }
    else if (IRQ_GPU_2(irq))
    {
        irq_reg->disable_2 |= 1 << irq;
    }
    else if (IRQ_BASIC(irq))
    {
        irq_reg->disable_basic |= 1 << irq;
    }
}


void interrupt_register(uint32_t irq, interrupt_handlers_t handlers)
{
    process[irq] = handlers.processor;
    clearer[irq] = handlers.clearer;
    if (IRQ_GPU_1(irq))
    {
        irq_reg->enable_1 |= 1 << irq;
    }
    else if (IRQ_GPU_2(irq))
    {
        irq_reg->enable_2 |= 1 << irq;
    }
    else if (IRQ_BASIC(irq))
    {
        irq_reg->enable_basic |= 1 << irq;
    }
}

void __attribute__((interrupt("UNDEF"))) _undefined_instruction_handler(void)
{
    uart_putc('U');
    while (1)
        ;
}
void __attribute__((interrupt("SWI"))) _software_interrupt_handler(void)
{
    uart_putc('S');
    while (1)
        ;
}
void __attribute__((interrupt("ABORT"))) _prefetch_abort_handler(void)
{
    uart_putc('P');
    while (1)
        ;
}
void __attribute__((interrupt("ABORT"))) _data_abort_handler(void)
{
    uart_putc('D');
    while (1)
        ;
}
void __attribute__((interrupt("UNUSED"))) _unused_handler(void)
{
    uart_putc('U');
    while (1)
        ;
}
void __attribute__((interrupt("IRQ"))) _irq_handler(void)
{
    _disable_interrupts();
    printk("Handling on %d\t", smp_get_core());
    for (int i = 0; i < NUM_INTERRUPTS; i++)
    {
        if (irq_reg->pending_1 & (1 << i))
        {
            if (clearer[i])
            {
                clearer[i](i);
            }
            if (process[i])
            {
                process[i](i);
            }
            irq_reg->pending_1 &= ~(1 << i);
        }
    }
    _enable_interrupts();
}

void __attribute__((interrupt("FIQ"))) _fiq_handler(void)
{
    uart_putc('F');
    while (1)
        ;
}