#include <asm/asm.h>

.section ".text.boot"
.code	32
.align	2

.global _start
_start:
    bl _reset_handler
    
// Preserve r0, r1, r2 for kernel
.global _reset_handler
_reset_handler:
    mrc MIDR(r11)
    ldr r10, =MAINID_ARMV6
    cmp r11, r10
    beq 2f 

    mrs r12, cpsr
    and r12, #CPSR_MODE_MASK
    cmp r12, #CPSR_MODE_HYPERVISOR
    bne 1f 

    mov r12, #(CPSR_MODE_SVC | CPSR_IRQ_INHIBIT | CPSR_FIQ_INHIBIT)
    msr cpsr_c, r12

1:
    mrc MPIDR(r12)
    ands r12, #0x3
    bne _hang

2:
    ldr     r3, =_interrupt_table
    mov     r4, #0x0000
    
    ldmia   r3!,{r5-r12}
    stmia   r4!,{r5-r12}
    ldmia   r3!,{r5-r12}
    stmia   r4!,{r5-r12}

    ldr sp, =__svc_stack_top_core0

    ldr r4, =__bss_start__
    ldr r5, =__bss_end__
    mov r6, #0
    mov r7, #0
    mov r8, #0
    mov r9, #0
    b       2f
1:
    stmia r4!, {r6-r9}
2:
    cmp r4, r5
    bne 1b

    bl arch_kernel_main

.global _hang
_hang:
    b _hang
