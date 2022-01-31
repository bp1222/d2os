#include <stdint.h>

static __attribute__((always_inline)) inline void arch_mmio_write(uint32_t address, uint32_t data)
{
    uint32_t *ptr = (uint32_t *)address;
    asm volatile("str %[data], [%[address]]"
                 :
                 : [address] "r"(ptr), [data] "r"(data));
}

static __attribute__((always_inline)) inline uint32_t arch_mmio_read(uint32_t address) 
{
    uint32_t *ptr = (uint32_t *)address;
    uint32_t data;
    asm volatile("ldr %[data], [%[address]]"
                 : [data] "=r"(data)
                 : [address] "r"(ptr));
    return data;
}