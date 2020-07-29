// Sound.h
// Runs on TM4C123 or LM4F120
// Prototypes for basic functions to play sounds from the
// original Space Invaders.
// Jonathan Valvano
// 1/17/2020

#include <stdint.h>
void Sound_Init(void);
typedef enum{Slice, Jump, Splash, Ouch, Gameover, Cough} soundeffect;
void Sound_Play(soundeffect sound);
