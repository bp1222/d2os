#include <stdint.h>

#include <kernel/interrupt.h>
#include <kernel/scheduler.h>
#include <kernel/drivers/timer/timer.h>
#include <kernel/drivers/uart/uart.h>

static volatile timer_registers_t *timer_reg = (timer_registers_t*)TIMER_BASE;

static void timer_process(irq_value_t irq) {
    schedule();
}

static void timer_clearer(irq_value_t irq) {
    switch (irq) {
        case INTERRUPT_TIMER0:
            timer_reg->control.timer0_matched = 1;
            break;
    }
}

void timer_init() {
    interrupt_register(0, (interrupt_handlers_t){timer_process, timer_clearer});
}

void timer_set(uint32_t usec) {
    timer_reg->timer0 = timer_reg->counter_low + usec;
}