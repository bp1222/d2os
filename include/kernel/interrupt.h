#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__
#include <kernel/kernel.h>

#define INTERRUPT_OFFSET 0xB200
#define INTERRUPT_BASE (PERIPHERAL_BASE + INTERRUPT_OFFSET)

typedef struct
{
    uint32_t pending_basic;
    uint32_t pending_1;
    uint32_t pending_2;
    uint32_t fiq_control;
    uint32_t enable_1;
    uint32_t enable_2;
    uint32_t enable_basic;
    uint32_t disable_1;
    uint32_t disable_2;
    uint32_t disable_basic;
} irq_registers_t;

typedef enum
{
    INTERRUPT_TIMER0 = 0,
    INTERRUPT_TIMER1 = 1,
    INTERRUPT_TIMER2 = 2,
    INTERRUPT_TIMER3 = 3,
    INTERRUPT_UART0 = 57,
} irq_value_t;

#define NUM_INTERRUPTS 64

#define IRQ_GPU_1(i) (i < 32)
#define IRQ_GPU_2(i) (i >= 32 && i < 64)

typedef void (*interrupt_handler_t)(irq_value_t irq);

void interrupt_init();
void interrupt_register(uint32_t irq, interrupt_handler_t handler);

void enable_interrupts();
void disable_interrupts();


#endif
