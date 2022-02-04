#include <stdint.h>

#include <kernel/kernel.h>
#include <kernel/process/task.h>
#include <kernel/mm/memory.h>

#include <kernel/drivers/gpio/bcm_2835_gpio.h>
#include <kernel/drivers/interrupt/bcm_2835_interrupt.h>
#include <kernel/drivers/timer/bcm_2835_timer.h>
#include <kernel/drivers/serial/bcm_2835_uart.h>

#include <asm/peripheral.h>

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

void __attribute__((noreturn)) arch_kernel_main(uint32_t r0, uint32_t r1, uint32_t r2)
{
    atags_detect((uint32_t *)r2);

    init_bcm_2835_gpio(PERIPHERAL_BASE);
    interrupt_manager = init_bcm_2835_interrupt(PERIPHERAL_BASE);
    init_bcm_2835_timer(PERIPHERAL_BASE, INTERRUPT_TIMER3);
    init_bcm_2835_uart(PERIPHERAL_BASE, INTERRUPT_UART0);

    enable_interrupts();

    kernel_main(atags_get_cmdline());
}

#include <kernel/utils/printk.h>

void arch_kernel_irq_handler(void *ctx)
{
    irq_value_t irq = interrupt_manager->get_irq(ctx);
    kernel_irq_handler(irq, ctx);
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

void arch_memory_init(uint32_t *memory_total, uint32_t *memory_kernel)
{
    extern uint32_t __end;
    *memory_total = atags_get_memory();
    *memory_kernel = (uint32_t)&__end;
}

#include <kernel/utils/printk.h>

void arch_create_kernel_task(kernel_task_t *task, void *fn, void *arg)
{
    printk("ret_from_fk: 0x%x\n", return_from_fork);
    task->context.pc = (uint32_t)return_from_fork;
    task->context.sp = (uint32_t)task->stack_top;
    task->context.r12 = (uint32_t)fn;  // r12 = fn
    task->context.r11 = (uint32_t)arg; // r11 = arg
}