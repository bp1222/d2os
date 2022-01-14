#ifndef __MEMORY_H__
#define __MEMORY_H__
#include <stdint.h>

#define MEMORY_MAGIC 0xdeadbeef

typedef struct {
    uint32_t magic;
    int32_t pages;
    void *ptr;
} memory_alloc_t;

typedef enum {
    KERNEL_MEMORY = 0,
    USER_MEMORY = 1,
} memory_type_t;

void memory_init();
void *kmalloc(uint32_t size, memory_type_t type);
void kfree(void *ptr);

#define isb() asm volatile("isb" :: \
                               : "memory")
#define dsb() asm volatile("dsb" :: \
                               : "memory")
#define dmb() asm volatile("dmb" :: \
                               : "memory")

#endif