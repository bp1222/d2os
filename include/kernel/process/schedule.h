#ifndef __SCHEDULE_H__
#define __SCHEDULE_H__
#include <kernel/process/task.h>

void scheduler_init();

void schedule();

kernel_task_t *get_current();

void enable_preempt();
void disable_preempt();

void arch_switch_task(kernel_task_t *old_task, kernel_task_t *new_task);

#endif