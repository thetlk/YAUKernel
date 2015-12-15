#include <sys/interrupts.h>
#include <driver/video.h>

void int_default(void)
{
    video_print("default int\n");
}

void int_clock(void)
{
    static int tic = 0;
    static int sec = 0;
    tic++;
    if (tic % 100 == 0) {
        sec++;
        tic = 0;
        video_print("clock\n");
    }
}

void int_keyboard(void)
{
    video_print("keyboard\n");
}
