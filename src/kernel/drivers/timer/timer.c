#include <stdint.h>

#include <kernel/interrupt.h>
#include <kernel/memory.h>
#include <kernel/drivers/timer/timer.h>
#include <kernel/drivers/uart/uart.h>
#include <kernel/processes/scheduler.h>
#include <kernel/utils/printk.h>

static volatile timer_registers_t *timer_reg = (timer_registers_t *)TIMER_BASE;
#define SCHEDULER_DELAY 1000000

static void timer_interrupt_handler(irq_value_t irq)
{
    switch (irq)
    {
    case INTERRUPT_TIMER1:
        timer_reg->control.timer1_matched = 1;
        timer_reg->timer1 = timer_reg->counter_low + SCHEDULER_DELAY;
        schedule();
        break;
    default:
        break;
    }
}

void timer_init(irq_value_t irq)
{
    switch (irq)
    {
    case INTERRUPT_TIMER1:
        timer_reg->timer1 = timer_reg->counter_low + SCHEDULER_DELAY;
        interrupt_register(irq, timer_interrupt_handler);
        break;
    default:
        printk("timer_init: unsupported timer");
        break;
    }
}