#include <kernel/utils/printk.h>

void panic() {
    printk("PANIC: dying\n");
    while(1)
    ;
}