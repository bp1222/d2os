#ifndef __MMU_H__
#define __MMU_H__

void setup_pagetable(uint32_t mem_start, uint32_t mem_end, uint32_t kernel_end);
void init_mmu();

#endif