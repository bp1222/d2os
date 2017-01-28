#include <include/kernel.h>
#include <drivers/uart/uart.h>

size_t strlen(const char* str)
{
    size_t ret = 0;
    while ( str[ret] != 0x0 )
        ret++;
    return ret;
}

volatile uart_registers *uart_reg = (volatile uart_registers *)UART0_BASE;

#define GPIO_BASE 0x20200000
#define GPPUD     (GPIO_BASE + 0x94)
#define GPPUDCLK0 (GPIO_BASE + 0x98)

void uart_init(void)
{
    // Disable UART0.
    uart_reg->CR = 0x00000000;

    // Setup the GPIO pin 14 && 15.
    // Disable pull up/down for all GPIO pins & delay for 150 cycles.
    //
    *(volatile uint32_t*)GPPUD = 0x00000000;
    delay(150);

    // Disable pull up/down for pin 14,15 & delay for 150 cycles.
    *(volatile uint32_t*)GPPUDCLK0 = (1 << 14) | (1 << 15);
    delay(150);

    // Write 0 to GPPUDCLK0 to make it take effect.
    *(volatile uint32_t*)GPPUDCLK0 = 0x00000000;

    // Clear pending interrupts.
    uart_reg->ICR = 0x000007FF;

    // Enable FIFO & 8 bit data transmissio (1 stop bit, no parity).
    uart_reg->LCRH = (1 << 1) | (1 << 2) | (1 << 5) | (1 << 6);

    // Mask all interrupts.
    uart_reg->IMSC = (1 << 1) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10);

    // Enable UART0, receive & transfer part of UART.
    uart_reg->CR = (1 << 0) | (1 << 8) | (1 << 9);
}

void uart_putc(const char byte)
{
    // Wait for UART to become ready to transmit.
    while (uart_reg->FR & (1 << 5)) {}
    uart_reg->DR = byte;
}

char uart_getc()
{
    // Wait for UART to have recieved something.
    while (uart_reg->FR & (1 << 4)) {}
    return uart_reg->DR;
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

