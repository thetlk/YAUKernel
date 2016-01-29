#include <sys/syscall.h>
#include <sys/asm.h>
#include <driver/video.h>

unsigned int syscall_handle(unsigned int syscall_number,
                    unsigned int arg0,
                    unsigned int arg1,
                    unsigned int arg2,
                    unsigned int arg3,
                    unsigned int arg4)
{
    unsigned int ret = 0;

    switch(syscall_number)
    {
        case SYS_WRITE:
            ret = sys_write((char *) arg0, arg1);
            break;

        default:
            SYSCALL_DEBUG(syscall_number, arg0, arg1, arg2, arg3, arg4);
            video_print_color("unhandled syscall number\n", COLOR(RED, WHITE));
            ret = 0xFFFFFFFF;
            break;
    }

    return ret;
}

unsigned int sys_write(char *data, unsigned int size)
{
    unsigned int i;

    cli();
    for(i=0; i<size; i++)
    {
        video_putchar(data[i]);
    }
    sti();

    return size;
}
