
#ifndef __BCM_2835_TIMER_H__
#define __BCM_2835_TIMER_H__
#include <stdint.h>

#include <kernel/interrupt.h>

#define TIMER_OFFSET 0x3000

typedef struct
{
    uint32_t timer0_matched : 1;
    uint32_t timer1_matched : 1;
    uint32_t timer2_matched : 1;
    uint32_t timer3_matched : 1;
    uint32_t reserved : 28;
} timer_control_reg_t;

typedef struct
{
    timer_control_reg_t control;
    uint32_t counter_low;
    uint32_t counter_high;
    uint32_t timer0;
    uint32_t timer1;
    uint32_t timer2;
    uint32_t timer3;
} timer_registers_t;

void init_bcm_2835_timer(uint32_t base, irq_value_t irq);

#endif