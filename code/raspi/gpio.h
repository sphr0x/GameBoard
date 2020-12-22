#ifndef GPIO_H
#define GPIO_H

// references:  https://elinux.org/RPi_GPIO_Code_Samples 
//		        https://www.pieter-jan.com/node/15 

#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define BCM2708_PERI_BASE       0xFE000000
#define GPIO_BASE               (BCM2708_PERI_BASE + 0x200000)	// GPIO controller
#define BLOCK_SIZE 		(4*1024)

// IO Acces
struct bcm2835_peripheral
{
    unsigned long addr_p;
    int mem_fd;
    void *map;
    volatile unsigned int *addr;
};


#define INP_GPIO(g)   *(gpio.addr + ((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g)   *(gpio.addr + ((g)/10)) |=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a) *(gpio.addr + (((g)/10))) |= (((a)<=3?(a) + 4:(a)==4?3:2)<<(((g)%10)*3))

#define GPIO_SET  *(gpio.addr + 7)  // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR  *(gpio.addr + 10) // clears bits which are 1 ignores bits which are 0

#define GPIO_READ(g)  *(gpio.addr + 13) &= (1<<(g))

extern struct bcm2835_peripheral gpio;

int map_peripheral(struct bcm2835_peripheral *p);
void unmap_peripheral(struct bm2835_peripheral *p);

void beep(unsigned int note, unsigned int duration);
void *play_music(void *id);

#define UP	27
#define DOWN	5
#define LEFT 	17
#define RIGHT	6
#define ENTER	13

bool button_pressed(int button);
void setup_gpio();

#endif
