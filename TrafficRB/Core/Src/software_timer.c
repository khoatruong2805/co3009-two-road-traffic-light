#include "software_timer.h"
#include "scheduler.h"
static uint32_t deadline;
static uint8_t armed,done;
void timer_set(uint32_t ticks){deadline=SCH_Now()+ticks;armed=1;done=0;}
void timer_stop(void){armed=done=0;}
void timer_run(void){if(armed && (int32_t)(SCH_Now()-deadline)>=0){armed=0;done=1;}}
uint8_t timer_expired(void){uint8_t d=done;done=0;return d;}
