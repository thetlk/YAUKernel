#include <sys/gdt.h>
#include <sys/idt.h>
#include <sys/asm.h>
#include <driver/pic.h>
#include <driver/video.h>

// void task1()
// {
// 	video_print("task1 running\n");
// 	while(1);
// }

// void launch_task()
// {
// 	video_print("launch_task ...\n");



// 	video_print_color("critical error, halting system\n", COLOR(RED, WHITE));
// 	asm volatile ("hlt");
// 	while(1);
// }

void kmain()
{
    video_screen_clean();
    video_print("YAUK !\n");

    gdt_init();
    idt_init();
    pic_init();

    ltr(0x38); // TSS at 0x38 gdt entry
    set_ss_esp();

    sti(); // enable interrupts

    // launch_task();
    while(1);
}
