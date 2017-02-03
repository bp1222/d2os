#include <include/kernel.h>
#include <drivers/uart/uart.h>
#include <drivers/gpio/gpio.h>

size_t strlen(const char* str)
{
    size_t ret = 0;
    while ( str[ret] != 0x0 )
        ret++;
    return ret;
}

volatile uart_registers_t *uart_reg = (volatile uart_registers_t *)UART0_BASE;

void uart_init(void)
{
    // Disable UART0.
    uart_reg->CR = 0x00000000;

    SET_PIN_PULL(PULL_DISABLE, 14);
    SET_PIN_PULL(PULL_DISABLE, 15);

    // Clear pending interrupts.
    uart_reg->ICR = 0x000007FF;

    // Enable FIFO & 8 bit data transmissio (1 stop bit, no parity).
    uart_reg->LCRH = (1 << 1) | (1 << 2) | (1 << 5) | (1 << 6);

    // Mask all interrupts.
    uart_reg->IMSC = (1 << 1) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10);

    // Enable UART0, receive & transfer part of UART.
    uart_reg->CR = (1 << 0) | (1 << 8) | (1 << 9) | (1 << 14);
}

void uart_putc(const char byte)
{
    if (byte) {
        // Wait for UART to become ready to transmit.
        while (uart_reg->FR & (1 << 5)) {}
        uart_reg->DR = byte;
    }
}

void uart_write(const char* buffer, size_t size)
{
    for ( size_t i = 0; i < size; i++ )
        uart_putc(buffer[i]);
}

void uart_puts(const char* str)
{
    uart_write((const char*) str, strlen(str));
}

char uart_getc()
{
    // Wait for UART to have recieved something.
    if (uart_reg->FR & (1 << 4)) {
        return uart_reg->DR;
    }

    return 0;
}
