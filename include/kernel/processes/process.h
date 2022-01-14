#ifndef __PROCESS_H__
#define __PROCESS_H__
#include <stdint.h>
#include <kernel/smp.h>

typedef struct process_t process_t;

extern process_t *first_process;
extern process_t *last_process;

typedef enum
{
    PROCESS_INIT = 0,
    PROCESS_PENDING = 1,
    PROCESS_RUNNING = 2,
    PROCESS_EXITING = 3,
    PROCESS_EXITED = 4,
} process_state_t;

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
    uint32_t spsr;
    uint32_t pc;
} registers_t;

struct process_t
{
    registers_t *registers;

    uint32_t pid;

    // Next possible process to run
    process_t *next;
    process_t *parent;

    char *name;

    process_state_t state;

    // Text of program
    void *text;
    uint32_t textsize;

    // stack
    void *stack;
    uint32_t stacksize;
};

typedef struct {
    void *stack;
    uint32_t size;
} stack_conf_t;

process_t *create_process(const char *name, stack_conf_t conf);
void print_process(process_t *p);
extern void switch_process(process_t *current, process_t *next);
extern void exec_process(process_t *p);

process_t *idle_task(cpu_core_t cpu);
process_t *init_userspace();

void set_current_process(cpu_core_t cpu, process_t* process);

#endif