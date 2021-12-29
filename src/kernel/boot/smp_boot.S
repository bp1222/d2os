#include <kernel/assembly.h>

.code 32
.align 2

.global smp_init_core
smp_init_core:
    mrs r0, cpsr
    and r0, #CPSR_MODE_MASK
    cmp r0, #CPSR_MODE_HYPERVISOR
    bne _continue_smp_init_core

    // We're in a Hypervisor mode, lets switch back to supervisor
    bic r0, r0, #CPSR_MODE_MASK
    orr r0, r0, #(CPSR_MODE_SVC | CPSR_IRQ_INHIBIT | CPSR_FIQ_INHIBIT)
    msr cpsr_c, r0

_continue_smp_init_core:
    mov r3, #(CPSR_MODE_SVC | CPSR_IRQ_INHIBIT | CPSR_FIQ_INHIBIT)
    msr cpsr_c, r3

    mrc MPIDR(r0)
    ands r0, #0x3

    cmp r0, #0x1
    ldreq r1, =__svc_stack_top_core1
    beq _stack_set

    cmp r0, #0x2
    ldreq r1, =__svc_stack_top_core2
    beq _stack_set
    
    cmp r0, #0x3
    ldreq r1, =__svc_stack_top_core3
    beq _stack_set

    // Error?
    b _inf_loop
    
_stack_set:
    mov sp, r1
    bl smp_boot_core

_inf_loop:
    wfe
    b _inf_loop
