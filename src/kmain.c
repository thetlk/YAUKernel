#include <sys/gdt.h>
#include <sys/idt.h>
#include <sys/asm.h>
#include <driver/pic.h>
#include <driver/video.h>

void kmain()
{
    video_screen_clean();
    video_print("YAUK !\n");

    gdt_init();
    idt_init();
    init_pic();

    sti(); // enable interrupts

    while(1);
}
