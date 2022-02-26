#ifndef __UART_H__
#define __UART_H__
#include <stdint.h>

#define UART0_OFFSET 0x201000

typedef volatile uint32_t uart_reg_t;

typedef struct
{
    uart_reg_t DR;
    uart_reg_t RSRECR;
    uart_reg_t reserved0[4];
    uart_reg_t FR;
    uart_reg_t reserved1;
    uart_reg_t ILPR;
    uart_reg_t IBRD;
    uart_reg_t FBRD;
    uart_reg_t LCRH;
    uart_reg_t CR;
    uart_reg_t IFLS;
    uart_reg_t IMSC;
    uart_reg_t RIS;
    uart_reg_t MIS;
    uart_reg_t ICR;
    uart_reg_t DMACR;
    uart_reg_t reserved2[14];
    uart_reg_t ITCR;
    uart_reg_t ITIP;
    uart_reg_t ITOP;
    uart_reg_t TDR;
} uart_registers_t;

#define UART0_FR_TXFE (1 << 7)
#define UART0_FR_RXFF (1 << 6)
#define UART0_FR_TXFF (1 << 5)
#define UART0_FR_RXFE (1 << 4)
#define UART0_FR_BUSY (1 << 3)
#define UART0_FR_CTS (1 << 0)

#define UART0_LCRH_WLEN_5BIT (0 << 5)
#define UART0_LCRH_WLEN_6BIT (1 << 5)
#define UART0_LCRH_WLEN_7BIT (2 << 5)
#define UART0_LCRH_WLEN_8BIT (3 << 5)
#define UART0_LCRH_FEN (1 << 4)
#define UART0_LCRH_STP2 (1 << 3)
#define UART0_LCRH_EPS (1 << 2)
#define UART0_LCRH_PEN (1 << 1)
#define UART0_LCRH_BRK (1 << 0)

#define UART0_CR_CTSEN (1 << 15)
#define UART0_CR_RTSEN (1 << 14)
#define UART0_CR_RTS (1 << 11)
#define UART0_CR_RXE (1 << 9)
#define UART0_CR_TXE (1 << 8)
#define UART0_CR_LBE (1 << 7)
#define UART0_CR_UARTEN (1 << 0)

#define UART0_ICR_OEIC (1 << 10)
#define UART0_ICR_BEIC (1 << 9)
#define UART0_ICR_PEIC (1 << 8)
#define UART0_ICR_FEIC (1 << 7)
#define UART0_ICR_RTIC (1 << 6)
#define UART0_ICR_TXIC (1 << 5)
#define UART0_ICR_RXIC (1 << 4)
#define UART0_ICR_CTSMIC (1 << 1)
#define UART0_ICR_ALL 0xF7FF

#define UART0_IMSC_OEIM (1 << 10)
#define UART0_IMSC_BEIM (1 << 9)
#define UART0_IMSC_PEIM (1 << 8)
#define UART0_IMSC_FEIM (1 << 7)
#define UART0_IMSC_RTIM (1 << 6)
#define UART0_IMSC_TXIM (1 << 5)
#define UART0_IMSC_RXIM (1 << 4)
#define UART0_IMSC_CTSMIM (1 << 1)

void init_bcm_2835_uart(uint32_t base, uint32_t irq);

#endif
