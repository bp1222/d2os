#include <stdint.h>
#include <stddef.h>

#include <kernel/kernel.h>
#include <kernel/utils/printk.h>
#include <kernel/drivers/gpio/gpio.h>
#include <kernel/drivers/uart/uart.h>

static volatile uart_registers_t *uart_reg = (volatile uart_registers_t *)UART0_BASE;

static void printk_putc(void *p, char c)
{
    uart_putc(c);
}

void uart_init(void)
{
    // Disable UART0.
    uart_reg->CR = 0x0;

    // Disable the UART Pins
    SET_PINS_PULL(PULL_DISABLE, (1 << 14) | (1 << 15));

    // Clear pending interrupts.
    uart_reg->ICR = 0x7FF;

    // Enable FIFO & 8 bit data transmissio (1 stop bit, no parity).
    uart_reg->LCRH = UART0_LCRH_PEN |
                     UART0_LCRH_FEN |
                     UART0_LCRH_WLEN_8BIT;

    // Mask all interrupts.
    uart_reg->IMSC = 0x0;

    // Enable UART0, receive & transfer part of UART.
    uart_reg->CR = UART0_CR_UARTEN |
                   UART0_CR_TXE |
                   UART0_CR_RXE |
                   UART0_CR_RTSEN;

    // Set the output for printk
    init_printk(NULL, printk_putc);
}

void uart_putc(const char byte)
{
    if (byte)
    {
        while (uart_reg->FR & UART0_FR_TXFF)
            ;
        uart_reg->DR = byte;
    }
}

void uart_write(const char *buffer, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        uart_putc(buffer[i]);
    }
}

char uart_getc()
{
    if (uart_reg->FR & UART0_FR_RXFE)
    {
        return uart_reg->DR;
    }

    return 0;
}
