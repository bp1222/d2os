#include <kernel/utils/printk.h>

void idle_task()
{
    while (1)
    {
        printk("IDLE\n");
        asm volatile("wfi");
    }
}