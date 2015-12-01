#ifndef DRIVER_VIDEO_H
#define DRIVER_VIDEO_H

#define SCREEN_ADDR 0xB8000
#define LINE_SIZE 	80
#define LINE_COUNT  25

#define SCREEN(l, c) ((char*) SCREEN_ADDR) + LINE_SIZE*2*l + c*2
#define CURRENT_SCREEN() SCREEN(line, column)

#define BLACK 	0
#define BLUE 	1
#define GREEN 	2
#define CYAN 	3
#define RED 	4
#define PINK 	5
#define ORANGE 	6
#define WHITE 	7

#define COLOR(fg, bg) bg << 4 | fg

/*
#define VIDEO_DISPLAY_OK() print("["); 						 \
				   print_color("OK", COLOR(GREEN, BLACK)); \
				   print("]\n"); */

void video_screen_clean();
void video_print(char *s);
void video_print_color(char *s, char color);
void video_putchar(char c);
void video_putchar_color(char c, char color);

#endif