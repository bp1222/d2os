
// Part of the Raspberry-Pi Bare Metal Tutorials
// https://www.valvers.com/rpi/bare-metal/
// Copyright (c) 2013-2020, Brian Sidebotham

// This software is licensed under the MIT License.
// Please see the LICENSE file included with this software.

// References for RPI2 and above parts of this boot code:

// https://wiki.osdev.org/Raspberry_Pi_Bare_Bones

// Go and read and absorb this quite excellent startup code. It's really nice:
// https://github.com/LdB-ECM/Raspberry-Pi/blob/master/SD_FAT32/SmartStart32.S

#include <kernel/assembly.h>

.section ".text.boot"
.code	32
.align	2

// At the start address we have a "jump table", specifically laid out to allow jump to an address that is stored in
// memory. This table must be laid out exactly as shown (including the instruction ldr pc,)
.global _start
_start:
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

_reset_handler:
    // Skip the Hypervisor mode check and core parking when RPI0/1
    mrc MIDR(r11)
    ldr r10, =MAINID_ARMV6
    cmp r11, r10
    beq _setup_interrupt_table

    // Did we start up in hypervisor mode? If we didn't go ahead an park the cpus, we can then get back to SVC mode
    // later on
    mrs r12, cpsr
    and r12, #CPSR_MODE_MASK
    cmp r12, #CPSR_MODE_HYPERVISOR
    bne _multicore_park

    // We're in hypervisor mode and we need to switch back in order to allow us to continue successfully
    bic r12, r12, #CPSR_MODE_MASK
    orr r12, r12, #(CPSR_MODE_SVC | CPSR_IRQ_INHIBIT | CPSR_FIQ_INHIBIT)
    msr cpsr_c, r12

_multicore_park:
    mrc MPIDR(r12)
    ands r12, #0x3
    bne _inf_loop

_setup_interrupt_table:
    // Preserve r0, r1, r2 for kernel
    ldr     r3, =_start
    mov     r4, #0x0000
    
    ldmia   r3!,{r5-r12}
    stmia   r4!,{r5-r12}
    ldmia   r3!,{r5-r12}
    stmia   r4!,{r5-r12}

    // We're going to use interrupt mode, so setup the interrupt mode
    // stack pointer which differs to the application stack pointer:
    mov r3, #(CPSR_MODE_IRQ | CPSR_IRQ_INHIBIT | CPSR_FIQ_INHIBIT)
    msr cpsr_c, r3
    ldr sp, =__irq_stack_top

    // Switch back to supervisor mode (our application mode) and
    // set the stack pointer. Remember that the stack works its way
    // down memory, our heap will work it's way up from after the
    // application.
    mov r3, #(CPSR_MODE_SVC | CPSR_IRQ_INHIBIT | CPSR_FIQ_INHIBIT)
    msr cpsr_c, r3
    ldr sp, =__svc_stack_top_core0

    // Handoff to C
    bl _cstartup

    // If main does return for some reason, just catch it and stay here.
_inf_loop:
    wfe
    b _inf_loop
