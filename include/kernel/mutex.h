#ifndef __MUTEX_H__
#define __MUTEX_H__
#include <stdint.h>

typedef uint32_t mutex_t;

void arch_mutex_acquire(mutex_t *);
void arch_mutex_release(mutex_t *);

#define mutex_acquire(mtx) arch_mutex_acquire(mtx)
#define mutex_release(mtx) arch_mutex_release(mtx)

#endif