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

typedef union {
    struct {
        uint32_t m : 5;
        uint32_t t : 1;
        uint32_t f : 1;
        uint32_t i : 1;
        uint32_t a : 1;
        uint32_t e : 1;
        uint32_t it_l : 6;
        uint32_t ge : 4;
        uint32_t reserved : 4;
        uint32_t j : 1;
        uint32_t it_h : 2;
        uint32_t q : 1;
        uint32_t v : 1;
        uint32_t c : 1;
        uint32_t z : 1;
        uint32_t n : 1;
    } r;
    uint32_t cpsr;
} cpsr_t;

void __attribute__((noreturn)) kernel_main(uint32_t r0, uint32_t r1, uint32_t atags)
{
    (void)r0;
    (void)r1;
    cpsr_t temp;

    asm volatile ("mrs %0,CPSR":"=r" (temp):);

    atags_detect((uint32_t*)atags);

    uart_init();
    
    printk("Welcome to D2os!\n\r");
    printk("CSPR: 0x%x\n\r", temp);
    printk("\tIRQ: %d\n\r", temp.r.i);
    printk("\tFIQ: %d\n\r", temp.r.f);
    printk("Boot Args: r0 = %x, r1 = %x, atags = %x\n\r", (uint32_t *)r0, (uint32_t*)r1, (uint32_t *)atags);
    atags_dump((uint32_t*)atags);

    interrupt_init();

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