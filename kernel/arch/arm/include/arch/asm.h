#ifndef __ASM_ASM_H__
#define __ASM_ASM_H__

#ifdef __ASSEMBLER__

// Special Offset Values
#define CPU_CONTEXT_SIZE (15 * 4)
#define C_R0 (0 * 4)
#define C_R1 (1 * 4)
#define C_R2 (2 * 4)
#define C_R3 (3 * 4)
#define C_R4 (4 * 4)
#define C_R5 (5 * 4)
#define C_R6 (6 * 4)
#define C_R7 (7 * 4)
#define C_R8 (8 * 4)
#define C_R9 (9 * 4)
#define C_R10 (10 * 4)
#define C_R11 (11 * 4)
#define C_R12 (12 * 4)
#define C_SP (13 * 4)
#define C_LR (14 * 4)
#define C_PC (15 * 4)
#define C_SPSR (16 * 4)

// CPSR Mode
#define CPSR_MODE_USER 0x10
#define CPSR_MODE_FIQ 0x11
#define CPSR_MODE_IRQ 0x12
#define CPSR_MODE_SVC 0x13
#define CPSR_MODE_ABORT 0x17
#define CPSR_MODE_HYPERVISOR 0x1A
#define CPSR_MODE_UNDEFINED 0x1B
#define CPSR_MODE_SYSTEM 0x1F
#define CPSR_MODE_MASK 0x1F

// See ARM section A2.5 (Program status registers)
#define CPSR_IRQ_INHIBIT 0x80
#define CPSR_FIQ_INHIBIT 0x40

// See ARM section B3.3
// The value can be decoded into constituent parts but can be gathered easily by running
// cat /proc/cpuinfo when booting any of the RPi boards into Linux

// Register Helpers
#define MIDR(reg) p15, 0, reg, c0, c0, 0
#define MPIDR(reg) p15, 0, reg, c0, c0, 5
#define CLIDR(reg) p15, 1, reg, c0, c0, 1
#define SCTLR(reg) p15, 0, reg, c1, c0, 0
#define CPACR(reg) p15, 0, reg, c1, c0, 2
#define TTBR0(reg) p15, 0, reg, c2, c0, 0
#define TTBCR(reg) p15, 0, reg, c2, c0, 2
#define DACR(reg) p15, 0, reg, c3, c0, 0
#define IIALL(reg) p15, 0, reg, c7, c5, 0
#define BPIALL(reg) p15, 0, reg, c7, c5, 6
#define DTLBIALL(reg) p15, 0, reg, c8, c6, 0
#define TLBIALL(reg) p15, 0, reg, c8, c7, 0
#define DTLBIASID(reg) p15, 0, reg, c8, c6, 2
#define TLBIASID(reg) p15, 0, reg, c8, c7, 2
#define CONTEXTIDR(reg) p15, 0, reg, c13, c0, 1

#else
#error "Including assembly header in C"
#endif

#endif