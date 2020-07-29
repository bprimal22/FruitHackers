// Fruit Hackers
// A game by BP Rimal and Kara Olson




// ******* Possible Hardware I/O connections*******************
// Slide pot pin 1 connected to ground
// Slide pot pin 2 connected to PD2/AIN5
// Slide pot pin 3 connected to +3.3V 
// fire button connected to PE0
// special weapon fire button connected to PE1
// 8*R resistor DAC bit 0 on PB0 (least significant bit)
// 4*R resistor DAC bit 1 on PB1
// 2*R resistor DAC bit 2 on PB2
// 1*R resistor DAC bit 3 on PB3 (most significant bit)
// LED on PB4
// LED on PB5

// Backlight (pin 10) connected to +3.3 V
// MISO (pin 9) unconnected
// SCK (pin 8) connected to PA2 (SSI0Clk)
// MOSI (pin 7) connected to PA5 (SSI0Tx)
// TFT_CS (pin 6) connected to PA3 (SSI0Fss)
// CARD_CS (pin 5) unconnected
// Data/Command (pin 4) connected to PA6 (GPIO), high for data, low for command
// RESET (pin 3) connected to PA7 (GPIO)
// VCC (pin 2) connected to +3.3 V
// Gnd (pin 1) connected to ground

#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "ST7735.h"
#include "Print.h"
#include "Random.h"
#include "PLL.h"
#include "ADC.h"
#include "Images.h"
#include "Sound.h"
#include "Timer0.h"
#include "Timer1.h"
#include "SysTick.h"
#include "TExaS.h"
#include "Button.h"

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts

int semaphore;
int game_over;
uint32_t game_points;
extern uint8_t missed_fruit; 
extern uint8_t collide_flag;
extern uint8_t collide_index;



// sprites: Apple Coronavirus	FruitHackers Mango	Orange	Valvano	Watermelon	Yerraballi
//		
const unsigned short *img[] = {Apple, Mango, Orange, Watermelon, Valvano, Coronavirus, Yerraballi};


sprite_t Fruits[7];
sprite_t Ninja;
sprite_t *f_ptr = Fruits;
sprite_t *Nin_ptr = &Ninja;

uint32_t last_ADC;
void Game_Init(void){
	game_over = 0;
	semaphore = 0;
	game_points = 0;
	for(int i =0; i<7; i++){
		Fruits[i].x = 0;
		Fruits[i].y = 0;
		Fruits[i].vx = Fruits[i].vy = 0;
		Fruits[i].image = img[i];
		//black image?
		Fruits[i].life = dead;
		Fruits[i].w = 300;
		Fruits[i].h = 300;
		Fruits[i].needDraw = 0;
	}
	last_ADC = ADC_In();
	Ninja.x = last_ADC;
	Ninja.y = 1500;
	Ninja.vx = Ninja.vy = 0;
	Ninja.image = Ninja1;					
	//black image?
	Ninja.life = alive;
	Ninja.w = Ninja.h = 400;
	Ninja.needDraw = 1;
}
uint8_t English = 0;
uint8_t Dutch = 0;
void choose_language(void){
	IO_Init();
	ST7735_DrawBitmap(0,159, FruitHackers, 128, 160);
	while(1){
	if(((GPIO_PORTF_DATA_R & 0x01) == 0) ||((GPIO_PORTF_DATA_R & 0x10) == 0)){
		break;
	}
}
	ST7735_FillScreen(0x0000); 
	ST7735_OutString("\n Press 1 for English");
	ST7735_OutString("\n \n \n \n Press 2 for Dutch");
	while(1){
		if((GPIO_PORTF_DATA_R & 0x01) == 0){
			
					ST7735_FillScreen(0x0000);
					ST7735_SetCursor(0,0);
					ST7735_OutString("Welkom in"); 
					ST7735_OutString("\nFruitsnijders");
					ST7735_OutString("\nAls je meer dan vier"); 
					ST7735_OutString("\nfruit stukjes mis,"); 
					ST7735_OutString("\njij sterft.");
					ST7735_OutString("\nOok, als je raakt");
					ST7735_OutString("\n\x82\x82n Yerraballi,");
					ST7735_OutString("\n\x82\x82n Valvano, of"); 
					ST7735_OutString("\n\x82\x82n coronavirus");
					ST7735_OutString("\naan, jij sterft.");
					ST7735_OutString("\nElk fruit dat");
					ST7735_OutString("\ngesneden is");
					ST7735_OutString("\nis plus tien punten");
					ST7735_OutString("\nop je score.");
					ST7735_OutString("\nVeel success en"); 
					ST7735_OutString("\nveel plezier!");	
					Dutch = 1;
	for(int j = 0; j < 8000000; j++){}
			return;
	}
		if((GPIO_PORTF_DATA_R & 0x10) == 0){
						
							ST7735_FillScreen(0x0000);
							ST7735_SetCursor(0,0);
							ST7735_OutString("Welcome to");
							ST7735_OutString("\nFruit Hackers");
							ST7735_OutString("\nIf you miss more"); 
							ST7735_OutString("\nthan four pieces");
							ST7735_OutString("\nof fruit, you die.");
							ST7735_OutString("\nAlso, if you touch");
							ST7735_OutString("\nYerraballi, Valvano,");
							ST7735_OutString("\nor a coronavirus,");
							ST7735_OutString("\nyou die.");
							ST7735_OutString("\nEvery fruit that");
							ST7735_OutString("\nis sliced"); 
							ST7735_OutString("\nresults in plus ten"); 
							ST7735_OutString("\nadded to your score.");
							ST7735_OutString("\nGood luck and"); 
							ST7735_OutString("\nhave fun!");
							English = 1;
			for(int j = 0; j < 8000000; j++){}
						return;
			}
	}
}
/*
int realmain(void){
  DisableInterrupts();
  PLL_Init(Bus80MHz);       // Bus clock is 80 MHz 
  Random_Init(1);
	Sound_Init();
	ADC_Init();
  //Output_Init();
  ST7735_FillScreen(0xFFFF);            // set screen to black
  while(1){
  //ST7735_DrawBitmap(Fruits[eApple].x, Fruits[eApple].y, Fruits[eApple].image, Fruits[eApple].w, Fruits[eApple].h); // apple
  ST7735_FillScreen(0x0000);   // set screen to black
  ST7735_SetCursor(1, 1);
  ST7735_OutString("GAME OVER");
  ST7735_SetCursor(1, 2);
  ST7735_OutString("Nice try,");
  ST7735_SetCursor(1, 3);
  ST7735_OutString("Earthling!");
  ST7735_SetCursor(2, 4);
  LCD_OutDec(1234);
}}
*/

