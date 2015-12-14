#ifndef INTERRUPTS_H
#define INTERRUPTS_H

void _asm_default_interrupt(void);
void _asm_irq_0(void);
void _asm_irq_1(void);

void isr_default_int(void);
void isr_clock_int(void);
void isr_kbd_int(void);

#endif