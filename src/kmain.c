#include <sys/gdt.h>
#include <driver/video.h>

void kmain()
{
    video_screen_clean();
    video_print("YAUK !\n");

    gdt_init();
    while(1);
}
