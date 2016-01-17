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

void int_pagefault(void)
{
    unsigned int faulting_addr;
    video_print_color("\n---- page fault ----\n", COLOR(RED, WHITE));

    asm volatile("mov %%eax, %%cr2  ;"
                 "mov %0, %%eax     ;"
            : "=m" (faulting_addr)
            :
            :
        );

    video_print_color("faulting_addr = 0x", COLOR(RED, WHITE));
    video_print_number_color(faulting_addr, 16, COLOR(RED, WHITE));
    video_putchar('\n');
    asm("hlt");
}

void int_default(void)
{
}

void int_clock(void *regs)
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
