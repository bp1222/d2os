#include "include/asm.h"

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

.macro kernel_entry
    sub   lr, lr, #4
    ldr   sp, [sp]

    stmia sp, {r0-lr}
.endm

// void __attribute__((noreturn)) exec_process(process_t*)
.global exec_process
exec_process:
    mov     sp, r0

    ldmia   sp!, {r0, lr} 
    msr     spsr, r0

    ldmia   sp, {r0-r12, sp, lr}^
    add     sp, sp, #60

    movs    pc, lr


// Need to check if handling interrupt from USER or SYSTEM context
_irq_handler_asm:
    sub lr, lr, #4                      // Fix Where to retun from IRQ for this process (PC, on RET)

    sub sp, sp, #60                     // Move stack pointer down to store user registers
    stmia sp, {r0-r12, sp, lr}^         // Push user registers to the stack 

    mrs r0, spsr                        // Get the saved state register
    stmdb sp!, {r0, lr}                 // Store the spsr, and return location for this process

    mov r0, sp                          // Pass the process context ptr to the handler
    bl arch_kernel_irq_handler          // Handle IRQ

    ldmia sp!, {r0, lr}                 // Load the spsr, and return location for this process
    msr spsr, r0                        // Set the saved state registers
    ldmia sp, {r0-r12, sp, lr}^         // set the user mode values
    add sp, sp, #60                     // Move stack pointer up for the loaded user registers

    movs pc, lr                         // Back to the user program

ret_from_irq:
    cpsie i
    movs pc, lr


.global switch_process
switch_process:
    // Save the current thread's state
    push    {r3-r5, lr}
    //ldr     r3, =irq_current_registers
    ldr     r4, [r0]

    // Restore the new thread's state
    ldr     r0, [r1]
    pop     {r0-r12}
    msr     spsr, r12   // Restore the new thread's program status register
    pop     {lr}
    pop     {lr}
    subs    pc, lr