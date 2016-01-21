#ifndef INTERRUPTS_H
#define INTERRUPTS_H

struct pushed_registers
{
    unsigned int gs;
    unsigned int fs;
    unsigned int es;
    unsigned int ds;
    unsigned int edi;
    unsigned int esi;
    unsigned int ebp;
    unsigned int esp_kernel;
    unsigned int ebx;
    unsigned int edx;
    unsigned int ecx;
    unsigned int eax;
    unsigned int eip;
    unsigned int cs;
    unsigned int eflags;
    unsigned int esp;
    unsigned int ss;
};

void _asm_default_interrupt(void);
void _asm_gp(void);
void _asm_pf(void);
void _asm_irq_0(void);
void _asm_irq_1(void);
void _asm_syscalls(void);

void int_default(void);
void int_clock(struct pushed_registers *regs);
void int_keyboard(void);
void int_pagefault(struct pushed_registers *regs);


#endif