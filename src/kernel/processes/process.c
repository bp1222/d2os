#include <stdint.h>
#include <stddef.h>

#include <kernel/memory.h>
#include <kernel/mutex.h>
#include <kernel/processes/process.h>
#include <kernel/utils/printk.h>

process_t *first_process;
process_t *last_process;

static mutex_t process_mutex;

uint32_t strlen (const char *str)
{
    return (*str) ? strlen(++str) + 1 : 0;
}

void memcpy(void *dest, const void* src, int bytes) {
    char *d = dest;
    const char *s = src;
    while (bytes--) {
        *d++ = *s++;
    }
}

void print_process(process_t *p) {
    printk("Process %s (0x%x)\n"
    "\t Stackbottom: 0x%x, Stacktop: 0x%x, Stacksize: 0x%x\n"
    "\t Registers:\n",
    p->name, p, p->stack, (uint32_t)p->stack + p->stacksize, p->stacksize, p->registers->sp);

    for (int i = 0; i < 12; i++)
    {
        printk("\t\tr%d = 0x%x\n", i, *(((uint32_t*)&p->registers->r0) + i));
    }
    printk("\t\tsp = 0x%x\n", p->registers->sp);
    printk("\t\tlr = 0x%x\n", p->registers->lr);
}

process_t *create_process(const char *name, stack_conf_t stack_conf)
{
    process_t *proc = kmalloc(sizeof(process_t), KERNEL_MEMORY);

    proc->state = PROCESS_INIT;
    char *n = kmalloc(strlen(name), KERNEL_MEMORY);
    memcpy(n, name, strlen(name));
    proc->name = n;

    proc->stack = stack_conf.stack;
    proc->stacksize = stack_conf.size;
    proc->registers = (registers_t*)((uint32_t)proc->stack + proc->stacksize - sizeof(registers_t));

    for (int i = 0; i < 15; i++)
    {
       *(((uint32_t*)&proc->registers->r0) + i) = 0xfeedbee0 + i;
    }

    proc->registers->sp = (uint32_t)proc->registers;


    mutex_acquire(&process_mutex);
    if (last_process) {
        last_process->next = proc;
        last_process = proc;
    }

    if (!first_process) {
        first_process = proc;
        last_process = proc;
    }
    mutex_release(&process_mutex);

    return proc;
}