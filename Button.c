//Button.c
#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "Sound.h"


volatile uint32_t FallingEdges = 0;
uint8_t Jump_flag;
uint8_t Jump_up;
uint8_t slice_pressed;
void Button_Init(void){volatile long delay;                            
  SYSCTL_RCGCGPIO_R |= 0x20;           // activate port F
  while((SYSCTL_PRGPIO_R&0x20)==0){}; // allow time for clock to start
  delay = 100;                  //    allow time to finish activating
	slice_pressed = 0;
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock GPIO Port F
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0                              // 2) GPIO Port F needs to be unlocked
  GPIO_PORTF_AMSEL_R &= ~0x11;  // 3) disable analog on PF4,0
                                // 4) configure PF4,0 as GPIO
  GPIO_PORTF_PCTL_R &= ~0x000F000F;
  GPIO_PORTF_DIR_R &= ~0x11;    // 5) make PF4,0 in
  GPIO_PORTF_AFSEL_R &= ~0x11;  // 6) disable alt funct on PF4,0
	GPIO_PORTF_PUR_R |= 0x11;     
  GPIO_PORTF_DEN_R |= 0x11;     // 7) enable digital I/O on PF4,0
  GPIO_PORTF_IS_R &= ~0x11;     //    PF4,0 is edge-sensitive
  GPIO_PORTF_IBE_R &= ~0x11;    //    PF4,0 is not both edges
  GPIO_PORTF_IEV_R &= ~0x11;     //    PF4,0 falling edge event (Neg logic)
  GPIO_PORTF_ICR_R = 0x11;      //    clear flag1-0
  GPIO_PORTF_IM_R |= 0x11;      // 8) arm interrupt on PF4,0
                                // 9) GPIOF priority 2
  NVIC_PRI7_R = (NVIC_PRI7_R&0xFF0FFFFF)|0x00200000;
  NVIC_EN0_R = 1<<30;   // 10)enable interrupt 30 in NVIC
}

void GPIOPortF_Handler(void){
	
	if ((GPIO_PORTF_RIS_R & 0x01)==0x01){ //PF1
		 GPIO_PORTF_ICR_R = 0x01;		// ACKNOWLEDGE FLAG 0
			Sound_Play(Slice);
			slice_pressed =1;
	}
	if ((GPIO_PORTF_RIS_R & 0x10)==0x10){ //PF4
		GPIO_PORTF_ICR_R = 0x10;      // acknowledge flag4
		Sound_Play(Jump);
		Jump_flag = 1;
		Jump_up =1;
	}
}

void IO_Init(void) {
 // --UUU-- Code to initialize PF4 and PF0
	SYSCTL_RCGCGPIO_R |= 0x20;
	volatile uint16_t noop;
	noop = 69;
	GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;
	GPIO_PORTF_CR_R |= 0xFF;
	GPIO_PORTF_DIR_R &= ~0x11;    //  make PF4,0 in
	GPIO_PORTF_PUR_R |= 0x11;
	GPIO_PORTF_DEN_R |= 0x11;
}

