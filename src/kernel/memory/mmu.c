#include <stdint.h>

#include <kernel/peripheral.h>
#include <kernel/memory/memory.h>
#include <kernel/memory/mmu.h>
#include <kernel/utils/printk.h>

#define NUM_PAGE_TABLE_ENTRIES 4096

static __attribute__((aligned(0x4000))) volatile uint32_t page_table[NUM_PAGE_TABLE_ENTRIES];

/* Enable a one-to-one physical to virtual mapping using 1MB pagetables */
void setup_pagetable(uint32_t mem_start, uint32_t mem_end, uint32_t kernel_end)
{
	uint32_t entry;
	int mmu_debug = 1;

	if (mmu_debug)
	{
		printk("\tSetting 1:1, cache disabled "
			   "for %d page table entries\n",
			   NUM_PAGE_TABLE_ENTRIES);
	}

	/* Enable supervisor only and cachable for kernel */
	for (entry = mem_start; entry < (kernel_end >> 20); entry++)
	{
		page_table[entry] = entry << 20 | SECTION_KERNEL;
	}
	if (mmu_debug)
	{
		printk("\tSetting cachable+kernel (0x%x) only for 0x%x to 0x%x\n",
			   SECTION_KERNEL, mem_start, kernel_end);
	}

	/* Enable cachable and readable by all for rest of RAM */
	for (entry = kernel_end >> 20; entry < mem_end >> 20; entry++)
	{
		page_table[entry] = entry << 20 | SECTION_RAM;
	}
	if (mmu_debug)
	{
		printk("\tSetting cachable+any (0x%x) for 0x%x to 0x%x\n",
			   SECTION_RAM, kernel_end, mem_end);
	}

	/* Set from 1GB-4GB as unused*/
	for (entry = mem_end >> 20; entry < (0x80000000 >> 20); entry++)
	{
		page_table[entry] = 0;
	}
	if (mmu_debug)
	{
		printk("\tSetting 1G - 4G (0x%x) for 0x%x to 0x%x\n",
			   0, mem_end, 0x80000000);
	}

	/* Set Peripheral area correctly */
	for (entry = PERIPHERAL_BASE >> 20; entry < PERIPHERAL_END >> 20; entry++)
	{
		page_table[entry] = entry << 20 | SECTION_PERIPH;
	}
	if (mmu_debug)
	{
		printk("\tSetting Peripheral (0x%x) for 0x%x to 0x%x\n",
			   SECTION_PERIPH, PERIPHERAL_BASE, PERIPHERAL_END);
	}
}

static void invalidate_instruction_cache() {
	uint32_t reg = 0;
	asm volatile("mcr p15, 0, %0, c7, c5, 1"
				 :
				 : "r"(reg));
}

static void flush_branch_target_cache() {
	uint32_t reg = 0;
	asm volatile("mcr p15, 0, %0, c7, c5, 6"
				 :
				 : "r"(reg));
}

static void tlb_invalidate_all()
{
	uint32_t reg = 0;
	asm volatile("mcr p15, 0, %0, c8, c7, 0"
				 :
				 : "r"(reg));
}

void init_mmu(uint32_t mem_start, uint32_t mem_end, uint32_t kernel_end)
{
	int debug = 1;
	uint32_t reg;

	if (debug)
		printk("Initializing PageTable\n\r");
	setup_pagetable(mem_start, mem_end, kernel_end);

	if (debug)
		printk("Initializing MMU\n\r");

	if (debug)
		printk("\tClean the data cache\n");
	clean_data_cache();

	dmb();
	isb();

	/* Need to enable SMP cache coherency in auxiliary control register (ACTLR) */
	if (debug)
		printk("\tEnabling SMPEN in ACTLR\n");
	asm volatile("mrc p15, 0, %0, c1, c0, 1"
				 : "=r"(reg));
	reg |= (1 << 6);
	asm volatile("mcr p15, 0, %0, c1, c0, 1"
				 :
				 : "r"(reg));

	/* DACR: Domain Access Control Register */
	if (debug)
		printk("\tInitialize DACR\n");
	reg = 0x55555555; // all domains, client access
	asm volatile("mcr p15, 0, %0, c3, c0, 0"
				 :
				 : "r"(reg));

	/* TTBCR : Translation Table Base Control Register */
	if (debug)
		printk("\tSetting TTBCR to use TTBR0 only\n");
	reg = 0;
	asm volatile("mcr p15, 0, %0, c2, c0, 2"
				 :
				 : "r"(reg));

	/* TTBR0 (VMSA): Translation Table Base Register 0 */
	if (debug)
		printk("\tSetting page table to %x\n", (uint32_t)page_table | 0x5b);
	reg = (uint32_t)&page_table | 0x5b; // 1011011
	asm volatile("mcr p15, 0, %0, c2, c0, 0"
				 :
				 : "r"(reg));

	dmb();
	isb();

	if (debug)
		printk("\tInvalidating data cache\n\r");
	invalidate_data_cache();

	if (debug)
		printk("\tInvalidating instruction cacche\n\r");
	invalidate_instruction_cache();

	if (debug)
		printk("\tFlushing branch target cache\n\r");
	flush_branch_target_cache();

	if (debug)
		printk("\tInvalidating TLB\n");
	tlb_invalidate_all();

	dsb();
	isb();

	if (debug)
		printk("\tEnable MMU, Cache, ICache\n");
	asm volatile("mrc p15, 0, %0, c1, c0, 0"
				 : "=r"(reg));
	/* Enable MMU, Cache, and ICache */
	reg |= (1 << 0) | (1 << 2) | (1 << 11) | (1 << 12);
	asm volatile("mcr p15, 0, %0, c1, c0, 0"
				 :
				 : "r"(reg));
}
