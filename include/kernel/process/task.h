#ifndef __TASK_H__
#define __TASK_H__
#include <stddef.h>

#include <arch/task.h>

typedef enum
{
    TASK_INIT = 0,
    TASK_PENDING = 1,
    TASK_RUNNING = 2,
    TASK_EXITING = 3,
    TASK_EXITED = 4,
} kernel_task_state_t;

typedef struct _kernel_task kernel_task_t;

struct _kernel_task
{
    cpu_context_t context;

    kernel_task_state_t state;

    void *stack_top;
    uint32_t stacksize;

    char *name;

    uint32_t pid;

    uint32_t preempt;
    int32_t counter;
    int32_t priority;

    kernel_task_t *next;
};

extern kernel_task_t *first_task;
extern kernel_task_t *last_task;

void create_kernel_task(void *fn, void *arg);
void arch_create_kernel_task(kernel_task_t *task, void *fn, void *arg);

#define INIT_TASK                    \
    {                                \
        .context = INIT_CPU_CONTEXT, \
        .state = TASK_INIT,          \
        .stack_top = 0,              \
        .stacksize = 0,              \
        .name = "init",              \
        .pid = 0,                    \
        .next = NULL                 \
    }

#endif