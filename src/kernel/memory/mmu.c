#include <stdint.h>

#include <kernel/utils/printk.h>



#define SCTLR_THUMB_EXCEPTION		(1<<30)
#define SCTLR_ACCESS_FLAG_ENABLE	(1<<29)
#define SCTLR_TEX_REMAP_ENABLE		(1<<28)
#define SCTLR_NONMASKABLE_FIQ		(1<<27)
#define SCTLR_EXCEPTION_ENDIANESS	(1<<25)
#define SCTLR_IRQ_VECTOR_ENABLE		(1<<24)
#define SCTLR_ALIGNMENT			(1<<22)
#define SCTLR_FAST_INTERRUPT_CONFIG	(1<<21)
#define SCTLR_UWXN			(1<<20)
#define SCTLR_WXN			(1<<19)
#define SCTLR_HARDWARE_ACCESS		(1<<17)
#define SCTLR_ROUND_ROBIT		(1<<14)
#define SCTLR_HIGH_VECTORS		(1<<13)
#define SCTLR_ICACHE_ENABLE		(1<<12)
#define SCTLR_BRANCH_PREDICTOR_ENABLE	(1<<11)
#define SCTLR_SWP_ENABLE		(1<<10)
#define SCTLR_CP15_BARRIER_ENABLE	(1<<5)
#define SCTLR_CACHE_ENABLE		(1<<2)
#define SCTLR_ALIGNMENT_CHECK		(1<<1)
#define SCTLR_MMU_ENABLE		(1<<0)

/* We want to cover all 4GB of address space	*/
/* Using 1MB pages, so need 4096 entries	*/
#define NUM_PAGE_TABLE_ENTRIES 4096

/* make sure properly aligned, as the low bits are reserved  */
/* This means we need 14-bit (16k) allignment */

static uint32_t __attribute__((aligned(16384))) page_table[NUM_PAGE_TABLE_ENTRIES];

/* We want a 1MB coarse page table descriptor */
/* B.3.5.1, p1326 */
/* All mappings global and executable */
/* 31-20 = section base address
   19 = NS (not secure)
   18 = 0 (section 0 or supersection 1)
   17 = NG (not global)
   16 = S (sharable)
   15 = AP[2]
   14-12 = TEX[2:0] = Type extension field
   11-10 - AP[1:0] (access permission bits)
   9 = Implementation defined
   8-5 = domain
   4 = XN
   3,2 = C,B determinte caching behavior, see Table b3-10
   1,0 = 1,0 - for coarse section 1MB pages
          16         8            AP 15,11-10
90c0e = 1001 0000 1100 0000 1110  011=full access
9080e = 1001 0000 1000 0000 1110  010=only root can write
9040e = 1001 0000 0100 0000 1110  001=only root can read/write
				TEX/CB = 000/11 = outer/inner WB no WA
9140e = 1001 0001 0100 0000 1110  001=only root can read/write
				TEX/CB = 001/11 = outer/inner WB yes WA
90c16 = 1001 0000 1100 0001 0110  011=full access, no cache
The above work.  Other values I tried didn't :(
This is: not-secure, shareable, domain 0, and the rest as described.
*/

// https://www.raspberrypi.org/forums/viewtopic.php?f=72&t=98904&start=25
// someone online sets to 0140e or 1140e, maybe 1040a
// 10416 for periph?

//#define SECTION_KERNEL	0x9040e		// shared, root-only, cached
#define SECTION_KERNEL 0x9140e // shared, root-only, cached
#define SECTION_USER 0x90c0e   // shared, any-access, cached
#define SECTION_1GB 0x90416    // root-only, non-cached
//92c10 = 1001 0010 1100 0001 0000
//#define SECTION_2GB	0x92c10		// root-only, non-cached
#define SECTION_2GB 0x90416     // root-only, non-cached
#define SECTION_PERIPH 0x90416  // root-only, non-cached
#define SECTION_DEFAULT 0x90c16 // any-access, non-cached

//#define SECTION_FULL_ACCESS_NO_CACHE	0x10c16

