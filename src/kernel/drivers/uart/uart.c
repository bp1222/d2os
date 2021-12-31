#include <stdint.h>
#include <stddef.h>

#include <kernel/interrupt.h>
#include <kernel/kernel.h>
#include <kernel/memory.h>
#include <kernel/utils/printk.h>
#include <kernel/drivers/gpio/gpio.h>
#include <kernel/drivers/uart/uart.h>

static uart_registers_t *uart_reg = (uart_registers_t *)UART0_BASE;

void uart_putc(const char byte)
{
    while (uart_reg->FR & UART0_FR_TXFF)
        ;
    uart_reg->DR = byte;
}

void uart_write(const char *buffer, size_t size)
{
    for (size_t i = 0; i < size; i++)
        uart_putc(buffer[i]);
}

uint32_t uart_getc()
{
    if (uart_reg->FR & UART0_FR_RXFE)
        return -1;

    return uart_reg->DR & 0xff;
}

static void printk_putc(void *p, char c)
{
    uart_putc(c);
}

static void uart0_interrupt_handler(irq_value_t irq)
{
    uint32_t c;
    while (1)
    {
        c = uart_getc();
        if (c == -1)
            break;
    }
    uart_reg->ICR |= UART0_ICR_ALL;
}

void uart_init(void)
{
    // Disable the UART Pins
    gpio_set_pull(14, GPIO_PULL_DISABLE);
    gpio_set_pull(15, GPIO_PULL_DISABLE);

    // Disable UART0.
    uart_reg->CR &= ~UART0_CR_UARTEN;

    // Clear pending interrupts.
    uart_reg->ICR |= UART0_ICR_ALL;

    // Wait until uart clear, flush FIFO
    while (uart_reg->FR & UART0_FR_BUSY)
        ;
    uart_reg->LCRH &= ~UART0_LCRH_FEN;

    // Enable FIFO & 8 bit data transmission (1 stop bit, no parity).
    uart_reg->LCRH |= 
                      UART0_LCRH_FEN |
                      UART0_LCRH_WLEN_8BIT;

    uart_reg->IMSC |= UART0_IMSC_RXIM;

    // Enable UART0, receive & transfer part of UART.
    uart_reg->CR |= UART0_CR_UARTEN |
                    UART0_CR_TXE |
                    UART0_CR_RXE;

    // Set the output for printk - This should move elsewhere
    init_printk(NULL, printk_putc);

    interrupt_register(INTERRUPT_UART0, uart0_interrupt_handler);
}
