#include <sys/interrupts.h>
#include <sys/asm.h>
#include <driver/video.h>
#include <driver/keyboard.h>

void int_default(void)
{
    // video_print("default int\n");
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
    unsigned char code;

    do {
        code = inb(0x64);
    } while((code & 0x01) == 0);

    code = inb(0x60);
    keyboard_handle_code(code);
}
