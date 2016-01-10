#include <sys/syscall.h>
#include <driver/video.h>

void syscall_handle(int n)
{
	video_print("syscall number = 0x");
	video_print_number(n, 16);
	video_print(" ---\n");
}
