#include "global.h"
#include "button.h"
#include "traffic_light.h"
#include "software_timer.h"
#include "fsm_automatic.h"
#include "fsm_manual.h"
#include "fsm_tuning.h"
void app_init(void){active=(Timing){7,5,2};mode=AUTOMATIC;auto_enter();}
void mode_run(void){
    if(!button_take(KEY_MODE))return;
    button_clear();timer_stop();
    if(mode==AUTOMATIC){mode=MANUAL;manual_enter();}
    else if(mode==MANUAL){mode=TUNING;tuning_enter();}
    else if(tuning_apply()){mode=AUTOMATIC;auto_enter();}
}