/* Enable a one-to-one physical to virtual mapping using 1MB pagetables */
void setup_pagetable(uint32_t mem_start, uint32_t mem_end, uint32_t kernel_end)
{
    uint32_t i;
    int mmu_debug = 1;

    /* Set up an identity-mapping for all 4GB */
    /* section-short descriptor 1MB pages */

    /* As a baseline, Set 1:1 mapping for all memory */
    /* Cache disabled, supervisor access only */

    if (mmu_debug)
    {
        printk("\tSetting 1:1, cache disabled "
               "for %d page table entries\n",
               NUM_PAGE_TABLE_ENTRIES);
    }

    /* First set some default values for all */
    for (i = 0; i < NUM_PAGE_TABLE_ENTRIES; i++)
    {
        page_table[i] = i << 20 | SECTION_DEFAULT;
    }
    if (mmu_debug)
    {
        printk("\tSetting default (0x%x) only for %x to %x\n",
               SECTION_DEFAULT, 0, 0xffffffff);
    }

    /* Enable supervisor only and cachable for kernel */
    for (i = (mem_start >> 20); i < (kernel_end >> 20); i++)
    {
        page_table[i] = i << 20 | SECTION_KERNEL;
    }
    if (mmu_debug)
    {
        printk("\tSetting cachable+kernel (0x%x) only for %x to %x\n",
               SECTION_KERNEL, mem_start, kernel_end);
    }

    /* Enable cachable and readable by all for rest of RAM */
    for (i = kernel_end >> 20; i < mem_end >> 20; i++)
    {
        page_table[i] = i << 20 | SECTION_USER;
    }
    if (mmu_debug)
    {
        printk("\tSetting cachable+any (0x%x) for %x to %x\n",
               SECTION_USER, kernel_end, mem_end);
    }

    /* Set from 1GB-2GB */
    for (i = 0x40000000 >> 20; i < 0x80000000 >> 20; i++)
    {
        page_table[i] = i << 20 | SECTION_1GB;
    }
    if (mmu_debug)
    {
        printk("\tSetting 1G (0x%x) for %x to %x\n",
               SECTION_1GB, 0x40000000, 0x80000000);
    }

    /* Set from 2GB-4GB */
    for (i = 0x80000000 >> 20; i <= 0xffffffff >> 20; i++)
    {
        page_table[i] = i << 20 | SECTION_2GB;
    }
    if (mmu_debug)
    {
        printk("\tSetting 2G (0x%x) for 0x%x to 0x%x\n",
               SECTION_2GB, 0x80000000, 0xffffffff);
    }
}


