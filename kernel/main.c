#include <stdint.h>
#include <stddef.h>

#include <kernel/kernel.h>
#include <kernel/interrupt.h>
#include <kernel/mm/memory.h>
#include <kernel/drivers/serial.h>
#include <kernel/process/schedule.h>
#include <kernel/process/task.h>
#include <kernel/utils/printk.h>

/*
#include <kernel/interrupt.h>
#include <kernel/memory.h>
#include <kernel/mmu.h>
#include <kernel/peripheral.h>
#include <kernel/smp.h>
#include <kernel/boot/atags.h>
#include <kernel/drivers/ps2/ps2_keyboard.h>
#include <kernel/drivers/uart/uart.h>
#include <kernel/drivers/timer/timer.h>
#include <kernel/processes/process.h>
#include <kernel/processes/scheduler.h>
*/

void idle_task();

extern uint32_t *__end_kernel;
extern uint32_t *__svc_stack_size;
extern uint32_t *__svc_stack_top_core0;

void __attribute__((noreturn)) kernel_main(const char *cmdline)
{
    init_printk(NULL, kernel_write);
    printk("Welcome to D2os!\n\r\n\r");
    printk("SVC Stack: 0x%x (Len: 0x%x)\n", &__svc_stack_top_core0, &__svc_stack_size);
    memory_init();
    enable_interrupts();

    create_kernel_task(idle_task, NULL);

    schedule();

    /*
    uart_init();

    printk("Welcome to D2os!\n\r\n\r");
    printk("Boot Args: r0 = %x, r1 = %x, atags = %x\n", (uint32_t *)r0, (uint32_t *)r1, (uint32_t *)atags);
    atags_dump((uint32_t *)atags);

    memory_init();

    mmu_init();

    vfp_init();

    timer_init(INTERRUPT_TIMER1);

    smp_boot();

    init_userspace();
    exec_process(p);

    int y = 0;
    while (1)
    {
        printk("NOPE 0x%x\n", y++);
        asm volatile("wfi");
    }
    */

    while (1)
    {
        volatile register uint32_t sp asm("sp");
        printk("Loop: 0x%x\n", sp);
        asm volatile("wfi");
    }
}

void delay(int count)
{
    asm volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n"
                 : "=r"(count)
                 : [count] "0"(count)
                 : "cc");
}