/*****************************************************/
// ***************Modular Testing**********************

/*
int sound_main(void){
DisableInterrupts();
PLL_Init(Bus80MHz);       // Bus clock is 80 MHz 
Random_Init(1);
Sound_Init();
//ST7735_FillScreen(0xFFFF);            // set screen to black
EnableInterrupts();
	while(1){
	}
}
*/

/*
int jump_main(void){
  DisableInterrupts();
//  PLL_Init(Bus80MHz);       // Bus clock is 80 MHz 
	TExaS_Init();
	ST7735_InitR(INITR_REDTAB);
	ADC_Init();
//	Random_Init(NVIC_ST_CURRENT_R);
	Sound_Init();
  //Output_Init();
	Game_Init();
	SysTick_Init();
	EnableInterrupts();
	//ST7735_FillScreen(0x0000);            // set screen to black
	ST7735_FillScreen(0x0000); 
	ST7735_DrawBitmap(10, 100, Ninja1, 40, 40);
	ST7735_FillScreen(0x0000); 
  while(1){
		ST7735_DrawBitmap((Ninja.x/10), (Ninja.y/10), Ninja.image, (Ninja.w/10), (Ninja.h/10));
		
	}
}
*/
int main(void){
  DisableInterrupts();
//  PLL_Init(Bus80MHz);       // Bus clock is 80 MHz 
	TExaS_Init();
	ST7735_InitR(INITR_BLACKTAB);
	choose_language();
  Random_Init(1);
	ADC_Init();
	Sound_Init();
  //Output_Init();
	Game_Init();
	SysTick_Init();
	EnableInterrupts();
	//ST7735_FillScreen(0x0000);            // set screen to black

	ST7735_DrawBitmap(10, 100, Ninja1, 40, 40);					
	ST7735_FillScreen(0x0000); 
  while(1){
		//Random_Init(NVIC_ST_CURRENT_R);
		ST7735_DrawBitmap((Ninja.x/10), (Ninja.y/10), Ninja.image, (Ninja.w/10), (Ninja.h/10));
		for(uint8_t i = 0; i<7; i++){
			if(Fruits[i].life == alive){
				ST7735_DrawBitmap((Fruits[i].x/10), (Fruits[i].y/10), Fruits[i].image, (Fruits[i].w/10), (Fruits[i].h/10));
			}

		}
		if(collide_flag == 1){
				ST7735_DrawBitmap((Fruits[collide_index].x/10), (Fruits[collide_index].y/10), FruitSplat, (Fruits[collide_index].w/10), (Fruits[collide_index].h/10));
				for(int i = 0; i<200000; i++);
				ST7735_DrawBitmap((Fruits[collide_index].x/10), (Fruits[collide_index].y/10), Black, (Fruits[collide_index].w/10), (Fruits[collide_index].h/10));
				collide_flag = 0;
				Fruits[collide_index].y = 0;
		}
		if(missed_fruit == 1){
			ST7735_DrawBitmap((Fruits[collide_index].x/10), (Fruits[collide_index].y/10), Black, (Fruits[collide_index].w/10), (Fruits[collide_index].h/10));
			missed_fruit = 0;
			Fruits[collide_index].y = 0;
		}
		if(game_over == 1){
			ST7735_FillScreen(0x0000);
			for(int i = 0; i<40000000; i++){}
			break;
			
		}
	}
			ST7735_SetCursor(0,0);
			if(English == 1){
			ST7735_OutString("\n Game Over Game Over");
			ST7735_OutString("\n You Loser You Loser");
			ST7735_OutString("\n Score: ");
			}
			if(Dutch == 1){	
			ST7735_OutString("\nOepsie Woepsie..."); 
			ST7735_OutString("\nje heb een foutje");
			ST7735_OutString("\ngemaakt");
			ST7735_OutString("\nHet Spel Is Over");
			ST7735_OutString("\nScore: ");
			}
			LCD_OutDec(game_points);
			
	
}


