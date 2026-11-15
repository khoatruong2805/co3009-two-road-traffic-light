#include "button.h"
#include "board_io.h"
static uint8_t history[4],held[4],pressed,longpress,stable;
void button_run(void){
    uint8_t raw=board_buttons();
    for(unsigned i=0;i<4;i++){uint8_t b=1u<<i;
        history[i]=((history[i]<<1)|((raw>>i)&1u))&7u;
        if(history[i]==7 && !(stable&b)){stable|=b;pressed|=b;held[i]=0;}
        if(!history[i]){stable&=~b;held[i]=0;}
        if((stable&b) && held[i]<80 && ++held[i]==80) longpress|=b;
    }
}
uint8_t button_take(unsigned i){uint8_t b=1u<<i,v=pressed&b;pressed&=~b;return !!v;}
uint8_t button_long(unsigned i){uint8_t b=1u<<i,v=longpress&b;longpress&=~b;return !!v;}
void button_clear(void){pressed=longpress=0;}
