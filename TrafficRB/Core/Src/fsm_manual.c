#include "global.h"
#include "fsm_manual.h"
#include "traffic_light.h"
#include "button.h"
void manual_enter(void){traffic_show(BOTH_RED);}
void fsm_manual_run(void){if(mode==MANUAL && button_take(KEY_NEXT)) traffic_show(phase==BOTH_RED?R1_GREEN:(Phase)((phase+1)%4));}
