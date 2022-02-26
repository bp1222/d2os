#ifndef __BCM_2836_INTERRUPT_H__
#define __BCM_2836_INTERRUPT_H__

#include <kernel/interrupt.h>

#define CONTROL_REG 0x00
#define CONTROL_CLK_SOURCE (1 << 8)
#define CONTROL_TIMER_INCREMENT (1 << 9)

#define PRESCALER_REG 0x08
#define GPU_INTERRUPT 0x0C

/* All of these are for Core 0, of 3 */
#define LOCAL_TIMER0_INT 0x40
#define LOCAL_CORE0_MBOX_INT 0x50
#define LOCAL_IRQ0 0x60
#define LOCAL_FIQ0 0x70

/* This is the start of a 4-mbox per core range */
#define LOCAL_CORE0_MBOX0_SET 0x80
#define LOCAL_CORE0_MOBX0_CLR 0xC0

kernel_interrupt_device_t *init_bcm_2836_interrupt(uint32_t base);

#endif