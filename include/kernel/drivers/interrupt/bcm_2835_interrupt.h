#ifndef __BCM_2835_INTERRUPT__
#define __BCM_2835_INTERRUPT__
#include <stdint.h>

#include <kernel/interrupt.h>

#define INTERRUPT_OFFSET 0xB200

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

#define NUM_INTERRUPTS 64

#define IRQ_GPU_1(i) (i < 32)
#define IRQ_GPU_2(i) (i >= 32 && i < 64)

kernel_interrupt_device_t *init_bcm_2835_interrupt(uint32_t base);

#endif