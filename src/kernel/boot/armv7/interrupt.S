#include <kernel/assembly.h>

.data
.code	32
.align	2
printstr:
  .ascii "\tSP: 0x%x\n\0"

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
_undefined_instruction_vector_h:    .word  _undefined_instruction_handler
_software_interrupt_vector_h:       .word  _software_interrupt_handler
_prefetch_abort_vector_h:           .word  _prefetch_abort_handler
_data_abort_vector_h:               .word  _data_abort_handler
_unused_vector_h:                   .word  _unused_handler
_interrupt_vector_h:                .word  _irq_handler_asm
_fast_interrupt_vector_h:           .word  _fiq_handler

.macro kernel_entry
    sub   lr, lr, #4
    ldr   sp, [sp]

    stmia sp, {r0-lr}
.endm

// void __attribute__((noreturn)) exec_process(process_t*)
.global exec_process
exec_process:
    ldr     r1, [r0]
    mov     sp, r1
    ldmia   sp, {r0-r12, sp}^

    ldr     r0, [sp, #(4 * 15)]
    ldr     lr, [sp, #(4 * 14)]
    msr     spsr, r0

    movs    pc, lr


// This is not optimal, we should use stack.  But for clarity as to what we're doing this works
_irq_handler_asm:
    sub lr, lr, #4                      // Fix Where to retun from IRQ for this process (PC, on RET)
    push {lr}                           // Store on the stack, for now need this register
    ldr lr, =irq_current_registers      // Load address for incoming reg struct
    stmia lr, {r0-r12, sp, lr}^         // Store user mode r0-r12, sp, lr into it.

    mrs r0, spsr                        // Get the saved state register
    pop {r1}                            // Load the LR for this IRQ
    add lr, lr, #(4 * 15)               // Increment the pointer in the irq_current_registers
    stmia lr, {r0, r1}                  // Store the spsr and the LR to the end

    bl _irq_handler

    ldr lr, =irq_current_registers      // Reset the lr ptr to the reg struct to restore
    ldmia lr, {r0-r12, sp, lr}^         // set the user mode values

    ldr r0, [lr, #(4 * 15)]             
    ldr r1, [lr, #(4 * 16)]

    msr spsr, r0

    movs pc, r1

/*
    mrs r12, spsr
    stmia lr!, {r0-r12, sp}
    mov r12, lr
    pop {lr}
    sub lr, lr, #4
    str lr, [r12]

    bl _irq_handler

    ldr r12, =irq_current_registers
    ldmia r12, {r0-r12, sp, lr}
    msr spsr, r12

    and r12, #0x1f
    cmp r12, #0x10
    bne ret_from_irq

    mov r12, sp
    cps #0x1f
    mov sp, r12
    cps #0x12
    */

ret_from_irq:
    cpsie i
    movs pc, lr


.global switch_process
switch_process:
    // Save the current thread's state
    push    {r3-r5, lr}
    ldr     r3, =irq_current_registers
    ldr     r4, [r0]

    // Restore the new thread's state
    ldr     r0, [r1]
    pop     {r0-r12}
    msr     spsr, r12   // Restore the new thread's program status register
    pop     {lr}
    pop     {lr}
    subs    pc, lr