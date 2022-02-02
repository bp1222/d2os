#include <stdint.h>
#include <stddef.h>

#include <kernel/kernel.h>
#include <kernel/mutex.h>
#include <kernel/mm/memory.h>
#include <kernel/utils/printk.h>

#include <asm/memory.h>

/* MOVE ME */
void memset(void *dest, uint8_t c, uint32_t l)
{
    uint8_t *d = dest;
    while (l--)
    {
        *d++ = c;
    }
}

#define MB 0x100000UL

#define RESERVED_KERNEL (8 * MB)
#define RESERVED_KERNEL_PAGES (RESERVED_KERNEL / PAGE_SIZE)

#define MEM_REGION (ARCH_MEM_SIZE * MB) / PAGE_SIZE / 32

static uint32_t memory_map[MEM_REGION];

static uint32_t memory_total;
static uint32_t memory_kernel;
static uint32_t kernel_pages;
static uint32_t max_page;

//static mutex_t memory_mutex;

void hexstrings(unsigned int d)
{
    //unsigned int ra;
    unsigned int rb;
    unsigned int rc;

    rb = 32;
    while (1)
    {
        rb -= 4;
        rc = (d >> rb) & 0xF;
        if (rc > 9)
            rc += 0x37;
        else
            rc += 0x30;
        printk("%c", rc);
        if (rb == 0)
            break;
    }
    printk("%c", 0x20);
}

void hexstring(unsigned int d)
{
    hexstrings(d);
    printk("%c", 0x0D);
    printk("%c", 0x0A);
}

static void set_memory_used(uint32_t page)
{
    uint32_t entry;
    uint8_t bit;

    entry = page / 32;
    bit = page % 32;

    memory_map[entry] |= 1 << bit;
}

static void set_memory_free(uint32_t page)
{
    uint32_t entry;
    uint8_t bit;

    entry = page / 32;
    bit = page % 32;

    memory_map[entry] &= ~(1 << bit);
}

static uint8_t test_memory_free(uint32_t page)
{
    uint32_t entry;
    uint8_t bit;

    entry = page / 32;
    bit = page % 32;

    return (memory_map[entry] & (1 << bit)) == 0;
}

static int32_t find_free_memory(uint32_t num_pages, uint32_t start, uint32_t end)
{
    int i, j;
    for (i = start; i < end; i++)
    {
        if (test_memory_free(i))
        {
            for (j = 0; j < num_pages; j++)
            {
                if (!test_memory_free(i + j))
                {
                    break;
                }
            }
            if (j == num_pages && i + j < end)
            {
                return i;
            }
        }
    }
    return -1;
}

void memory_init()
{
    arch_memory_init(&memory_total, &memory_kernel);
    kernel_pages = memory_kernel / PAGE_SIZE;
    max_page = memory_total / PAGE_SIZE / 32;

    for (uint32_t i = 0; i < kernel_pages; i++)
    {
        set_memory_used(i);
    }

    printk("Initializing %dMB of memory. %d pages required, %d available\n\r"
           "%dkB reserved (%dkB) kernel, %dkB memory map)\n\r",
           memory_total / MB,
           max_page, MEM_REGION,
           RESERVED_KERNEL / 1024,
           memory_kernel / 1024,
           memory_total / PAGE_SIZE / 1024 / 32);
}

void *kmalloc(uint32_t size, memory_type_t type)
{
    uint32_t start, end;
    uint32_t num_pages;
    int32_t start_page;
    memory_alloc_t *alloc;

    switch (type)
    {
    case KERNEL_MEMORY:
        start = 0;
        end = RESERVED_KERNEL_PAGES;
        break;
    case USER_MEMORY:
        start = RESERVED_KERNEL_PAGES;
        end = memory_total;
        break;
    default:
        printk("kmalloc: unknown memory type\n");
        return NULL;
    }

    num_pages = ((size + sizeof(memory_alloc_t)) / PAGE_SIZE) + 1;

    mutex_acquire(&memory_mutex);
    if ((start_page = find_free_memory(num_pages, start, end)) == -1)
    {
        printk("kmalloc: unable to find contiguous memory space\n");
        return NULL;
    }
    for (int i = 0; i < num_pages; i++)
    {
        set_memory_used(start_page + i);
    }
    mutex_release(&memory_mutex);

    alloc = (memory_alloc_t *)(start_page * PAGE_SIZE);

    memset(alloc, 0, num_pages * PAGE_SIZE);

    alloc->magic = MEMORY_MAGIC;
    alloc->pages = num_pages;
    alloc->ptr = (void *)((uintptr_t)alloc + sizeof(memory_alloc_t));

    return alloc->ptr;
}

void kfree(void *ptr)
{
    memory_alloc_t *alloc = (memory_alloc_t *)((uintptr_t)ptr - sizeof(memory_alloc_t));
    int32_t start_page;

    if (alloc->magic != MEMORY_MAGIC)
    {
        printk("kfree: attempting to free bad memory\n");
        panic();
        return;
    }

    start_page = (uint32_t)alloc / PAGE_SIZE;

    mutex_acquire(&memory_mutex);
    for (int i = 0; i < alloc->pages; i++)
    {
        set_memory_free(start_page + i);
    }
    mutex_release(&memory_mutex);
}