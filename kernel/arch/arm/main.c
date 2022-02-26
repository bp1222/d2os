#include <stdint.h>

#include <kernel/kernel.h>
#include <kernel/process/task.h>
#include <kernel/mm/memory.h>

#include <kernel/drivers/gpio/bcm_2835_gpio.h>
#include <kernel/drivers/interrupt/bcm_2835_interrupt.h>
#include <kernel/drivers/interrupt/bcm_2836_interrupt.h>
#include <kernel/drivers/timer/bcm_2835_timer.h>
#include <kernel/drivers/serial/bcm_2835_uart.h>

#include <arch/peripheral.h>

#include "boards/bcm_2835.h"
#include "atags/atags.h"

void arch_enable_interrupts()
{
    asm volatile("cpsie i");
}

void arch_disable_interrupts()
{
    asm volatile("cpsid i");
}

kernel_interrupt_device_t *interrupt_manager;
kernel_interrupt_device_t *interrupt_manager2;

void __attribute__((noreturn)) arch_kernel_main(uint32_t r0, uint32_t r1, uint32_t r2)
{
    atags_detect((uint32_t *)r2);

    interrupt_manager2 = init_bcm_2836_interrupt(PERIPHERAL_BASE);
    interrupt_manager = init_bcm_2835_interrupt(PERIPHERAL_BASE);

    // init_bcm_2836_clock();
    init_bcm_2835_gpio(PERIPHERAL_BASE);
    init_bcm_2835_timer(PERIPHERAL_BASE, INTERRUPT_TIMER3);
    init_bcm_2835_uart(PERIPHERAL_BASE, INTERRUPT_UART0);

    kernel_main(atags_get_cmdline());
}

#include <kernel/utils/printk.h>

void arch_kernel_irq_handler(void *ctx)
{
    irq_value_t irq;
    kernel_task_registers_t *r = ctx;

    irq = interrupt_manager2->get_irq(ctx);

    while ((irq = interrupt_manager->get_irq(ctx)) > 0)
    {
        kernel_irq_handler(irq, ctx);
    }
}

void print_sp(uint32_t sp)
{
    printk("\tSP: 0x%x\n");
}

uint32_t arch_get_cpu(void)
{
    uint32_t reg = 0;
    asm volatile("mrc p15, 0, %0, c0, c0, 5" ::"r"(reg));
    return reg & 0x3;
}

void __inline__ arch_isb()
{
    asm volatile("isb" ::
                     : "memory");
}

void __inline__ arch_dsb()
{
    asm volatile("dsb" ::
                     : "memory");
}

void __inline__ arch_dmb()
{
    asm volatile("dmb" ::
                     : "memory");
}

#include <kernel/utils/printk.h>

void arch_create_kernel_task(kernel_task_t *task, void *fn, void *arg)
{
    printk("ret_from_fk: 0x%x\n", return_from_fork);
    task->context.lr = (uint32_t)return_from_fork;
    task->context.sp = (uint32_t)task->stack_top;
    task->context.r10 = (uint32_t)fn;  // r10 = fn
    task->context.r11 = (uint32_t)arg; // r11 = arg
}

void __attribute__((interrupt("UNDEF"))) _undefined_instruction_handler(void)
{
    printk("UNDEFINED at \n\r");
    while (1)
        ;
}

void __attribute__((interrupt("SWI"))) _software_interrupt_handler(void)
{
    printk("SWI at \n\r");
    while (1)
        ;
}

void __attribute__((interrupt("ABORT"))) _prefetch_abort_handler(void)
{
    printk("PREFETCH at \n\r");
    while (1)
        ;
}

void __attribute__((interrupt("ABORT"))) _data_abort_handler(void)
{
    printk("MEMORY ABORT\n\r");
    while (1)
        ;
}

void __attribute__((interrupt("UNUSED"))) _unused_handler(void)
{
    printk("UNUSED at \n\r");
    while (1)
        ;
}

void __attribute__((interrupt("FIQ"))) _fiq_handler(void)
{
    printk("FIQ at \n\r");
    while (1)
        ;
}