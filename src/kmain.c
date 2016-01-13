#include <sys/gdt.h>
#include <sys/idt.h>
#include <sys/asm.h>
#include <sys/syscall.h>
#include <sys/pagemem.h>
#include <driver/pic.h>
#include <driver/video.h>
#include <libc/mem.h>

void task1()
{
    char *msg = (char*) 0x100; // ds:0x100
    msg[0] = 't';
    msg[1] = 'a';
    msg[2] = 's';
    msg[3] = 'k';
    msg[4] = '1';
    msg[5] = '\n';

    asm volatile("mov %%ecx, 6      ;"
                 "mov %%ebx, %0     ;"
                 "mov %%eax, 0x01   ;"
                 "int 0x30          ;"
            :
            : "m" (msg)
            :
        );

    while(1);
}

void launch_task()
{
    video_print("launch_task ...\n");

    memcpy((unsigned char *) 0x30000, (unsigned char *) &task1, 100);

    asm volatile("cli;"
        "push 0x33 ;"               // ss
        "push 0x30000 ;"            // esp
        "pushf ;"
        "pop %%eax ;"
        "or %%eax, 0x200 ;"         // set IF
        "and %%eax, 0xFFFFBFFF ;"   // unset NT
        "push %%eax ;"              // eflags
        "push 0x23 ;"               // cs
        "push 0x0 ;"                // eip
        "mov %%ax, 0x2B ;"
        "mov %%ds, %%ax ;"          // user data segment
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
    pagemem_init();

    ltr(0x38); // TSS at 0x38 gdt entry
    set_ss_esp();

    sti(); // enable interrupts

    launch_task();
    // while(1);
}
