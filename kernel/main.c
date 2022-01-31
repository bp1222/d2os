#include <stdint.h>

#include <kernel/kernel.h>

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
#include <kernel/utils/printk.h>
*/

void __attribute__((noreturn)) kernel_main(const char *cmdline)
{
    //interrupt_init();

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

    process_t *p = idle_task(CPU_0);
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
        ;
}

void delay(int count)
{
    asm volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n"
                 : "=r"(count)
                 : [count] "0"(count)
                 : "cc");
}