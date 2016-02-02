#include <sys/syscall.h>
#include <sys/asm.h>
#include <driver/video.h>

syscall_handler_func_t syscall_handler_table[SYSCALL_MAX];

int syscall_set_handler(unsigned int number, syscall_handler_func_t func)
{
    int ret = -1;

    if(number < SYSCALL_MAX)
    {
        syscall_handler_table[number] = func;
    }

    return ret;
}

void syscall_init()
{
    unsigned int i;

    for(i=0; i<SYSCALL_MAX; i++)
    {
        syscall_set_handler(i, 0);
    }

    syscall_set_handler(SYS_WRITE, (syscall_handler_func_t) sys_write);
}

int syscall_handle(unsigned int syscall_number,
                    unsigned int arg0,
                    unsigned int arg1,
                    unsigned int arg2,
                    unsigned int arg3,
                    unsigned int arg4)
{
    int ret = 0;

    // video_printf("syscall_handler_table[syscall_number] = %p\n", syscall_handler_table[syscall_number]);
    if(syscall_number < SYSCALL_MAX && syscall_handler_table[syscall_number] != 0)
    {
        ret = syscall_handler_table[syscall_number](arg0, arg1, arg2, arg3, arg4);
    } else {
        SYSCALL_DEBUG(syscall_number, arg0, arg1, arg2, arg3, arg4);
        video_print_color("!! unhandled syscall !!\n", COLOR(RED, WHITE));
        ret = -1;
    }

    return ret;
}

SYSCALL_HANDLER2(sys_write, char *data, unsigned int size)
{
    unsigned int i;

    cli();
    for(i=0; i<size; i++)
    {
        video_putchar(data[i]);
    }
    sti();

    return (int) size;
}
