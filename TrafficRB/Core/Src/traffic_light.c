#include "traffic_light.h"
#include "board_io.h"
void traffic_show(Phase p){
    static const uint8_t pair[5][2]={{L_GREEN,L_RED},{L_AMBER,L_RED},{L_RED,L_GREEN},{L_RED,L_AMBER},{L_RED,L_RED}};
    if((unsigned)p>4)p=BOTH_RED;
    phase=p;board_lamps(pair[p][0],pair[p][1]);
}
