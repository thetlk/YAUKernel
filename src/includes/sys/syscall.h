#ifndef SYSCALL_H
#define SYSCALL_H

typedef int (*syscall_handler_func)(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);

void syscall_init(void);
int syscall_set_handler(unsigned int number, syscall_handler_func func);
int syscall_handle(unsigned int syscall_number,
                    unsigned int arg0,
                    unsigned int arg1,
                    unsigned int arg2,
                    unsigned int arg3,
                    unsigned int arg4);

#define SYSCALL_MAX 256
#define SYS_WRITE 0x01

#define UNUSED(X) unsigned int unused##X __attribute__ ((unused))
#define SYSCALL_HANDLER0(fname) int fname(UNUSED(0), UNUSED(1), UNUSED(2), UNUSED(3), UNUSED(4))
#define SYSCALL_HANDLER1(fname, param0) int fname(param0, UNUSED(1), UNUSED(2), UNUSED(3), UNUSED(4))
#define SYSCALL_HANDLER2(fname, param0, param1) int fname(param0, param1, UNUSED(2), UNUSED(3), UNUSED(4))
#define SYSCALL_HANDLER3(fname, param0, param1, param2) int fname(param0, param1, param2, UNUSED(3), UNUSED(4))
#define SYSCALL_HANDLER4(fname, param0, param1, param2, param3) int fname(param0, param1, param2, param3, UNUSED(4))
#define SYSCALL_HANDLER5(fname, param0, param1, param2, param3, param4) int fname(param0, param1, param2, param3, param4)

SYSCALL_HANDLER2(sys_write, char *data, unsigned int size);

#define SYSCALL_DEBUG(n, arg0, arg1, arg2, arg3, arg4) \
    video_print("syscall_number = 0x");                \
    video_print_number(n, 16);                         \
    video_print("\n\tebx = 0x");                       \
    video_print_number(arg0, 16);                      \
    video_print("\n\tecx = 0x");                       \
    video_print_number(arg1, 16);                      \
    video_print("\n\tedx = 0x");                       \
    video_print_number(arg2, 16);                      \
    video_print("\n\tedi = 0x");                       \
    video_print_number(arg3, 16);                      \
    video_print("\n\tesi = 0x");                       \
    video_print_number(arg4, 16);                      \
    video_print("\n");

#endif