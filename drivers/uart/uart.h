#ifndef __UART_H__
#define __UART_H__
#include <include/kernel.h>

#define UART0_BASE  0x20201000

typedef struct {
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

extern volatile uart_registers_t *uart_reg;

void uart_init(void);
void uart_putc(const char byte);
char uart_getc();
void uart_puts(const char* str);

#endif
