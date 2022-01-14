#include <stdint.h>
#include <stddef.h>

#include <kernel/interrupt.h>
#include <kernel/smp.h>
#include <kernel/drivers/timer/timer.h>
#include <kernel/processes/process.h>
#include <kernel/utils/printk.h>

process_t *current_process[NUM_CPU];

int schduling = 0;

extern registers_t irq_current_registers;

void schedule()
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

        current->registers->r0 = irq_current_registers.r0;
        current->registers->r1 = irq_current_registers.r1;
        current->registers->r2 = irq_current_registers.r2;
        current->registers->r3 = irq_current_registers.r3;
        current->registers->r4 = irq_current_registers.r4;
        current->registers->r5 = irq_current_registers.r5;
        current->registers->r6 = irq_current_registers.r6;
        current->registers->r7 = irq_current_registers.r7;
        current->registers->r8 = irq_current_registers.r8;
        current->registers->r9 = irq_current_registers.r9;
        current->registers->r10 = irq_current_registers.r10;
        current->registers->r11 = irq_current_registers.r11;
        current->registers->r12 = irq_current_registers.r12;
        current->registers->sp = irq_current_registers.sp;
        current->registers->lr = irq_current_registers.lr;
        current->registers->spsr = irq_current_registers.spsr;
        current->registers->pc = irq_current_registers.pc;

        irq_current_registers.r0 = next->registers->r0;
        irq_current_registers.r1 = next->registers->r1;
        irq_current_registers.r2 = next->registers->r2;
        irq_current_registers.r3 = next->registers->r3;
        irq_current_registers.r4 = next->registers->r4;
        irq_current_registers.r5 = next->registers->r5;
        irq_current_registers.r6 = next->registers->r6;
        irq_current_registers.r7 = next->registers->r7;
        irq_current_registers.r8 = next->registers->r8;
        irq_current_registers.r9 = next->registers->r9;
        irq_current_registers.r10 = next->registers->r10;
        irq_current_registers.r11 = next->registers->r11;
        irq_current_registers.r12 = next->registers->r12;
        irq_current_registers.sp = next->registers->sp;
        irq_current_registers.lr = next->registers->lr;
        irq_current_registers.spsr = next->registers->spsr;
        irq_current_registers.pc = next->registers->pc;
    }
}

void set_current_process(cpu_core_t cpu, process_t* process) {
    current_process[cpu] = process;
}