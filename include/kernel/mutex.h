#ifndef __MUTEX_H__
#define __MUTEX_H__
#include <stdint.h>

typedef uint32_t mutex_t;

extern void mutex_acquire(mutex_t *mutex);
extern void mutex_release(mutex_t *mutex);

#endif