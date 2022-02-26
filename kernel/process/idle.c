#include <stdint.h>
#include <kernel/utils/printk.h>

void idle_task()
{
    uint32_t reg;
    while (1)
    {
        asm volatile("mov %0, sp"
                     : "=r"(reg));
        printk("IDLE, 0x%x\n", reg);
        asm volatile("wfi");
    }
}