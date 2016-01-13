#ifndef INTERRUPTS_H
#define INTERRUPTS_H

void _asm_default_interrupt(void);
void _asm_pf(void);
void _asm_irq_0(void);
void _asm_irq_1(void);
void _asm_syscalls(void);

void int_default(void);
void int_clock(void);
void int_keyboard(void);

#endif