#include <sys/interrupts.h>
#include <driver/video.h>

void isr_default_int(void)
{
    video_print("default int\n");
}

void isr_clock_int(void)
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

void isr_kbd_int(void)
{
    video_print("keyboard\n");
}
