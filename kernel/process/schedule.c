#include <stdint.h>

#include <kernel/process/schedule.h>

// PER CPU
kernel_task_t init_task = INIT_TASK;
static kernel_task_t *current_task[1] = {&(init_task)};

__inline__ kernel_task_t *get_current()
{
    return current_task[0];
}

void disable_preempt()
{
    get_current()->preempt++;
}

void enable_preempt()
{
    get_current()->preempt--;
}

void schedule()
{
    kernel_task_t *next = first_task;

    if (get_current()->preempt > 0)
    {
        return;
    }

    disable_preempt();

    while (1)
    {
        int32_t c = -1;

        for (kernel_task_t *t = first_task; t != NULL; t = t->next)
        {
            if (t->state == TASK_RUNNING && t->counter > c)
            {
                c = t->counter;
                next = t;
            }
        }
        if (c)
        {
            break;
        }
        for (kernel_task_t *t = first_task; t != NULL; t = t->next)
        {
            if (t)
            {
                t->counter = (t->counter >> 1) + t->priority;
            }
        }
    }

    if (get_current() != next)
    {
        kernel_task_t *prev = get_current();
        current_task[0] = next;
        arch_switch_task(prev, next);
    }
    enable_preempt();
}