#ifndef SYSCALL_H
#define SYSCALL_H

unsigned int syscall_handle(unsigned int syscall_number,
                    unsigned int arg0,
                    unsigned int arg1,
                    unsigned int arg2,
                    unsigned int arg3,
                    unsigned int arg4);

unsigned int sys_write(char *data, unsigned int size);

#define SYS_WRITE 0x01

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