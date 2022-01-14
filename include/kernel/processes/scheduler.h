#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__
#include <kernel/smp.h>
#include <kernel/processes/process.h>

extern process_t *current_process[NUM_CPU];

void schedule();

#endif