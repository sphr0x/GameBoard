#include "display.h"

unsigned char Display::img[1025] = {0};

// pixel map für A-z und 0-9
const unsigned char Display::characters[36][5] = {
	{4, 0x7E, 0x12, 0x12, 0x7e}, 	// A
	{4, 0x7E, 0x4a, 0x4a, 0x34}, 	// B
	{4, 0x7e, 0x42, 0x42, 0x42}, 	// C
	{4, 0x7E, 0x42, 0x42, 0x3C}, 	// D
	{4, 0x7E, 0x52, 0x52, 0x42}, 	// E
	{4, 0x7E, 0x12, 0x12, 0x02}, 	// F
	{4, 0x7E, 0x42, 0x42, 0x72}, 	// G
	{4, 0x7E, 0x10, 0x10, 0x7E}, 	// H
	{4, 0x42, 0x7E, 0x42, 0x00}, 	// I
	{4, 0x62, 0x42, 0x7E, 0x02}, 	// J
	{4, 0x7E, 0x18, 0x24, 0x42}, 	// K
	{4, 0x7E, 0x40, 0x40, 0x40}, 	// L
	{4, 0x7E, 0x04, 0x04, 0x7E}, 	// M
	{4, 0x7E, 0x0C, 0x30, 0x7E}, 	// N
	{4, 0x7E, 0x42, 0x42, 0x7E}, 	// O
	{4, 0x7E, 0x12, 0x12, 0x1E}, 	// P
	{4, 0x7E, 0x42, 0x4A, 0x7E}, 	// Q
	{4, 0x7E, 0x12, 0x32, 0x6E}, 	// R
	{4, 0x4E, 0x4A, 0x4A, 0x72}, 	// S
	{4, 0x02, 0x7E, 0x02, 0x02}, 	// T
	{4, 0x7E, 0x40, 0x40, 0x7E}, 	// U
	{4, 0x3E, 0x60, 0x60, 0x3E}, 	// V
	{4, 0x7E, 0x20, 0x20, 0x7E}, 	// W
	{4, 0x66, 0x3C, 0x3C, 0x66}, 	// X
	{4, 0x1E, 0x10, 0x10, 0x7E}, 	// Y
	{4, 0x62, 0x52, 0x4C, 0x46}, 	// Z

	{4, 0x7E, 0x52, 0x4A, 0x7E}, 	// 0
	{3, 0x44, 0x7E, 0x40}, 		// 1
	{4, 0x64, 0x52, 0x4A, 0x44}, 	// 2
	{3, 0x42, 0x4A, 0x34}, 		// 3
	{4, 0x18, 0x14, 0x7E, 0x10}, 	// 4
	{3, 0x4E, 0x4A, 0x32}, 		// 5
	{4, 0x3C, 0x4A, 0x4A, 0x32}, 	// 6
	{3, 0x6E, 0x1A, 0x06}, 		// 7
	{4, 0x34, 0x4A, 0x4A, 0x34}, 	// 8
	{4, 0x4C, 0x52, 0x56, 0x3C}, 	// 9
};

Display::Display()
{
	display_init();
	clear_display();
	pointer = 12;
}

// Via I2C Schreiben
void Display::writeI2C(unsigned char* data, int bytes, bool should_close){
	int i2cHandle;
	int i2cAddress = OLED_I2C_ADDRESS;
	char deviceName[40];
	sprintf(deviceName, "/dev/i2c-1");
	if ((i2cHandle = open(deviceName, O_RDWR)) < 0){
		printf("error opening I2C\n");
	}

	else{
		if (ioctl(i2cHandle, I2C_SLAVE, i2cAddress) < 0){
			printf("Error at ioctl\n");
		}
		else{
			write(i2cHandle, data, bytes);
		}
	}
	if(should_close)
	{
		close(*deviceName);
	}
}

