#include "keypad.h"

void delay_ms(uint32_t ms){
    uint32_t value = *MS_COUNTER_REG_ADDR;
    uint32_t expected_time = value+ms;
    while(value<expected_time){
        value = *MS_COUNTER_REG_ADDR;
    }
    
}

void keypad_init(){
    
}

uint8_t keypad_getkey(){
    uint8_t keys = *BTN_STATE_REG_ADDR;
    uint8_t pos = 0;
    if(keys!=0){
        while(!CHECK_BIT(keys,pos)){
            pos++;
        }
        pos++;
    }
   return pos;
}