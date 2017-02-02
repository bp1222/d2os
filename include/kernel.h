#ifndef __KERNEL_H__
#define __KERNEL_H__

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint32_t;
typedef unsigned long ulong;
typedef char bool;
typedef unsigned long size_t;

#define NULL 0x0

inline void delay(int count)
{
    asm volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n"
        : "=r"(count): [count]"0"(count) : "cc");
}

#endif
