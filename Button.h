//Buttons.h
//Contains edge-triggered interrupt for buttons
//Kara Olson and BP Rimal

#ifndef __buttons_h
#define __buttons_h
#include <stdint.h>

//------------EdgeCounter_Init------------
// Initialize Edge Triggered Interrupts for Port F switches
// Input: none
// Output: none
void Button_Init(void);
void IO_Init(void);
	
//DOES EDGECOUNTER HANDLER GO IN HEADER TOO??


#endif

