#ifndef DISPLAY_H
#define DISPLAY_H

// Bibliothek zum Schreiben via I2C auf das Display

#include <unistd.h>//Needed for I2C port
#include <fcntl.h>//Needed for I2C port
#include <sys/ioctl.h>//Needed for I2C port
#include <linux/i2c-dev.h>//Needed for I2C port
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <string>
#include <ctime>

#define WIDTH										128
#define HEIGHT 										64

#define OLED_I2C_ADDRESS 							0x3C
#define OLED_SETSTARTLINE							0x40

/* 
for init sequence look into datasheet last page
the first byte 0x40 is a single command to set start at 0,0
the display has 128 * 64 pixels that store in 1024 bytes, which each pixel = 1 bit
-> need 1025 bytes buffer to show full display
bytes are separated in 128 columns and 8 rows which start from left to right 
1 -> 2 -> ..... 128
129 ->          (128 + 127)
and inside byte: bits are presented from up to down
1
2
3
4
so to access point(0, 0) with the formular below, the + 1 because first byte to send is 0x40
to draw the pixel just write 1 into it, and 0 to undraw(turn off)
*/
#define nth_byte_pixel(x, y)						((x) +(y)/8*128 + 1)
#define nth_bit_pixel(x, y)							((y)%8)

class Display
{
private:
	static unsigned char img[1025];
	const static unsigned char characters[36][5];
	void display_init();
	int pointer;
public:
	Display();

	void writeI2C(unsigned char* data, int bytes, bool should_close = true);

	void draw_pixel(int x, int y, int color = 1);
	void draw_horizontal_line(int x, int y, int length, int color = 1);
	void draw_vertical_line(int x, int y, int length, int color = 1);
	void draw_rect(int x, int y, int width, int height, int color = 1);
	void fill_rect(int x, int y, int width, int height, int color = 1);
	void draw_circle(int x, int y, int radius, int color = 1);
	void fill_circle(int x, int y, int radius, int color = 1);
	int draw_char(int x, int y, char c);
	void draw_text(int x, int y, std::string s);

	void display(bool should_close = true);
	void clear_display();

	void move_pointer(int direction);
};

#endif
