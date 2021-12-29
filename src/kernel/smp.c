#include <stdint.h>

#include "vfp.h"

#include <kernel/peripheral.h>
#include <kernel/kernel.h>
#include <kernel/mmu.h>
#include <kernel/smp.h>
#include <kernel/utils/printk.h>

extern void smp_init_core();

static volatile uint32_t core_booted[4];

void smp_boot_core(uint32_t r0, uint32_t r1)
{
    mmu_init();
    vfp_init();

    core_booted[r0] = 1;
    while (1) {
        delay(100);
        printk("Hello from %d\n", r0);
    }
}

void smp_boot()
{
    printk("Initializing SMP\n\r");

    for (int i = 1; i < 4; i++)
    {
        printk("Booting Core %d\n", i);
        mmio_write(CORE0_MBOX3_SET + (i * 0x10), (uint32_t)&smp_init_core);
        printk("Waiting on Core %d\n", i);
        while (core_booted[i] == 0)
            ;
        printk("Core %d Started\n", i);
    }
}

uint32_t smp_get_core() {
    uint32_t reg;
    asm volatile("mrc p15, 0, %0, c0, c0, 5"
                 :
                 : "r"(reg));
    return reg & 0x3;
}