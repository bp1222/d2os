#ifndef __MEMORY_H__
#define __MEMORY_H__

void memory_init();

#define isb() asm volatile("isb" :: \
                               : "memory")
#define dsb() asm volatile("dsb" :: \
                               : "memory")
#define dmb() asm volatile("dmb" :: \
                               : "memory")

#endif