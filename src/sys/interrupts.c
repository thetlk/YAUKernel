#include <sys/interrupts.h>
#include <sys/asm.h>
#include <sys/scheduler.h>
#include <driver/video.h>
#include <driver/keyboard.h>

void int_generalprotection()
{
    video_print_color("\n----- general protection fault -----\n", COLOR(RED, WHITE));
    asm("hlt");
}

void int_pagefault(struct pushed_registers *regs)
{
    unsigned int faulting_addr;

    asm volatile("mov %%eax, %%cr2  ;"
                 "mov %0, %%eax     ;"
            : "=m" (faulting_addr)
            :
            :
        );

    video_print_color("\n---- page fault ----\n", COLOR(RED, WHITE));

    video_printf("faulting_addr = %p\n", faulting_addr);
    video_printf("gs = %p\n", regs->gs);
    video_printf("fs = %p\n", regs->fs);
    video_printf("es = %p\n", regs->es);
    video_printf("ds = %p\n", regs->ds);
    video_printf("edi = %p\n", regs->edi);
    video_printf("esi = %p\n", regs->esi);
    video_printf("ebp = %p\n", regs->ebp);
    video_printf("esp = %p\n", regs->esp);
    video_printf("ebx = %p\n", regs->ebx);
    video_printf("edx = %p\n", regs->edx);
    video_printf("ecx = %p\n", regs->ecx);
    video_printf("eax = %p\n", regs->eax);
    video_printf("eip = %p\n", regs->eip);
    video_printf("cs = %p\n", regs->cs);
    video_printf("eflags = %p\n", regs->eflags);
    video_printf("esp = %p\n", regs->esp);
    video_printf("ss = %p\n", regs->ss);

    asm("hlt");
}

void int_default(void)
{
}

void int_clock(struct pushed_registers *regs)
{
    schedule(regs);
}

void int_keyboard(void)
{
    unsigned char code;

    do {
        code = inb(0x64);
    } while((code & 0x01) == 0);

    code = inb(0x60);
    keyboard_handle_code(code);
}
