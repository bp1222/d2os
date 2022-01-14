#include <stdint.h>
#include <stddef.h>

#include <kernel/interrupt.h>
#include <kernel/smp.h>
#include <kernel/drivers/timer/timer.h>
#include <kernel/processes/process.h>
#include <kernel/utils/printk.h>
#include <kernel/utils/string.h>

process_t *current_process[NUM_CPU];

void schedule(void *ctx)
{
    cpu_core_t core = smp_get_core();
    process_t *current = current_process[core];
    process_t *next = NULL;

    while (1) {
        next = current->next;

        if (next == NULL) {
            next = first_process;
        }

        if (next != NULL) {
            break;
        }

        if (next == current) {
            next = first_process;
            break;
        }
    }

    if (current != next) {
        current_process[core] = next;

        memcpy(&current->registers, ctx, sizeof(registers_t));
        memcpy(ctx, &next->registers, sizeof(registers_t));
    }
}

void set_current_process(cpu_core_t cpu, process_t* process) {
    current_process[cpu] = process;
}

void reap()
{
    printk("REAPING PROCESS\n");
    while (1)
        ;
}