#ifndef __KERNEL_H__
#define __KERNEL_H__

void __attribute__((noreturn)) kernel_main(const char *cmdline);

void panic();

uint32_t arch_get_cpu(void);
#define get_cpu() arch_get_cpu()

#endif