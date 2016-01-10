#include <sys/gdt.h>
#include <sys/idt.h>
#include <sys/asm.h>
#include <sys/syscall.h>
#include <driver/pic.h>
#include <driver/video.h>
#include <libc/mem.h>

void task1()
{
    asm volatile("mov %%eax, 0x42; int 0x30 ;" ::: );
    while(1);
}

void launch_task()
{
    video_print("launch_task ...\n");

    memcpy((unsigned char *) 0x30000, (unsigned char *) &task1, 100);

    asm volatile("cli;"
        "push 0x33 ;"
        "push 0x30000 ;"
        "pushf ;"
        "pop %%eax ;"
        "or %%eax, 0x200 ;"
        "and %%eax, 0xFFFFBFFF ;"
        "push %%eax ;"
        "push 0x23 ;"
        "push 0x0 ;"
        "mov %%ax, 0x2B ;"
        "mov %%ds, %%ax ;"
        "iret"
        :::
        );

    video_print_color("critical error, halting system\n", COLOR(RED, WHITE));
    asm volatile ("hlt");
    while(1);
}

void kmain()
{
    video_screen_clean();
    video_print("YAUK !\n");

    gdt_init();
    idt_init();
    pic_init();

    ltr(0x38); // TSS at 0x38 gdt entry
    set_ss_esp();

    // sti(); // enable interrupts

    launch_task();
    // while(1);
}
