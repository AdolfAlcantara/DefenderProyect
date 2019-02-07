#include "system.h"
#define TIMER 0xffff0008
#define KEYPAD 0xffff0004
#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

uint32_t volatile * const timer = (uint32_t  *)TIMER;
uint32_t volatile * const keypad = (uint32_t  *)KEYPAD;

void delay_ms(uint32_t ms);
void keypad_init();
uint8_t keypad_getkey();