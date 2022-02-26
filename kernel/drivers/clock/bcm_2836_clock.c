#include <stdint.h>
#include <kernel/utils/printk.h>

void init_bcm_2836_clock()
{
    uint32_t reg;
    asm volatile("mrc p15, 0, %0, c14, c0, 0"
                 : "=r"(reg));
    printk("0x%x\n", reg);
}