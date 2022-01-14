#include <stdint.h>

#include <kernel/memory.h>
#include <kernel/kernel.h>
#include <kernel/smp.h>
#include <kernel/utils/printk.h>
#include <kernel/processes/process.h>

extern uint32_t __svc_stack_top_core0;
extern uint32_t __svc_stack_top_core1;
extern uint32_t __svc_stack_top_core2;
extern uint32_t __svc_stack_top_core3;

void sub_idle(int *i)
{
    register int sp asm("sp");
    printk("Sub Idle(0x%x): 0x%x\n", sp, (*i)++);
    asm volatile("wfi");
}

void idle_task_cb()
{
    int i = 0;
    register int sp asm("sp");
    while (1)
    {
        printk("Idle(0x%x): 0x%x\n", sp, i++);
        sub_idle(&i);
    }
}

process_t *idle_task (cpu_core_t cpu) {
    process_t *p;
    void *stack;

    switch (cpu) {
        case CPU_0:
            stack = (void*)&__svc_stack_top_core0;
            break;
        case CPU_1:
            stack = (void*)&__svc_stack_top_core1;
            break;
        case CPU_2:
            stack = (void*)&__svc_stack_top_core2;
            break;
        case CPU_3:
            stack = (void*)&__svc_stack_top_core3;
            break;
    }

    p = create_process("idle", (stack_conf_t){stack, 2048});

    p->registers.lr = (uint32_t)panic;
    p->registers.spsr = 0x1F; // Run in privledged mode
    p->registers.pc = (uint32_t)idle_task_cb;

    p->state = PROCESS_PENDING;

    print_process(p);

    set_current_process(cpu, p);
    return p;
}