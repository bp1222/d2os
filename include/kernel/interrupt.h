#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

#define INTERRUPT_BASE 0x2000B000

typedef struct {
    uint32_t IRQ_basic_pending;
    uint32_t IRQ_pending_1;
    uint32_t IRQ_pending_2;
    uint32_t FIQ_control;
    uint32_t Enable_IRQs_1;
    uint32_t Enable_IRQs_2;
    uint32_t Enable_Basic_IRQs;
    uint32_t Disable_IRQs_1;
    uint32_t Disable_IRQs_2;
    uint32_t Disable_Basic_IRQs;
} irq_registers_t;

#endif
