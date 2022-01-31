#include <stdint.h>
#include <stddef.h>
#include <kernel/interrupt.h>
#include <kernel/drivers/serial.h>
#include <kernel/drivers/gpio.h>
#include <kernel/drivers/serial/bcm_2835_uart.h>

static uart_registers_t *uart_reg;

static void uart_putc(const char byte)
{
    while (uart_reg->FR & UART0_FR_TXFF)
        ;
    uart_reg->DR = byte;
}

static void uart_write(const char *buffer, uint32_t size)
{
    uint32_t i;
    for (i = 0; i < size; i++)
        uart_putc(buffer[i]);
}

static uint32_t uart_getc()
{
    if (uart_reg->FR & UART0_FR_RXFE)
        return -1;

    return uart_reg->DR & 0xff;
}

/*
static void printk_putc(void *p, char c)
{
    uart_putc(c);
}
*/

static void uart0_interrupt_handler(irq_value_t irq, void *ctx)
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

static void uart_init(void)
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
    //init_printk(NULL, printk_putc);

}

static kernel_serial_device_t uart_device = {
    .write = uart_write,
};

void init_bcm_2835_uart(uint32_t peripheral_base, uint32_t irq) {
    uart_reg = (uart_registers_t *)(peripheral_base + UART0_OFFSET);
    uart_init();
    serial_init(&uart_device);
    set_interrupt_handler(irq, uart0_interrupt_handler);
}
