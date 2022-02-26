#include <arch/asm.h>

.code	32
.align	2

.section ".text.boot"
.global _interrupt_table
_interrupt_table:
    ldr pc, _reset_vector_h
    ldr pc, _undefined_instruction_vector_h
    ldr pc, _software_interrupt_vector_h
    ldr pc, _prefetch_abort_vector_h
    ldr pc, _data_abort_vector_h
    ldr pc, _unused_vector_h
    ldr pc, _interrupt_vector_h
    ldr pc, _fast_interrupt_vector_h

_reset_vector_h:                    .word  _reset_handler
_undefined_instruction_vector_h:    .word  _undef_abort_handler //_undefined_instruction_handler
_software_interrupt_vector_h:       .word  _swi_abort_handler //_software_interrupt_handler
_prefetch_abort_vector_h:           .word  _pf_abort_handler //_prefetch_abort_handler
_data_abort_vector_h:               .word  _d_abort_handler //_data_abort_handler
_unused_vector_h:                   .word  _u_abort_handler //_unused_handler
_interrupt_vector_h:                .word  _irq_handler_asm
_fast_interrupt_vector_h:           .word  _fiq_abort_handler //_fiq_handler

.global _fiq_abort_handler
_fiq_abort_handler:
    nop 
    nop
.global _u_abort_handler
_u_abort_handler:
    nop 
    nop
.global _d_abort_handler
_d_abort_handler:
    nop 
    nop
.global _pf_abort_handler
_pf_abort_handler:
    nop 
    nop
.global _swi_abort_handler
_swi_abort_handler:
    nop 
    nop
.global _undef_abort_handler
_undef_abort_handler:
    nop 
    nop

_irq_handler_asm:
    # Only need to store 2 IRQ values before we move to SVC mode
    ldr sp, _irq_stack_h

    # Fix the return
    sub lr, lr, #4

    # Store currents lr, and the spsr to the IRQ stack
    str lr, [sp]
    mrs lr, spsr
    str lr, [sp, #4]

    # Prepare to change to SVC mode
    mrs r13, cpsr
    eor r13, r13, #(CPSR_MODE_IRQ ^ CPSR_MODE_SVC)
    msr spsr_cxsf, r13

    # Determine which handler mode we're in
    and lr, lr, #0x0f
    ldr	lr, [pc, lr, lsl #2]

    # Switch to SVC mode and handle the interrupt
    movs pc, lr

.align 2
.word _irq_handler_usr
.word .
.word .
.word _irq_handler_svc

_irq_handler_usr:
    # Move stackpointer down to store the cpu context
    sub sp, sp, #(CPU_CONTEXT_SIZE)

    # Save r1-r12 to the stack
    stmia sp, {r0-r12}

    # Store User Regisgers
    ldr r0, [sp, #C_SP]
    stmia r0, {sp, lr}^
    b .

_irq_handler_svc:
    # Move stackpointer down to store the cpu context
    sub sp, sp, #(CPU_CONTEXT_SIZE)

    # Save r0-r12 to the stack
    stmia sp, {r0-r12}

    #
    # fiddle to finish the rest of the stack
    # r0 = lr_irq, r1 = spsr_irq
    #
    ldr r0, _irq_stack_h
    ldmia r0, {r0-r1}

    # Store them to the stack
    add r2, sp, #(13 * 4)
    stmia r2, {r0-r1}

    mov r0, sp
    bl arch_kernel_irq_handler

    add r3, sp, #(13 * 4)
    ldmia r3, {r0-r1}

    # Restore SPSR
    msr spsr_cxsf, r1
    mov lr, r0

    # Restore r0-r12
    ldmia sp, {r0, r12}

    # Move stack pointer back up
    add sp, sp, #(CPU_CONTEXT_SIZE)

    cmp r0, lr
    beq _error

    # Kick it back to the interrupted process
    movs pc, lr

.global _error
_error:
    nop
    nop
    b .

_irq_stack_h: .word _irq_stack
_irq_stack:
    .word 0 // lr_irq
    .word 0 // spsr_irq

.global return_from_fork
return_from_fork:
    bl enable_preempt
    mov r0, r11
    mov pc, r10

// r0 = (kernel_task_t*)old, r1 = (kernel_task_t*)new
.global arch_switch_task 
arch_switch_task:
    stmia   r0, {r4-r11, sp, lr}
    ldmia   r1, {r4-r11, sp, lr}
    mov     pc, lr