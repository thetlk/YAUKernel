#include <boot/multiboot.h>
#include <sys/gdt.h>
#include <sys/idt.h>
#include <sys/asm.h>
#include <sys/syscall.h>
#include <sys/pagemem.h>
#include <sys/memory.h>
#include <sys/task.h>
#include <sys/kmalloc.h>
#include <driver/pic.h>
#include <driver/video.h>

void task1()
{
    int i;
    char *msg = (char*) 0x40000100;
    msg[0] = 't';
    msg[1] = 'a';
    msg[2] = 's';
    msg[3] = 'k';
    msg[4] = '1';
    msg[5] = '\n';

    while(1)
    {
        asm volatile("mov %%ecx, 6     ;"
                 "mov %%ebx, %0     ;"
                 "mov %%eax, 0x01   ;"
                 "int 0x30          ;"
            :
            : "m" (msg)
            :
        );
        for(i=0; i<10000; i++);
    }
}

void task2()
{
    int i;
    char *msg = (char*) 0x40000100;
    msg[0] = 't';
    msg[1] = 'a';
    msg[2] = 's';
    msg[3] = 'k';
    msg[4] = '2';
    msg[5] = '\n';

    while(1)
    {
        asm volatile("mov %%ecx, 6      ;"
                 "mov %%ebx, %0     ;"
                 "mov %%eax, 0x01   ;"
                 "int 0x30          ;"
            :
            : "m" (msg)
            :
        );
        for(i=0; i<10000000; i++);
    }
}

void kmain_continue(struct multiboot_info *mbi)
{
    idt_init();
    pic_init();

    memory_init(mbi);
    pagemem_init();
    kmalloc_init();

    // task_load((void*) 0x100000, &task1, 0x1000); // segfault here
    // task_load((void*) 0x200000, &task2, 0x1000);

    video_print_color("Enable interrupts !\n", COLOR(WHITE, GREEN));
    // sti(); // enable interrupts

    while(1);
}

void kmain(unsigned long magic, struct multiboot_info *mbi)
{
    video_screen_clean();
    video_print("YAUK !\n");

    if(magic != MULTIBOOT_BOOTLOADER_MAGIC)
    {
        video_printf("Bad magic - unable to boot :( (0x%x)\n", magic);
        asm volatile("hlt");
    }

    multiboot_display(mbi);
    
    gdt_init();
    set_ss_esp();

    kmain_continue(mbi);
}
