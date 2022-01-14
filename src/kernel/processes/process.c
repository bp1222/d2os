#include <stdint.h>
#include <stddef.h>

#include <kernel/memory.h>
#include <kernel/mutex.h>
#include <kernel/processes/process.h>
#include <kernel/utils/printk.h>
#include <kernel/utils/string.h>

process_t *first_process;
process_t *last_process;

static mutex_t process_mutex;

void print_process(process_t *p)
{
    printk("Process %s (0x%x)\n"
           "\t Stackbottom: 0x%x, Stacktop: 0x%x, Stacksize: 0x%x\n"
           "\t Registers:\n",
           p->name, p, p->stack, (uint32_t)p->stack + p->stacksize, p->stacksize, p->registers.sp);

    printk("\t\tpc   = 0x%x\n", p->registers.pc);
    printk("\t\tspsr = 0x%x\n", p->registers.spsr);
    printk("\t\tsp   = 0x%x\n", p->registers.sp);
    printk("\t\tlr   = 0x%x\n", p->registers.lr);
    for (int i = 12; i >= 0; i--)
    {
        printk("\t\tr%d = 0x%x\n", i, p->registers.r[i]);
    }
}

process_t *create_process(const char *name, stack_conf_t stack_conf)
{
    process_t *proc = kmalloc(sizeof(process_t), KERNEL_MEMORY);

    proc->state = PROCESS_INIT;
    proc->name = kmalloc(strlen(name), KERNEL_MEMORY);
    memcpy(proc->name, name, strlen(name));

    proc->stack = stack_conf.stack;
    proc->stacksize = stack_conf.size;
    for (int i = 12; i >= 0; i--)
    {
        proc->registers.r[i] = 0xfeedbee0 + i;
    }
    proc->registers.sp = (uint32_t)proc->stack;
    proc->registers.lr = (uint32_t)reap;

    mutex_acquire(&process_mutex);
    if (last_process)
    {
        last_process->next = proc;
        last_process = proc;
    }

    if (!first_process)
    {
        first_process = proc;
        last_process = proc;
    }
    mutex_release(&process_mutex);

    return proc;
}
