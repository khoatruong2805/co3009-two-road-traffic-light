#include "tasks.h"
#include "scheduler.h"
#include "button.h"
#include "global.h"
#include "software_timer.h"
#include "fsm_automatic.h"
#include "fsm_manual.h"
#include "fsm_tuning.h"
#include "board_io.h"
#ifndef TRAFFIC_SIM
#include "dht20.h"
#include "lcd.h"
#endif
void tasks_start(void){
    SCH_Init();board_init();app_init();
    /* Stable order for equal deadlines: sample, route mode, timer, active FSM. */
    SCH_Add_Task(button_run,1,1); SCH_Add_Task(mode_run,1,1);
    SCH_Add_Task(timer_run,1,1); SCH_Add_Task(fsm_automatic_run,1,1);
    SCH_Add_Task(fsm_manual_run,1,1); SCH_Add_Task(fsm_tuning_run,1,1);
#ifndef TRAFFIC_SIM
    SCH_Add_Task(dht20_run,1,1); SCH_Add_Task(display_run,0,10); SCH_Add_Task(lcd_run,1,1);
#endif
}