void init_mmu() {

int debug = 1;
	uint32_t reg;


	/* Need to enable SMP cache coherency in auxiliary control register (ACTLR) */
	if (debug) printk("\tEnabling SMPEN in ACTLR\n");
	asm volatile("mrc p15, 0, %0, c1, c0, 1" :  "=r" (reg):: "cc");
	if (debug) printk("\tAUX was %x and SMPEN was %d\n",reg,
		!!(reg&(1<<6)));
	reg|=(1<<6);	// Set SMPEN.
	asm volatile("mcr p15, 0, %0, c1, c0, 1" : : "r" (reg):"cc");
	if (debug) printk("\tMMU is on\n");

#if 0
	/* Flush TLB */
	if (debug) printk("\tInvalidating TLB\n");
	tlb_invalidate_all();

	/* Flush l1-icache */
	if (debug) printk("\tInvalidating icache\n");
	icache_invalidate_all();

	/* Flush l1-dcache */
	if (debug) printk("\tInvalidating dcache\n");
	disable_l1_dcache();
#endif

	/* Need to flush l2-cache too? */

	/* TTBCR : Translation Table Base Control Register */
	/* B3.5.4 (1330) */
	/* Choice of using TTBR0 (user) vs TTBR1 (kernel) */
	/* This is based on address range, also TTBCR.N */
	/* N is bottom 3 bits, if 000 then TTBR1 not used */
	/* We set N to 0, meaning only use TTBR0 */
	asm volatile("mrc p15, 0, %0, c2, c0, 2" : "=r" (reg) : : "cc");
	if (debug) printk("\tTTBCR before = %x\n",reg);
	reg=0;
	asm volatile("mcr p15, 0, %0, c2, c0, 2" : : "r" (reg) : "cc");

	/* See B.4.1.43 */
	/* DACR: Domain Access Control Register */
	/* All domains, set manager access (no faults for accesses) */
	if (debug) printk("\tInitialize DACR\n");
	reg=0x55555555;	// all domains, client access
	asm volatile("mcr p15, 0, %0, c3, c0, 0" : : "r" (reg): "cc");

	/* Initialize SCTLR.AFE */
	/* This boots with value 0, but set to 0 anyway */
	if (debug) printk("\tInitialize SCTLR.AFE\n");
	asm volatile("mrc p15, 0, %0, c1, c0, 0" : "=r" (reg) : : "cc");
	if (debug) printk("\tSCTLR before AFE = %x\n",reg);
	reg&=~SCTLR_ACCESS_FLAG_ENABLE;
	asm volatile("mcr p15, 0, %0, c1, c0, 0" : : "r" (reg) : "cc");

	/* TTBR0 (VMSA): Translation Table Base Register 0 */
	/* See B.4.1.154 (page 1729) */
	/* This is the userspace pagetable, can be per-process */

	/* Bits 31-N are the address of the table */
	/* Low bits are various config options, we leave them at 0 */
	/* FIXME: might need to do something if SMP support added */
	if (debug) {
		printk("\tSetting page table to %x\n",page_table);
		printk("\tPTE[0] = %x\n",page_table[0]);
	}

	reg=(uint32_t)page_table;
			// Saw 0100 1010 online (0x4a?)
			// which would be NOS=0 : outer sharable
	reg|=0x4a;	// 0110 1010

//	reg|=0x6a;	// 0110 1010
			// IRGN = 10 -> 01 (note it's bit 0,6]
			//   inner cache writeback/allocate
			// NOS = 1 : inner sharable
			// RGN = 01 : outer cache: writeback/allocate
			// S = 1 : sharable
	asm volatile("mcr p15, 0, %0, c2, c0, 0"
		: : "r" (reg) : "memory");

	/* See B.4.1.130 on page 1707 */
	/* SCTLR, VMSA: System Control Register */
	/* Enable the MMU by setting the M bit (bit 1) */
	asm volatile("mrc p15, 0, %0, c1, c0, 0" : "=r" (reg) : : "cc");
	if (debug) printk("\tSCTLR before = %x\n",reg);

	/* on pi3 this appears to be c50838 at boot */
	/* 0000 0000 1100 0101 0000 1000 0011 1000 */
	/* TE (thumb exec) =0 */
	/* AFE = 0 (no mess with AP bits) */
	/* TRE = 0 TEX remap disabled (TEX+CB choose caching) */
	/* EE = 0 (little-endian exceptions) */
	/* RR = 0 random replacement */
	/* V=0 vectors at 0x000000 */
	/* I=0 icache disabled */
	/* Z=1 branch predictor enabled */
	/* SW=0 swap insn disabled */
	/* CP15BEN=1 cp15 barriers enabled */
	/* c=0 dcache disabed */
	/* a=0 alignment check disabled */
	/* m=0 mmu disabled */

	/* Enable MMU 8 */
	reg|=SCTLR_MMU_ENABLE;

	/* Enable caches! */

	reg|=SCTLR_CACHE_ENABLE;
	reg|=SCTLR_ICACHE_ENABLE;

	asm volatile("mcr p15, 0, %0, c1, c0, 0" : : "r" (reg) : "cc");

	asm volatile("dsb");	/* barrier */
	asm volatile("isb");	/* barrier */

	asm volatile("mrc p15, 0, %0, c1, c0, 0" : "=r" (reg) : : "cc");
	if (debug) printk("\tSCTLR after = %x\n",reg);
}