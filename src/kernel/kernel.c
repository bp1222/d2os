#include <kernel/kernel.h>
#include <kernel/interrupt.h>
#include <kernel/utils/printk.h>
#include <kernel/boot/atags.h>
#include <kernel/drivers/uart/uart.h>
#include <kernel/drivers/gpio/gpio.h>

void _enable_interrupts();

volatile irq_registers_t *irq_registers = (irq_registers_t *)(INTERRUPT_BASE + 0x200);

void toggle_led(uint32_t pin)
{
    if (!(GPIO_REG(GPLEV0) & (1 << pin)))
    {
        GPIO_REG(GPSET0) |= (1 << pin);
    }
    else
    {
        GPIO_REG(GPCLR0) |= (1 << pin);
    }
}

extern int _cpsr_startup_mode;
extern uint32_t __bss_end__;

extern void _inf_loop();

void __attribute__((noreturn)) kernel_main(uint32_t r0, uint32_t r1, uint32_t r2)
{
    (void)r0;
    (void)r1;
    uint32_t *r2p = (uint32_t*)r2;

    uart_init();
    
    atags_detect(r2p);

    printk("Welcome to D2os!\n\r");
    atags_dump(r2p);
    uint32_t rounded = 1;//(&__bss_end__)/(1024*1024);
    rounded += 1;
    rounded *= (1024 * 1024);
    printk("Initializing memory: kernel 0x%x bytes, rounded up to 0x%x\n\r", &__bss_end__, rounded);
    printk("Boot Args: r0 = %x, r1 = %x, r2 = %x\n\r", (uint32_t *)r0, (uint32_t*)r1, (uint32_t *)r2);

    _inf_loop();

    goto die;

    // Set GPIO 25 for output LED
    SET_OUTPUT_PIN(25);

    // Set GPIO 23 for input button
    SET_INPUT_PIN(23);
    SET_PIN_PULL(PULL_UP, 23);
    GPIO_REG(GPFEN0) |= (1 << 23);
    GPIO_REG(GPEDS0) |= (1 << 23);

    irq_registers->Enable_IRQs_2 |= (1 << 17);

    _enable_interrupts();
die:
    while (1)
        ;
}

void __attribute__((interrupt("IRQ"))) irq_handler(void)
{
    if (GPIO_REG(GPEDS0) & (1 << 23))
    {
        GPIO_REG(GPEDS0) |= (1 << 23);
        toggle_led(25);
    }
}

void delay(int count)
{
    asm volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n"
                 : "=r"(count)
                 : [count] "0"(count)
                 : "cc");
}