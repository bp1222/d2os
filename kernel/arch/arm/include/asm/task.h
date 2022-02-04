#ifndef __ASM_TASK_H__
#define __ASM_TASK_H__
#include <stdint.h>

void return_from_fork();

typedef struct
{
    uint32_t r4;
    uint32_t r5;
    uint32_t r6;
    uint32_t r7;
    uint32_t r8;
    uint32_t r9;
    uint32_t r10;
    uint32_t r11;
    uint32_t r12;
    uint32_t sp;
    uint32_t pc; // Stored as PC, loaded as LR
} cpu_context_t;

// This is representation of stored registers on the stack on interrupt entry
typedef struct
{
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r4;
    uint32_t r5;
    uint32_t r6;
    uint32_t r7;
    uint32_t r8;
    uint32_t r9;
    uint32_t r10;
    uint32_t r11;
    uint32_t r12;
    uint32_t sp;
    uint32_t lr;
    uint32_t pc;
    uint32_t spsr;
} kernel_task_registers_t;

#define INIT_CPU_CONTEXT \
    (cpu_context_t)      \
    {                    \
        .r4 = 0,         \
        .r5 = 0,         \
        .r6 = 0,         \
        .r7 = 0,         \
        .r8 = 0,         \
        .r9 = 0,         \
        .r10 = 0,        \
        .r11 = 0,        \
        .r12 = 0,        \
        .sp = 0,         \
        .pc = 0          \
    }

#endif