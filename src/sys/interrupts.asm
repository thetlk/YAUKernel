extern int_default, int_clock, int_keyboard

global _asm_default_interrupt, _asm_irq_0, _asm_irq_1

_asm_default_interrupt:
    call int_default
    mov al,0x20
    out 0x20,al
    iret

_asm_irq_0:
    call int_clock
    mov al,0x20
    out 0x20,al
    iret

_asm_irq_1:
    call int_keyboard
    mov al,0x20
    out 0x20,al
    iret

