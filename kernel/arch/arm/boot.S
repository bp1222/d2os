.equ MAINID_ARMV6, 0x410FB767
.equ MAINID_ARMV7, 0x410FC073

.equ CPSR_MODE_USER, 0x10
.equ CPSR_MODE_FIQ, 0x11
.equ CPSR_MODE_IRQ, 0x12
.equ CPSR_MODE_SVC, 0x13
.equ CPSR_MODE_ABORT, 0x17
.equ CPSR_MODE_HYPERVISOR, 0x1A
.equ CPSR_MODE_UNDEFINED, 0x1B
.equ CPSR_MODE_SYSTEM, 0x1F
.equ CPSR_MODE_MASK, 0x1F

// See ARM section A2.5 (Program status registers)
.equ CPSR_IRQ_INHIBIT, 0x80
.equ CPSR_FIQ_INHIBIT, 0x40

.section ".text.boot"
.code	32
.align	2

.global _start
_start:
    bl _reset_handler
    
// Preserve r0, r1, r2 for kernel
.global _reset_handler
_reset_handler:
    mrc p15, 0, r11, c0, c0, 0
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
    mrc p15, 0, r12, c0, c0, 5
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



/*


    // Determine what mode the cpu has started up in. When we get to the very start of the ARM execution we can
    // extract the Current Program Status Register value and mask off the Mode field which is defined in the ARM
    // Architecture Reference Manual for ARMV6 ARMV7 and ARMV8 as M[4:0] - in other words the 5 lowest bits
    // Do this in a high register so as not to overwrite anything the bootloader has passed over (such as atags)
    mrs r12, CPSR
    and r12, #CPSR_MODE_MASK    // Zero all bits except the CPSR_MODE_MASK bits to be left with the mode value in r11

    // Store the CPSR start mode in a "global variable" that is accessible to all (including the C world)
    ldr r11, =_cpsr_startup_mode
    str r12, [r11]

    // Check to see if we're BCM2385 (RPI0/1) - these processors only have one core and so we can go ahead and
    // continue executing the startup code. However, all other RPI models have processors which have multiple cores.
    // When the ARM processor starts all four processors run the same code. 'tis weird, n'est pas?
    // In order to get some sanity back to our world we "park" the remaining cores and just work with one core. We're
    // not yet at the point of requiring parallel processing.

    // Skip the Hypervisor mode check and core parking when RPI0/1
    mrc p15, 0, r11, c0, c0, 0
    ldr r10, =#MAINID_ARMV6
    cmp r11, r10
    beq _setup_interrupt_table

    // Did we start up in hypervisor mode? If we didn't go ahead an park the cpus, we can then get back to SVC mode
    // later on
    cmp r12, #CPSR_MODE_HYPERVISOR
    bne _multicore_park

    // We're in hypervisor mode and we need to switch back in order to allow us to continue successfully
    mrs r12, CPSR
    bic r12, r12, #CPSR_MODE_MASK
    orr r12, r12, #(CPSR_MODE_SVR | CPSR_IRQ_INHIBIT | CPSR_FIQ_INHIBIT )
    msr SPSR_cxsf, r12

    add lr, pc, #4
    .word 0xE12EF30E
    .word 0xE160006E

_multicore_park:
    // On RPI2/3 make sure all cores that are not core 0 branch off to an infinite loop to make them enter a spinlock
    // We will then only operate with core 0 and setup stack pointers and the like for core 0
    mrc p15, 0, r12, c0, c0, 5
    ands r12, #0x3
    bne _inf_loop

_setup_interrupt_table:

    mov     r0, #0x8000
    mov     r1, #0x0000
    ldmia   r0!,{r2, r3, r4, r5, r6, r7, r8, r9}
    stmia   r1!,{r2, r3, r4, r5, r6, r7, r8, r9}
    ldmia   r0!,{r2, r3, r4, r5, r6, r7, r8, r9}
    stmia   r1!,{r2, r3, r4, r5, r6, r7, r8, r9}

    // We're going to use interrupt mode, so setup the interrupt mode
    // stack pointer which differs to the application stack pointer:
    mov r0, #(CPSR_MODE_IRQ | CPSR_IRQ_INHIBIT | CPSR_FIQ_INHIBIT )
    msr cpsr_c, r0
    ldr sp, =0x7000

    // Switch back to supervisor mode (our application mode) and
    // set the stack pointer. Remember that the stack works its way
    // down memory, our heap will work it's way up from after the
    // application.
    mov r0, #(CPSR_MODE_SVR | CPSR_IRQ_INHIBIT | CPSR_FIQ_INHIBIT )
    msr cpsr_c, r0
    ldr sp, =0x8000

    // Enable VFP ------------------------------------------------------------

    // r1 = Access Control Register
    MRC p15, #0, r1, c1, c0, #2
    // enable full access for p10,11
    ORR r1, r1, #(0xf << 20)
    // Access Control Register = r1
    MCR p15, #0, r1, c1, c0, #2
    MOV r1, #0
    // flush prefetch buffer because of FMXR below
    MCR p15, #0, r1, c7, c5, #4
    // and CP 10 & 11 were only just enabled
    // Enable VFP itself
    MOV r0,#0x40000000
    // FPEXC = r0
    FMXR FPEXC, r0

*/