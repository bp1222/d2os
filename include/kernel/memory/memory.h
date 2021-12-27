#ifndef __MEMORY_H__
#define __MEMORY_H__
#include <kernel/list.h>

DEFINE_LIST(page)

typedef uint8_t page_flags_t;

#define PAGE_FLAG_ALLOCATED (1 << 0)
#define PAGE_FLAG_KERNAL (1 << 1)
#define PAGE_FLAG_KERNAL_HEAP (1 << 2)

typedef struct page
{
    uint32_t virt_map;
    page_flags_t flags;

    DEFINE_LINK(page)
} page_t;

void memory_init();
void *alloc_page();
void free_page();

#endif