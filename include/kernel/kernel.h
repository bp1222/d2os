#ifndef __KERNEL_H__
#define __KERNEL_H__

void __attribute__((noreturn)) kernel_main(uint32_t, uint32_t, uint32_t);

void delay(int count);

void panic();

#endif
