extern syscall_handle
extern int_default
extern int_pagefault
extern int_clock, int_keyboard

global _asm_syscalls
global _asm_default_interrupt
global _asm_pf
global _asm_irq_0, _asm_irq_1

%macro SAVE_REGS 0
    pushad
    push ds
    push es
    push fs
    push gs
    push ebx
    mov bx, 0x10
    mov ds, bx
    pop ebx
%endmacro

%macro RESTORE_REGS 0
    pop gs
    pop fs
    pop es
    pop ds
    popad
%endmacro

%macro END_OF_INTERRUPT 0
    push eax
    mov al, 0x20
    out 0x20, al
    pop eax
%endmacro

    ; page fault
_asm_pf:
    SAVE_REGS
    END_OF_INTERRUPT
    RESTORE_REGS
    iret

_asm_default_interrupt:
    SAVE_REGS
    call int_default
    END_OF_INTERRUPT
    RESTORE_REGS
    iret

_asm_irq_0:
    SAVE_REGS
    call int_clock
    END_OF_INTERRUPT
    RESTORE_REGS
    iret

_asm_irq_1:
    SAVE_REGS
    call int_keyboard
    END_OF_INTERRUPT
    RESTORE_REGS
    iret

_asm_syscalls:
    ; we're not ussing pushad & co because
    ; we need to preserve eax return value
    ; save register values
    push eax
    push ecx
    push edx
    push ebx
    push esp
    push ebp
    push esi
    push edi
    push ds
    push es
    push fs
    push gs
    ; set good ds value
    push ebx
    mov bx, 0x10
    mov ds, bx
    pop ebx
    ; syscall arguments
    push esi
    push edi
    push edx
    push ecx
    push ebx
    push eax
    call syscall_handle
    ; cleanup pushed args
    add esp, 0x18
    ; send END_OF_INTERRUPT
    END_OF_INTERRUPT
    ; restore registers, but not eax
    pop gs
    pop fs
    pop es
    pop ds
    pop edi
    pop esi
    pop ebp
    pop esp
    pop ebx
    pop edx
    pop ecx
    add esp, 0x4 ; instead of pop eax
    iret
