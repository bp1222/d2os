.section ".text.boot"
.globl _start
.globl _enable_interrupts

.equ    CPSR_MODE_IRQ,          0x12
.equ    CPSR_MODE_SVR,          0x13
.equ    CPSR_IRQ_INHIBIT,       0x40

.equ    SVR_STACK,        0x01F00000
.equ    IRQ_STACK,        0x01EF0000
.equ    USER_STACK,       0x01EE0000
.equ    ABORT_STACK,      0x01ED0000
.equ    UNDEFINED_STACK,  0x01EC0000

_start:
    ldr pc, _reset_h
    nop
    nop
    nop
    nop
    nop
    ldr pc, _interrupt_vector_h
    nop

    _reset_h:                           .word   _reset_
    nop
    nop
    nop
    nop
    nop
    _interrupt_vector_h:                .word   irq_handler
    nop

    _irq_stack:                         .word   IRQ_STACK
    _svr_stack:                         .word   SVR_STACK
    _user_stack:                        .word   USER_STACK
    _undefined_stack:                   .word   UNDEFINED_STACK
    _abort_stack:                       .word   ABORT_STACK

_reset_:
    mov     r0, #0x8000
    mov     r1, #0x0000
    ldmia   r0!,{r2, r3, r4, r5, r6, r7, r8, r9}
    stmia   r1!,{r2, r3, r4, r5, r6, r7, r8, r9}
    ldmia   r0!,{r2, r3, r4, r5, r6, r7, r8, r9}
    stmia   r1!,{r2, r3, r4, r5, r6, r7, r8, r9}

    mov r0, #(CPSR_MODE_IRQ | CPSR_IRQ_INHIBIT )
    msr cpsr_c, r0
    ldr sp, _irq_stack

    mov r0, #(CPSR_MODE_SVR | CPSR_IRQ_INHIBIT )
    msr cpsr_c, r0
    ldr sp, _svr_stack

    bl kernel_main

_enable_interrupts:
    mrs     r0, cpsr
    bic     r0, r0, #0x80
    msr     cpsr_c, r0
    mov     pc, lr
