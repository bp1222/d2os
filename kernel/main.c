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

    create_kernel_task(idle_task, NULL);

    schedule();

    while (1)
        ;
}

void delay(int count)
{
    asm volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n"
                 : "=r"(count)
                 : [count] "0"(count)
                 : "cc");
}