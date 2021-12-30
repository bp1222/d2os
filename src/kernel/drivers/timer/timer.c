#include <stdint.h>

#include <kernel/interrupt.h>
#include <kernel/scheduler.h>
#include <kernel/drivers/timer/timer.h>
#include <kernel/drivers/uart/uart.h>
#include <kernel/utils/printk.h>

static volatile timer_registers_t *timer_reg = (timer_registers_t *)TIMER_BASE;

static void timer_process(irq_value_t irq)
{
    switch (irq)
    {
    case INTERRUPT_TIMER0:
        schedule();
        break;
    case INTERRUPT_TIMER1:
        printk("\tTimer1\n");
        timer_set(INTERRUPT_TIMER1, 3000000);
        break;
    default:
        break;
    }
}

static void timer_clearer(irq_value_t irq)
{
    switch (irq)
    {
    case INTERRUPT_TIMER0:
        timer_reg->control.timer0_matched = 1;
        break;
    case INTERRUPT_TIMER1:
        timer_reg->control.timer1_matched = 1;
        break;
    default:
        break;
    }
}

void timer_init(irq_value_t irq)
{
    interrupt_register(irq, (interrupt_handlers_t){timer_process, timer_clearer});
}

void timer_set(irq_value_t irq, uint32_t usec)
{
    switch (irq)
    {
    case INTERRUPT_TIMER0:
        timer_reg->timer0 = timer_reg->counter_low + usec;
        break;
    case INTERRUPT_TIMER1:
        timer_reg->timer1 = timer_reg->counter_low + usec;
        break;
    default:
        break;
    }
}