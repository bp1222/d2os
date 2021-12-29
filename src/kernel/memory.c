#include <stdint.h>
#include <stddef.h>

#include <kernel/kernel.h>
#include <kernel/memory.h>
#include <kernel/mmu.h>
#include <kernel/boot/atags.h>
#include <kernel/utils/printk.h>
#include <kernel/utils/string.h>

#define MB 0x100000
#define PAGE_SIZE 0x1000
#define RESERVED_KERNEL (8 * MB)

#define MEM_REGION 1024 * MB / PAGE_SIZE / 32
static uint32_t memory_map[MEM_REGION];

extern uint32_t __end;

static uint32_t memory_total;
static uint32_t memory_kernel;
static uint32_t kernel_pages;
static uint32_t max_page;

static int memory_used(uint32_t page) {
    uint32_t entry;
    uint8_t bit;

    entry = page / 32;
    bit = page % 32;
    memory_map[entry] |= 1 << bit;
}

static int memory_free(uint32_t page) {
    uint32_t entry;
    uint8_t bit;

    entry = page / 32;
    bit = page % 32;
    memory_map[entry] &= ~(1 << bit);
}

void memory_init()
{
    memory_total = atags_get_memory();
    memory_kernel = (uint32_t)&__end;
    kernel_pages = memory_kernel / PAGE_SIZE;
    max_page = memory_total / PAGE_SIZE / 32;

    for (uint32_t i = 0; i < kernel_pages; i++) {
        memory_used(i);
    }

    printk("Initializing %dMB of memory. %d pages required, %d available\n\r"
           "%dkB reserved (%dkB) kernel, %dkB memory map)\n\r",
           memory_total / MB,
           max_page, MEM_REGION,
           RESERVED_KERNEL / 1024,
           memory_kernel / 1024,
           memory_total / PAGE_SIZE / 1024 / 32);

	pagetable_init(0, memory_total, RESERVED_KERNEL);
}