/*
 * Copyright (c) 2004,2012 Kustaa Nyholm / SpareTimeLabs
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this list
 * of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice, this
 * list of conditions and the following disclaimer in the documentation and/or other
 * materials provided with the distribution.
 *
 * Neither the name of the Kustaa Nyholm or SpareTimeLabs nor the names of its
 * contributors may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 */
#include <kernel/utils/printk.h>

typedef void (*putcf) (void*,char);
static putcf stdout_putf;
static void* stdout_putp;

static void ui2a(unsigned int num, unsigned int base, int uc,char * bf)
{
    int n=0;
    unsigned int d=1;
    while (num / d >= base)
        d*=base;
    while (d!=0) {
        int dgt = num / d;
        num %= d;
        d /= base;
        if (n || dgt>0 || d==0) {
            *bf++ = dgt+(dgt<10 ? '0' : (uc ? 'A' : 'a')-10);
            ++n;
        }
    }
    *bf=0;
}

static void i2a (int num, char * bf)
{
    if (num<0) {
        num=-num;
        *bf++ = '-';
    }
    ui2a(num,10,0,bf);
}

static int a2d(char ch)
{
    if (ch>='0' && ch<='9')
        return ch-'0';
    else if (ch>='a' && ch<='f')
        return ch-'a'+10;
    else if (ch>='A' && ch<='F')
        return ch-'A'+10;
    else return -1;
}

static char a2i(char ch, char** src,int base,int* nump)
{
    char* p= *src;
    int num=0;
    int digit;
    while ((digit=a2d(ch))>=0) {
        if (digit>base) break;
        num=num*base+digit;
        ch=*p++;
    }
    *src=p;
    *nump=num;
    return ch;
}

static void putchw(void* putp, putcf putf, int n, char z, char* bf)
{
    char fc=z? '0' : ' ';
    char ch;
    char* p=bf;
    while (*p++ && n > 0)
        n--;
    while (n-- > 0)
        putf(putp,fc);
    while ((ch= *bf++))
        putf(putp,ch);
}

static void putbin(unsigned int val, char* buff, int sz)
{
    char *pbuff = buff;

    /* Must be able to store one character at least. */
    if (sz < 1) return;

    /* Special case for zero to ensure some output. */
    if (val == 0) {
        *pbuff++ = '0';
        *pbuff = '\0';
        return;
    }

    /* Work from the end of the buffer back. */
    pbuff += sz - 1;

    /* For each bit (going backwards) store character. */
    while (sz-- > 0) {
        *pbuff-- = ((val & 1) == 1) ? '1' : '0';
        val >>= 1;
    }
}

void tfp_format(void* putp, putcf putf, char *fmt, va_list va)
{
    char bf[12];
    char buf[32];

    char ch;

    while ((ch=*(fmt++))) {
        if (ch!='%')
            putf(putp,ch);
        else {
            char lz=0;
            int w=0;
            ch=*(fmt++);
            if (ch=='0') {
                ch=*(fmt++);
                lz=1;
            }
            if (ch>='0' && ch<='9') {
                ch=a2i(ch,&fmt,10,&w);
            }
            switch (ch) {
            case 0:
                break;
            case 'b':
                putbin(va_arg(va, unsigned int), buf, 32);
                putchw(putp, putf, w, 0, buf);
                break;
            case 'u':
                ui2a(va_arg(va, unsigned int), 10, 0, bf);
                putchw(putp, putf, w, lz, bf);
                break;
            case 'd':
                i2a(va_arg(va, int), bf);
                putchw(putp, putf, w, lz, bf);
                break;
            case 'x':
            case 'X':
                ui2a(va_arg(va, unsigned int), 16, (ch == 'X'), bf);
                putchw(putp, putf, w, lz, bf);
                break;
            case 'c':
                putf(putp, (char)(va_arg(va, int)));
                break;
            case 's':
                putchw(putp, putf, w, 0, va_arg(va, char *));
                break;
            case '%':
                putf(putp, ch);
            default:
                break;
            }
        }
    }
}

void init_printk(void* putp, void (*putf) (void*,char))
{
    stdout_putf=putf;
    stdout_putp=putp;
}

void tfp_printk(char *fmt, ...)
{
    va_list va;
    va_start(va,fmt);
    tfp_format(stdout_putp,stdout_putf,fmt,va);
    va_end(va);
}

static void putcp(void* p,char c)
{
    *(*((char**)p))++ = c;
}

void tfp_sprintk(char* s,char *fmt, ...)
{
    va_list va;
    va_start(va,fmt);
    tfp_format(&s,putcp,fmt,va);
    putcp(&s,0);
    va_end(va);
}