#include <stdint.h>

#include <kernel/drivers/timer/timer.h>
#include <kernel/utils/printk.h>

#define SCHEDULER_DELAY 1000000

void schedule() {
    printk("Scheduling\n\r");
    timer_set(SCHEDULER_DELAY);
}