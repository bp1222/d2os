#include <stdint.h>
#include <stddef.h>

#include <kernel/kernel.h>
#include <kernel/interrupt.h>
#include <kernel/drivers/timer.h>
#include <kernel/drivers/timer/bcm_2835_timer.h>
#include <kernel/process/schedule.h>
#include <kernel/utils/printk.h>

static volatile timer_registers_t *system_timer;

#define TICK 100000

void schedule_next_timer(uint32_t time)
{
    system_timer->timer3 = system_timer->counter_low + time;
}

void timer_interrupt_handler(irq_value_t irq, void *ctx)
{
    if (system_timer->control.timer3_matched)
    {
        system_timer->control.timer3_matched = 1;
        schedule_next_timer(TICK);
        printk("TICK\n");
        //schedule();
    }
}

static kernel_timer_device_t timer_device = {
    NULL,
};

void init_bcm_2835_timer(uint32_t base, irq_value_t irq)
{
    system_timer = (timer_registers_t *)(base + TIMER_OFFSET);

    timer_init(&timer_device);
    set_interrupt_handler(irq, timer_interrupt_handler);

    schedule_next_timer(TICK);
}
