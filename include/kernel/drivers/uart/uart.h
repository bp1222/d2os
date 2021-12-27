#ifndef __UART_H__
#define __UART_H__
#include <kernel/peripheral.h>

#define UART0_OFFSET 0x201000
#define UART0_BASE (PERIPHERAL_BASE + UART0_OFFSET)

typedef struct
{
    uint32_t DR;
    uint32_t RSRECR;
    uint32_t reserved_0x08[4];
    uint32_t FR;
    uint32_t reserved_0x1c;
    uint32_t ILPR;
    uint32_t IBRD;
    uint32_t FBRD;
    uint32_t LCRH;
    uint32_t CR;
    uint32_t IFLS;
    uint32_t IMSC;
    uint32_t RIS;
    uint32_t MIS;
    uint32_t ICR;
    uint32_t DMACR;
    uint32_t reserved_0x4c[14];
    uint32_t ITCR;
    uint32_t ITIP;
    uint32_t ITOP;
    uint32_t TDR;
} uart_registers_t;

#define UART0_FR_TXFE (1 << 7) /* TXFE = Transmit FIFO empty */
#define UART0_FR_RXFF (1 << 6) /* RXFF = Receive FIFO full   */
#define UART0_FR_TXFF (1 << 5) /* TXFF = Transmit FIFO full */
#define UART0_FR_RXFE (1 << 4) /* RXFE = Receive FIFO empty */
#define UART0_FR_BUSY (1 << 3) /* BUSY = UART is busy transmitting */
#define UART0_FR_CTS (1 << 0)  /* CTS  = inverted version of nUARTCTS value */

#define UART0_LCRH_WLEN_5BIT (0 << 5)
#define UART0_LCRH_WLEN_6BIT (1 << 5)
#define UART0_LCRH_WLEN_7BIT (2 << 5)
#define UART0_LCRH_WLEN_8BIT (3 << 5)
#define UART0_LCRH_FEN (1 << 4)  /* FEN  = enable FIFOs */
#define UART0_LCRH_STP2 (1 << 3) /* STP2 = enable 2 stop bits */
#define UART0_LCRH_EPS (1 << 2)  /* EPS  = even parity select */
#define UART0_LCRH_PEN (1 << 1)  /* PEN  = parity enable */
#define UART0_LCRH_BRK (1 << 0)  /* BRK  = send break after next character */

#define UART0_CR_CTSEN (1 << 15) /*   15 = CTSEN = CTS Flow Enable */
#define UART0_CR_RTSEN (1 << 14) /*   14 = RTSEN = RTS Flow Enable */
#define UART0_CR_RTS (1 << 11)   /*   11 = RTS = Request to Send */
#define UART0_CR_RXE (1 << 9)    /*    9 = RXE = Receive Enable */
#define UART0_CR_TXE (1 << 8)    /*    8 = TXE = Transmit Enable */
#define UART0_CR_LBE (1 << 7)    /*    7 = LBE = Loopback Enable */
#define UART0_CR_UARTEN (1 << 0) /*    0 = UARTEN = UART Enable */

#define UART0_IMSC_OEIM (1 << 10)  /* OERIS = Overrun Interrupt Raw Status */
#define UART0_IMSC_BEIM (1 << 9)   /* BERIS = Break Interrupt Raw Status*/
#define UART0_IMSC_PEIM (1 << 8)   /* PERIS = Parity Interrupt Raw Status */
#define UART0_IMSC_FEIM (1 << 7)   /* FERIS = Framing Interrupt Raw Status */
#define UART0_IMSC_RTIM (1 << 6)   /* RTRIS = Receivce Timeout Raw Status */
#define UART0_IMSC_TXIM (1 << 5)   /* TXRIS = Transmit Interrupt Raw Status */
#define UART0_IMSC_RXIM (1 << 4)   /* RXRIS = Receive Masked Interrupt Raw Status */
#define UART0_IMSC_CTSMIM (1 << 1) /* CTSRIS = nUARTCTS Raw Status */

void uart_init(void);
void uart_putc(const char byte);
char uart_getc();

#endif
