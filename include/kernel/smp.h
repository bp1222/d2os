#ifndef __SMP_H__
#define __SMP_H__

typedef enum
{
    CPU_0 = 0,
    CPU_1 = 1,
    CPU_2 = 2,
    CPU_3 = 3,
} cpu_core_t;

#define NUM_CPU 4

void smp_boot();
cpu_core_t smp_get_core();

// RPi2 drops Core1-3 into a dead state on power
// These mboxes are magic where we can write an address they
// start exeution on.
#define CORE0_MBOX3_SET 0x4000008C
#define CORE1_MBOX3_SET 0x4000009C
#define CORE2_MBOX3_SET 0x400000AC
#define CORE3_MBOX3_SET 0x400000BC

#endif