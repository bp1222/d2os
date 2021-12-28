
// Part of the Raspberry-Pi Bare Metal Tutorials
// https://www.valvers.com/rpi/bare-metal/
// Copyright (c) 2013-2020, Brian Sidebotham

// This software is licensed under the MIT License.
// Please see the LICENSE file included with this software.

// References for RPI2 and above parts of this boot code:

// https://wiki.osdev.org/Raspberry_Pi_Bare_Bones

// Go and read and absorb this quite excellent startup code. It's really nice:
// https://github.com/LdB-ECM/Raspberry-Pi/blob/master/SD_FAT32/SmartStart32.S

.section ".text.boot"

.global _start
.global _get_stack_pointer
.global _exception_table

#include <kernel/assembly.h>

.code	32
.align	2

// At the start address we have a "jump table", specifically laid out to allow jump to an address that is stored in
// memory. This table must be laid out exactly as shown (including the instruction ldr pc,)
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
    // Determine what mode the cpu has started up in. When we get to the very start of the ARM execution we can
    // extract the Current Program Status Register value and mask off the Mode field which is defined in the ARM
    // Architecture Reference Manual for ARMV6 ARMV7 and ARMV8 as M[4:0] - in other words the 5 lowest bits
    // Do this in a high register so as not to overwrite anything the bootloader has passed over (such as atags)
    mrs r12, CPSR
    and r12, #CPSR_MODE_MASK    // Zero all bits except the CPSR_MODE_MASK bits to be left with the mode value in r11

    // Store the CPSR start mode in a ":global variable" that is accessible to all (including the C world)
    ldr r11, =_cpsr_startup_mode
    str r12, [r11]

    // Check to see if we're BCM2385 (RPI0/1) - these processors only have one core and so we can go ahead and
    // continue executing the startup code. However, all other RPI models have processors which have multiple cores.
    // When the ARM processor starts all four processors run the same code. 'tis weird, n'est pas?
    // In order to get some sanity back to our world we "park" the remaining cores and just work with one core. We're
    // not yet at the point of requiring parallel processing.

    // Skip the Hypervisor mode check and core parking when RPI0/1
    mrc MIDR(r11)
    ldr r10, =MAINID_ARMV6
    cmp r11, r10
    beq _setup_interrupt_table

    // Did we start up in hypervisor mode? If we didn't go ahead an park the cpus, we can then get back to SVC mode
    // later on
    cmp r12, #CPSR_MODE_HYPERVISOR
    bne _multicore_park

    // We're in hypervisor mode and we need to switch back in order to allow us to continue successfully
    mrs r12, cpsr
    bic r12, r12, #CPSR_MODE_MASK
    orr r12, r12, #(CPSR_MODE_SVC | CPSR_IRQ_INHIBIT | CPSR_FIQ_INHIBIT )
    msr cpsr_c, r12

_multicore_park:
    // On RPI2/3 make sure all cores that are not core 0 branch off to an infinite loop to make them enter a spinlock
    // We will then only operate with core 0 and setup stack pointers and the like for core 0
    mrc IDCR(r12)
    ands r12, #0x3
    bne _inf_loop

_setup_interrupt_table:
    // Preserve r0, r1, r2 for kernel
    ldr     r3, =_start
    mov     r4, #0x0000
    
    ldmia   r3!,{r5, r6, r7, r8, r9, r10, r11, r12}
    stmia   r4!,{r5, r6, r7, r8, r9, r10, r11, r12}
    ldmia   r3!,{r5, r6, r7, r8, r9, r10, r11, r12}
    stmia   r4!,{r5, r6, r7, r8, r9, r10, r11, r12}

    // We're going to use interrupt mode, so setup the interrupt mode
    // stack pointer which differs to the application stack pointer:
    mov r3, #(CPSR_MODE_IRQ | CPSR_IRQ_INHIBIT | CPSR_FIQ_INHIBIT)
    msr cpsr_c, r3
    ldr sp, =0x4000

    // Switch back to supervisor mode (our application mode) and
    // set the stack pointer. Remember that the stack works its way
    // down memory, our heap will work it's way up from after the
    // application.
    mov r3, #(CPSR_MODE_SVC | CPSR_IRQ_INHIBIT | CPSR_FIQ_INHIBIT)
    msr cpsr_c, r3
    ldr sp, =0x8000

#if 0
    // Enable L1 Cache
    // R3 = System Control Register
    mrc p15,#0,r3,c1,c0,#0
    // Enable caches and branch prediction
    orr r3,#SCTLR_ENABLE_BRANCH_PREDICTION
    orr r3,#SCTLR_ENABLE_DATA_CACHE
    orr r3,#SCTLR_ENABLE_INSTRUCTION_CACHE
    // System Control Register = R3
    mcr p15,0,r3,c1,c0,0

    // Enable VFP 
    // r3 = Access Control Register
    mrc p15,#0,r3,c1,c0,#2
    // enable full access for p10,11
    orr r3, r3, #(0xf << 20)
    // Access Control Register = r3
    mcr p15,#0,r3,c1,c0,#2
    // Set FPEXC EN bit
    mov r3,#0x40000000
    // fpexc = r3
    vmsr fpexc, r3
#endif

    // Handoff to C
    bl _cstartup

    // If main does return for some reason, just catch it and stay here.
.global _inf_loop
_inf_loop:
    wfe
    b _inf_loop

// A 32-bit value that represents the processor mode at startup
_cpsr_startup_mode:  .word    0x0

_get_stack_pointer:
    // Return the stack pointer value
    str     sp, [sp]
    ldr     r0, [sp]

    // Return from the function
    mov     pc, lr
