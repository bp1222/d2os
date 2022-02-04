#include <kernel/mutex.h>
#include <kernel/mm/memory.h>
#include <kernel/process/schedule.h>
#include <kernel/process/task.h>

extern kernel_task_t init_task;
kernel_task_t *first_task = &init_task;
kernel_task_t *last_task = &init_task;

mutex_t task_mutex;

void create_kernel_task(void *fn, void *arg)
{
    kernel_task_t *task = kmalloc(sizeof(kernel_task_t), KERNEL_MEMORY);

    task->context = INIT_CPU_CONTEXT;
    task->state = TASK_RUNNING;
    task->stack_top = (void *)(task + PAGE_SIZE);
    task->stacksize = PAGE_SIZE - sizeof(kernel_task_t);
    task->pid = 0;
    task->preempt = 1;
    task->counter = 0;
    task->priority = 1;

    arch_create_kernel_task(task, fn, arg);

    disable_preempt();
    mutex_acquire(&task_mutex);
    last_task->next = task;
    last_task = task;
    mutex_release(&task_mutex);
    enable_preempt();
}