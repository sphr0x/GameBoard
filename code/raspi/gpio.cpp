//
//  How to access GPIO registers from C-code on the Raspberry-Pi
//  Example program
//  15-January-2012
//  Dom and Gert
//  Revised: 15-Feb-2013


// Access from ARM Running Linux

#include "gpio.h"
#include "piezo.h"

struct bcm2835_peripheral gpio = {GPIO_BASE};

// Exposes the physical address defined in the passed structure using mmap on /dev/mem
// Mappen der GPIO-Adressen
int map_peripheral(struct bcm2835_peripheral *p)
{
	// Open /dev/mem
	if ((p->mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0)
	{
    	printf("Failed to open /dev/mem, try checking permissions.\n");
    	return -1;
	}

	p->map = mmap(
    	NULL,
    	BLOCK_SIZE,
    	PROT_READ|PROT_WRITE,
    	MAP_SHARED,
    	p->mem_fd,      // File descriptor to physical memory virtual file '/dev/mem'
    	p->addr_p       // Address in physical map that we want this memory block to expose
	);

	if (p->map == MAP_FAILED)
	{
    	perror("mmap");
    	return -1;
	}

	p->addr = (volatile unsigned int *)p->map;

	close(p->mem_fd);
	return 0;
}

// Freigeben von shared memory
void unmap_peripheral(struct bcm2835_peripheral *p)
{
    munmap(p->map, BLOCK_SIZE);
    close(p->mem_fd);
}

// Test: Knopfdruck
bool button_pressed(int button)
{
	if(GPIO_READ(button))return false;
	return true;
}

// Setzten der Pins
// Piezo -> output
// Tasten -> input
void setup_gpio()
{
	INP_GPIO(BeepPin);
	OUT_GPIO(BeepPin);
	INP_GPIO(UP);
	INP_GPIO(DOWN);
	INP_GPIO(LEFT);
	INP_GPIO(RIGHT);
	INP_GPIO(ENTER);
}

// Steuern des Tons und dessen Dauer 
void beep(unsigned int note, unsigned int duration)
{
	//This is the semiperiod of each note.
	long beepDelay = (long)(1000000/note);
	//This is how much time we need to spend on the note.
	long time = (long)((duration*1000)/(beepDelay*2));
	for (int i=0;i<time;i++)
	{
		//1st semiperiod
		GPIO_SET |= 1<<BeepPin;
		usleep(beepDelay);
		//2nd semiperiod
		GPIO_CLR |= 1<<BeepPin;
		usleep(beepDelay);
	}

	//Add a little delay to separate the single notes
	GPIO_CLR |= 1<<BeepPin;
	usleep(20);
}

//The source code of the Imperial March from Star Wars
void *play_music(void *id)
{
	beep( a_node, 500);
	beep( a_node, 500);
	beep( f_node, 350);
	beep( cH_node, 150);

	beep( a_node, 500);
	beep( f_node, 350);
	beep( cH_node, 150);
	beep( a_node, 1000);
	beep( eH_node, 500);

	beep( eH_node, 500);
	beep( eH_node, 500);
	beep( fH_node, 350);
	beep( cH_node, 150);
	beep( gS_node, 500);

	beep( f_node, 350);
	beep( cH_node, 150);
	beep( a_node, 1000);
	beep( aH_node, 500);
	beep( a_node, 350);

	beep( a_node, 150);
	beep( aH_node, 500);
	beep( gHS_node, 250);
	beep( gH_node, 250);
	beep( fHS_node, 125);

	beep( fH_node, 125);
	beep( fHS_node, 250);

	usleep(250);

	beep( aS_node, 250);
	beep( dHS_node, 500);
	beep( dH_node, 250);
	beep( cHS_node, 250);
	beep( cH_node, 125);

	beep( b_node, 125);
	beep( cH_node, 250);

	usleep(250);

	beep( f_node, 125);
	beep( gS_node, 500);
	beep( f_node, 375);
	beep( a_node, 125);
	beep( cH_node, 500);

	beep( a_node, 375);
	beep( cH_node, 125);
	beep( eH_node, 1000);
	beep( aH_node, 500);
	beep( a_node, 350);

	beep( a_node, 150);
	beep( aH_node, 500);
	beep( gHS_node, 250);
	beep( gH_node, 250);
	beep( fHS_node, 125);

	beep( fH_node, 125);
	beep( fHS_node, 250);

	usleep(250);

	beep( aS_node, 250);
	beep( dHS_node, 500);
	beep( dH_node, 250);
	beep( cHS_node, 250);
	beep( cH_node, 125);

	beep( b_node, 125);
	beep( cH_node, 250);

	sleep(250);

	beep( f_node, 250);
	beep( gS_node, 500);
	beep( f_node, 375);
	beep( cH_node, 125);
	beep( a_node, 500);

	beep( f_node, 375);
	beep( c_node, 125);
	beep( a_node, 1000);
	
	return NULL;
}
