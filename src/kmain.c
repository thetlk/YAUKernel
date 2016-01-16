#include <sys/gdt.h>
#include <sys/idt.h>
#include <sys/asm.h>
#include <sys/syscall.h>
#include <sys/pagemem.h>
#include <sys/task.h>
#include <driver/pic.h>
#include <driver/video.h>
#include <libc/mem.h>

void task1()
{
    // int i;
    char *msg = (char*) 0x40000100;
    msg[0] = '1';

    while(1)
    {
        asm volatile("mov %%ecx, 1      ;"
                 "mov %%ebx, %0     ;"
                 "mov %%eax, 0x01   ;"
                 "int 0x30          ;"
            :
            : "m" (msg)
            :
        );
        // for(i=0; i<1000000; i++);
    }
}

void task2()
{
    // int i;
    char *msg = (char*) 0x40000100;
    msg[0] = '2';

    while(1)
    {
        asm volatile("mov %%ecx, 1      ;"
                 "mov %%ebx, %0     ;"
                 "mov %%eax, 0x01   ;"
                 "int 0x30          ;"
            :
            : "m" (msg)
            :
        );
        // for(i=0; i<10000000; i++);
    }
}

void kmain_continue()
{
    idt_init();
    pic_init();

    video_print("Load TSS ... ");
    ltr(0x38); // TSS at 0x38 gdt entry
    video_print_ok();

    pagemem_init();

    task_load((void*) 0x100000, &task1, 0x1001);
    task_load((void*) 0x200000, &task2, 0x1001);

    video_print_color("Enable interrupts !\n", COLOR(WHITE, GREEN));
    sti(); // enable interrupts

    while(1);
}


void kmain()
{
    video_screen_clean();
    video_print("YAUK !\n");
    
    gdt_init();
    set_ss_esp();

    kmain_continue();
}
