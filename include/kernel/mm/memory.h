#ifndef __MEMORY_H__
#define __MEMORY_H__
#include <stdint.h>
#include <arch/memory.h>

#define MEMORY_MAGIC 0xdeadbeef

typedef struct
{
    uint32_t magic;
    int32_t pages;
    void *ptr;
} memory_alloc_t;

typedef enum
{
    KERNEL_MEMORY = 0,
    USER_MEMORY = 1,
} memory_type_t;

void memory_init();

void *kmalloc(uint32_t size, memory_type_t type);
void kfree(void *ptr);

void arch_memory_init();
void arch_pagetable_init(uint32_t mem_start, uint32_t mem_end, uint32_t kernel_end);
void arch_mmu_init();

void arch_isb();
void arch_dsb();
void arch_dmb();

#endif