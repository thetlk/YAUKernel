#include <sys/syscall.h>
#include <driver/video.h>

int syscall_handle(int syscall_number,
                    int arg0,
                    int arg1,
                    int arg2,
                    int arg3,
                    int arg4)
{
    SYSCALL_DEBUG(syscall_number, arg0, arg1, arg2, arg3, arg4);
    return 0;
}
