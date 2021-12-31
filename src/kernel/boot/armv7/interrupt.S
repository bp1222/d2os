#include <kernel/assembly.h>

.global _enable_interrupts
_enable_interrupts:
    mrs     r0, cpsr
    bic     r0, r0, #CPSR_IRQ_INHIBIT
    msr     cpsr_c, r0
    cpsie   i

    // Return from the function
    mov     pc, lr

.global _disable_interrupts
_disable_interrupts:
    mrs     r0, cpsr
    orr     r0, r0, #CPSR_IRQ_INHIBIT
    msr     cpsr_c, r0
    cpsid   i

    // Return from the function
    mov     pc, lr
    
.global _irq_handler_asm
_irq_handler_asm:
    sub     lr, lr, #4      // Adjsut return address
    srsdb   sp!, #0x13      // Save irq lr and irq spsp to supervisor stack, and save the resulting stack pointer as the current stack pointer
    cpsid   if, #0x13       // Switch to supervisor mode with interrupts disabled
    push    {r0-r3, r12, lr}// Save the caller save registers
    and     r1, sp, #4      // Make sure stack is 8 byte aligned
    sub     sp, sp, r1
    push    {r1}            // Save the stack adjustment
    bl      _irq_handler
    pop     {r1}            // Get the stack adjustment
    add     sp, sp, r1
    pop     {r0-r3, r12, lr}// Revert the caller save registers
    rfeia   sp!             // Load the saved return address and program state register from before the interrupt from the stack and return 

.global _interrupt_table
_interrupt_table:
    ldr pc, _reset_vector_h
    ldr pc, _undefined_instruction_vector_h
    ldr pc, _software_interrupt_vector_h
    ldr pc, _prefetch_abort_vector_h
    ldr pc, _data_abort_vector_h
    ldr pc, _unused_handler_h
    ldr pc, _interrupt_vector_h
    ldr pc, _fast_interrupt_vector_h

_reset_vector_h:                    .word  _reset_handler
_undefined_instruction_vector_h:    .word  _undefined_instruction_handler
_software_interrupt_vector_h:       .word  _software_interrupt_handler
_prefetch_abort_vector_h:           .word  _prefetch_abort_handler
_data_abort_vector_h:               .word  _data_abort_handler
_unused_handler_h:                  .word  _unused_handler
_interrupt_vector_h:                .word  _irq_handler
_fast_interrupt_vector_h:           .word  _fiq_handler