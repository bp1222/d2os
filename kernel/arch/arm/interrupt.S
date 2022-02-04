#include <asm/asm.h>

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

_reset_vector_h:                    .word  . //_reset_handler
_undefined_instruction_vector_h:    .word  . //_undefined_instruction_handler
_software_interrupt_vector_h:       .word  . //_software_interrupt_handler
_prefetch_abort_vector_h:           .word  . //_prefetch_abort_handler
_data_abort_vector_h:               .word  . //_data_abort_handler
_unused_vector_h:                   .word  . //_unused_handler
_interrupt_vector_h:                .word  _irq_handler_asm
_fast_interrupt_vector_h:           .word  . //_fiq_handler

_irq_handler_asm:
    ldr r13, __irq_stack_h

    # Fix the return
    sub lr, lr, #4

    # Store currents lr, and the spsr to the IRQ stack
    str lr, [r13]
    mrs lr, spsr
    str lr, [r13, #4]

    # Prepare to change to SVC mode
    mrs r13, cpsr
    msr spsr, r13
    msr spsr_c, #(CPSR_MODE_SVC | CPSR_IRQ_INHIBIT | CPSR_FIQ_INHIBIT)

    # Determine which handler mode we're in
    and lr, lr, #0x0f
    ldr	lr, [pc, lr, lsl #2]

    # Switch to SVC mode and handle the interrupt
    movs pc, lr

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

    # Save r1-r12 to the stack
    stmia sp, {r0-r12}

    #
    # fiddle to finish the rest of the stack
    # r0 = sp_svc, r1 = lr_svc, r2 = lr_irq, r3 = spsr_irq
    #
    add r0, sp, #(CPU_CONTEXT_SIZE)
    mov r1, lr
    ldr r2, __irq_stack_h
    ldmia r2, {r2-r3}

    # Store them to the stack
    add r4, sp, #(12 * 4)
    stmia r4, {r0-r3}

    bl arch_kernel_irq_handler

    add r4, sp, #(12 * 4)
    ldmia r4, {r0-r3}

    # Restore SPSR
    msr spsr, r3
    mov lr, r2

    # Restore r0-r12
    ldmia sp, {r0, r12}

    # Move stack pointer back up
    add sp, sp, #(CPU_CONTEXT_SIZE)

    movs pc, lr

__irq_stack_h: .word __irq_stack

__irq_stack:
    .word 0 // lr_irq
    .word 0 // spsr_irq

.global return_from_fork
return_from_fork:
    bl enable_preempt
    mov r0, r11
    mov pc, r12

// r0 = (kernel_task_t*)old, r1 = (kernel_task_t*)new
.global arch_switch_task 
arch_switch_task:
    stmia   r0, {r4-r12, sp, pc}
    ldmia   r1, {r4-r12, sp, lr}
    mov     pc, lr