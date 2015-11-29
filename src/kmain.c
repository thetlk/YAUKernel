#include <driver/video.h>

void kmain()
{
	video_screen_clean();
	video_print_color("coucou eliott", COLOR(PINK, WHITE));
	while(1);
}
