#include <stdint.h>

#include <kernel/memory.h>
#include <kernel/utils/printk.h>

void vfp_init(void) {
    uint32_t reg = 0;
    printk("Initializing Floating Point\n\r");

    asm volatile ("mrc p15, 0, %0, c1, c0, 2" : "=r"(reg));
    reg |= (0xf << 20);
    asm volatile ("mcr p15, 0, %0, c1, c0, 2" :: "r"(reg));

    isb();

    reg = 0x40000000;
    asm volatile ("vmsr fpexc, %0" :: "r"(reg));
}