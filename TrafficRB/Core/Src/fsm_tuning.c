#include "global.h"
#include "fsm_tuning.h"
#include "traffic_light.h"
#include "button.h"
void tuning_enter(void){traffic_show(BOTH_RED);draft=active;field=invalid=0;}
uint8_t tuning_apply(void){invalid=(draft.red!=draft.green+draft.amber);if(!invalid) active=draft;return !invalid;}
void fsm_tuning_run(void){
    if(mode!=TUNING)return;
    if(button_take(KEY_NEXT)){field=(field+1)%3;invalid=0;}
    uint8_t *v=field==0?&draft.red:(field==1?&draft.green:&draft.amber);
    if(button_take(KEY_PLUS) && *v<99){++*v;invalid=0;}
    if(button_take(KEY_MINUS) && *v>1){--*v;invalid=0;}
}
