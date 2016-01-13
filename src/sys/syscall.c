#include <sys/syscall.h>
#include <driver/video.h>

unsigned int syscall_handle(unsigned int syscall_number,
                    unsigned int arg0,
                    unsigned int arg1,
                    unsigned int arg2,
                    unsigned int arg3,
                    unsigned int arg4)
{
    unsigned int ret = 0;
    SYSCALL_DEBUG(syscall_number, arg0, arg1, arg2, arg3, arg4);

    switch(syscall_number)
    {
        case SYS_WRITE:
            ret = sys_write((unsigned char *) arg0, arg1);
            break;

        default:
            video_print_color("unhandled syscall number\n", COLOR(RED, WHITE));
            ret = -1;
            break;
    }

    return ret;
}

unsigned int sys_write(unsigned char *data, unsigned int size)
{
    unsigned int i;

    // assume data is on userland data segment
    // we have to add DS offset on data
    data += 0x30000; // beurk.

    for(i=0; i<size; i++)
    {
        video_putchar(data[i]);
    }

    return size;
}
