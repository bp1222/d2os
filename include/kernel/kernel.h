#ifndef __KERNEL_H__
#define __KERNEL_H__

#include <stdint.h>
#include <stdlib.h>

void __attribute__((noreturn)) kernel_main(uint32_t, uint32_t, uint32_t);

void delay(int count);

#endif
