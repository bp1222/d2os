#include <stdint.h>
#include <stddef.h>

#include <kernel/kernel.h>
#include <kernel/list.h>
#include <kernel/boot/atags.h>
#include <kernel/memory/memory.h>
#include <kernel/memory/mmu.h>
#include <kernel/utils/printk.h>
#include <kernel/utils/string.h>

#define PAGE_SIZE 0x1000
#define MB 0x100000

extern uint32_t __end;

IMPLEMENT_LIST(page);

static page_t *all_pages_array;
static page_list_t free_pages;
static uint32_t num_pages;

void memory_init()
{
    num_pages = atags_get_memory() / PAGE_SIZE;
    all_pages_array = (page_t *)&__end;

    memset(all_pages_array, 0, sizeof(page_t) * num_pages);

    INITIALIZE_LIST(free_pages)

    uint32_t kernel_pages = (uint32_t)&__end / PAGE_SIZE;
    for (uint32_t i = 0; i < kernel_pages; i++) {
        all_pages_array[i].virt_map = i * PAGE_SIZE;
        all_pages_array[i].flags = PAGE_FLAG_ALLOCATED | PAGE_FLAG_KERNAL;
    }
    for (uint32_t i = kernel_pages; i < num_pages; i++) {
        all_pages_array[i].virt_map = i * PAGE_SIZE;
        all_pages_array[i].flags = 0;
        append_page_list(&free_pages, &all_pages_array[i]);
    }

    printk("Initializing memory: extended kernel size 0x%x, ram size: 0x%x\n\r", (uint32_t)&__end, atags_get_memory());
    printk("\tPages: %x\n\r", num_pages);

    void *m = alloc_page();
    free_page(m);

#define RESERVED_KERNEL	(16*1024*1024)			// 16MB

    setup_pagetable(0, atags_get_memory(), RESERVED_KERNEL);
    init_mmu();
}

void *alloc_page()
{
    page_t *page;

    if (size_page_list(&free_pages) == 0) {
        return NULL;
    }

    page = pop_page_list(&free_pages);
    page->flags = PAGE_FLAG_ALLOCATED | PAGE_FLAG_KERNAL;

    memset((void*)page->virt_map, 0, PAGE_SIZE);
    return (void*)page->virt_map;
}

void free_page(void *p)
{
    page_t *page;
    uint32_t page_num = (uintptr_t)p / PAGE_SIZE;
    
    if (page_num > num_pages) {
        printk("free_page: trying to free an unknown page\n\r");
        return;
    }
    
    page = &all_pages_array[page_num];
    page->flags = 0;
    append_page_list(&free_pages, page);
}