// ADC.c
// Runs on LM4F120/TM4C123
// Provide functions that initialize ADC0
// Last Modified: 1/17/2020
// Student names: BP Rimal and Kara Olson
// Last modification date: 4/30/2020

#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"

// ADC initialization function 
// Input: none
// Output: none
// measures from PD2, analog channel 5
void ADC_Init(void){ 
	SYSCTL_RCGCGPIO_R |= 0x08;			//1. Activate clock for Port D
	volatile int adcnop;
	adcnop = 69;												// wait for the clock to stabalize
	adcnop++;
	adcnop++;
	GPIO_PORTD_DIR_R &= (~0x04);		//2. Set direction
	GPIO_PORTD_AFSEL_R |= 0x04;			//3. Activate alternate funciton for PD2
	GPIO_PORTD_DEN_R &= (~0x04);		//4. Disable digital IO interface on PD2
	GPIO_PORTD_AMSEL_R |= 0x04;			//5. Enable analog mode on PD2
	
	SYSCTL_RCGCADC_R |= 0x01;				//6. Activate clock for ADC0
	volatile int adcnoop;
	adcnoop = 69;											// wait for the clock to stabalize
	adcnoop++;
	adcnoop++;
	ADC0_PC_R = 0x01;								//7. speed for ADC0; 1=125kHz
	ADC0_SSPRI_R = 0x0123;					//8. give sequencer 3 the most priority
	ADC0_ACTSS_R &= (~0x08);				//9. disable SQ3
	ADC0_EMUX_R &= (~0xF000);				//10. S/W trigger for SQ3
	ADC0_SSMUX3_R = 5;							//11. slave select Ain5 (channel 5) (PD2)
	ADC0_SSCTL3_R = 0x06;						//12. no TS0, D0, yes IE0, END0
	ADC0_IM_R &= (~0x08);						//13. disable SQ3 interrupt
	ADC0_SAC_R = 4;   							// 16-point averaging, move this line into your ADC_Init()
	ADC0_ACTSS_R |= 0x08;						//14. Enable SQ3
	
}

//------------ADC_In------------
// Busy-wait Analog to digital conversion
// Input: none
// 				gets 12-bit result of ADC conversion
// Output: 0-1280 FP x position
// measures from PD2, analog channel 5
uint32_t ADC_In(void){  
  uint32_t result;
	ADC0_PSSI_R = 0x08;													// 1. initiate SQ3
	while((ADC0_RIS_R & 0x08) == 0);					// 2. wait for conversion to be done
	result = ADC0_SSFIFO3_R & 0x0FFF;						// 3. read 12 bit result
	ADC0_ISC_R = 0x08;													// 4. Acknowledge completion; 0x08 beccause SQ3
	return ((result*940)/4096);
}

