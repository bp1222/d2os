#include <stdint.h>

#include "vfp.h"

#include <kernel/interrupt.h>
#include <kernel/kernel.h>
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

void print_mode_stacks() {
    uint32_t mode;

    asm volatile(
        "msr cpsr_c, %[stor]\n"
        "mov %[mode], sp\n"
        : [mode] "=r"(mode)
        : [stor] "r"(0x12));
    asm volatile(
        "msr cpsr_c, %[stor]\n" 
        :
        : [stor] "r"(0x13));

    printk("IRQ Stack: 0x%x\n", mode);

    asm volatile(
        "mov %0, sp\n" :"=r"(mode));

    printk("SVC Stack: 0x%x\n", mode);
}

void print_sp_reg();

void __attribute__((noreturn)) kernel_main(uint32_t r0, uint32_t r1, uint32_t atags)
{
    (void)r0;
    (void)r1;

    /* Read Hardware Info */
    atags_detect((uint32_t *)atags);

    /* Setup interrupts */
    interrupt_init();

    /* Init Serial */
    uart_init();

    printk("Welcome to D2os!\n\r\n\r");
    printk("Boot Args: r0 = %x, r1 = %x, atags = %x\n", (uint32_t *)r0, (uint32_t *)r1, (uint32_t *)atags);
    atags_dump((uint32_t *)atags);
    print_mode_stacks();

    /* Setup memory, start MMU */
    memory_init();

    /* Start MMU */
    //mmu_init();

    /* Setup Floating Point */
    //vfp_init();

    /* Start System Timer */
    timer_init(INTERRUPT_TIMER1);

    /* Start other processors */
    //smp_boot();

    process_t *p = idle_task(CPU_0);
    init_userspace();
    exec_process(p);

    int y = 0;
    while (1)
    {
        
        printk("NOPE 0x%x\n", y++);
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