// Softwarekonfiguration siehe Datenblatt
void Display::display_init()
{
	unsigned char initSequence[26] = { 0x00, 0xAE, 0xA8, 0x3f, 0xD3, 0x00 ,0x40, 0xA1, 0xC8, 0xDA, 0x12, 0x81, 0x7F, 0xA4, 0xA6, 0xD5, 0x80, 0x8D, 0x14, 0xD9, 0x22, 0xD8, 0x30, 0x20, 0x00, 0xAF };
	writeI2C(initSequence, 26);

	unsigned char setFullRange[7] = { 0x00, 0x21, 0x00, 0x7F, 0x22, 0x00, 0x07 };
	writeI2C(setFullRange, 7);
}

// Einen Pixel an x/y Position setzen
void Display::draw_pixel(int x, int y, int color)
{
	if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
	{
		img[nth_byte_pixel(x, y)] |= color << nth_bit_pixel(x, y);
	}
}

// horizontale Linie 
void Display::draw_horizontal_line(int x, int y, int length, int color)
{
	for (int i = 0; i < length; ++i)
	{
		draw_pixel(x + i, y, color);
	}
}
// vertikale Linie 
void Display::draw_vertical_line(int x, int y, int length, int color)
{
	for (int i = 0; i < length; ++i)
	{
		draw_pixel(x, y + i, color);
	}
}
// Rechteck
void Display::draw_rect(int x, int y, int width, int height, int color)
{
	draw_horizontal_line(x, y, width, color);
	draw_horizontal_line(x, y + height, width, color);
	draw_vertical_line(x, y, height, color);
	draw_vertical_line(x + width, y, height, color);
}
// Füllen Rechteck
void Display::fill_rect(int x, int y, int width, int height, int color)
{
	for (int i = 0; i < width; ++i) 
	{
		draw_vertical_line(x + i, y, height, color);
	}
}
// Kreis
void Display::draw_circle(int x, int y, int radius, int color)
{
	int i, x1, y1;
	for (i = 0; i < 360; ++i)
	{
		x1 = x + radius * cos(i * M_PI / 180);
		y1 = y + radius * sin(i * M_PI / 180);
		draw_pixel(x1, y1, color);
	}
}
// Füllen Kreis
void Display::fill_circle(int x, int y, int radius, int color)
{
	// TBD
}
// Buchstabe / Zahl
int Display::draw_char(int x, int y, char c)
{
	unsigned int position_of_byte_to_display = nth_byte_pixel(x, y);
	unsigned int position_of_character_in_characters;

	// Suchen des richtigen chars
	if ('a' <= c && c <= 'z')
	{
		position_of_character_in_characters = c - 'a';
	}

	else if ('A' <= c && c <= 'Z')
	{
		position_of_character_in_characters = c - 'A';
	}

	else if ('0' <= c && c <= '9')
	{
		position_of_character_in_characters = c - '0' + 'z' - 'a' + 1;
	}

	else if(c=='.') 
	{
		img[position_of_byte_to_display] = 0b01000000;
		return 1;
	}

	// Leerzeichen
	else
	{
		return 2;
	}
	// Suchen der Startpositionen des chars zum Schreiben in den Buffer 
	for (int i = 0; i < characters[position_of_character_in_characters][0]; ++i)
	{
		img[position_of_byte_to_display + i] = characters[position_of_character_in_characters][i + 1];
	}

	return characters[position_of_character_in_characters][0];
}
// String
void Display::draw_text(int x, int y, std::string s)
{
	if(sizeof(s) > 1)
	{
		for (auto c : s)
		{
			x += draw_char(x, y, c) + 1;
		}
	}

}
// Schreibt Buffer auf I2C
void Display::display(bool should_close)
{
	img[0] = OLED_SETSTARTLINE;
	writeI2C(img, sizeof(img), should_close);
}
// Bildschirm "löschen"
void Display::clear_display()
{
	img[0] = OLED_SETSTARTLINE;
	for (int i = 1; i < 1025; ++i)
	{
		img[i] = 0x00;
	}
}
