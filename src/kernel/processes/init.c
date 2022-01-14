#include <stdint.h>

#include <kernel/memory.h>
#include <kernel/kernel.h>
#include <kernel/smp.h>
#include <kernel/utils/printk.h>
#include <kernel/processes/process.h>

void init_cb()
{
    int i = 0;

    register int sp asm("sp");
    while (1)
    {
        printk("INIT PROCESS %d 0x%x\n", i++, sp);
        asm volatile ("wfi");
        if (i > 5) {
            return;
        }
    }
}

process_t *init_userspace()
{
    void *stack = kmalloc(2048, USER_MEMORY);

    process_t *p = create_process("init", (stack_conf_t){stack, 2048});

    p->text = &init_cb;
    p->textsize = 4;

    p->registers.spsr = 0x50;
    p->registers.pc = (uint32_t)init_cb;

    p->state = PROCESS_PENDING;

    print_process(p);

    return p;
}
