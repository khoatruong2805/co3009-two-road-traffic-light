#include "global.h"
#include "fsm_automatic.h"
#include "traffic_light.h"
#include "software_timer.h"
static void start(Phase p){traffic_show(p);timer_set(100u*((p==R1_GREEN||p==R2_GREEN)?active.green:active.amber));}
void auto_enter(void){start(R1_GREEN);}
void fsm_automatic_run(void){if(mode==AUTOMATIC && timer_expired())start((Phase)((phase+1)%4));}
