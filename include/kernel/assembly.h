#ifndef __ASSEMBLY_H__
#define __ASSEMBLY_H__

#ifdef __ASSEMBLER__

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
#define CPSR_THUMB 0x20

// See ARM section B3.3
// The value can be decoded into constituent parts but can be gathered easily by running
// cat /proc/cpuinfo when booting any of the RPi boards into Linux
#define MAINID_ARMV6 0x410FB767
#define MAINID_ARMV7 0x410FC073
#define MAINID_ARMV8 0x410FD034

// Register Helpers
#define MIDR(reg) p15, 0, reg, c0, c0, 0
#define IDCR(reg) p15, 0, reg, c0, c0, 5
#define SCTLR(reg) p15, 0, reg, c1, c0, 0
#define CLIDR(reg) p15, 1, reg, c0, c0, 1
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