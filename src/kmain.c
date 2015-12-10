#include <sys/gdt.h>
#include <sys/idt.h>
#include <sys/pic.h>
#include <driver/video.h>

void kmain()
{
    video_screen_clean();
    video_print("YAUK !\n");

    gdt_init();
    init_idt();
    init_pic();

    asm volatile ("sti");

    while(1);
}
