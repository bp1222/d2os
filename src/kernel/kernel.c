#include <stdint.h>

#include <kernel/interrupt.h>
#include <kernel/kernel.h>
#include <kernel/scheduler.h>
#include <kernel/boot/atags.h>
#include <kernel/drivers/uart/uart.h>
#include <kernel/drivers/timer/timer.h>
#include <kernel/memory/memory.h>
#include <kernel/utils/printk.h>

extern void _inf_loop();

void __attribute__((noreturn)) kernel_main(uint32_t r0, uint32_t r1, uint32_t atags)
{
    (void)r0;
    (void)r1;

    /* Read Hardware Info */
    atags_detect((uint32_t*)atags);

    /* Init Serial */
    uart_init();
    
    printk("Welcome to D2os!\n\r\n\r");
    printk("Boot Args: r0 = %x, r1 = %x, atags = %x\n\r\n\r", (uint32_t *)r0, (uint32_t*)r1, (uint32_t *)atags);
    atags_dump((uint32_t*)atags);

    /* Setup memory, start MMU */
    memory_init();

    /* Setup interrupts */
    interrupt_init();

    /* Start System Timer */
    timer_init();

    goto die;

/*
    // Set GPIO 25 for output LED
    SET_OUTPUT_PIN(25);

    // Set GPIO 23 for input button
    SET_INPUT_PIN(23);
    SET_PINS_PULL(PULL_UP, (1 << 23));
    GPIO_REG(GPFEN0) |= (1 << 23);
    GPIO_REG(GPEDS0) |= (1 << 23);

    irq_registers->Enable_IRQs_2 |= (1 << 17);

    _enable_interrupts();
*/
die:
    printk("\n\rFully Booted\n\r");

    schedule();

    while (1) {
        asm("wfi");
    }
}

void delay(int count)
{
    asm volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n"
                 : "=r"(count)
                 : [count] "0"(count)
                 : "cc");
}