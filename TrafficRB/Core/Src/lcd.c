#include "lcd.h"
#include "board_io.h"
#include "board_config.h"
#include "scheduler.h"
#include "dht20.h"
#include "global.h"
#include <string.h>
#include <stdio.h>
static char wanted[32],shown[32];
static uint8_t stage,scan;
static uint32_t next=10;
static void nib(uint8_t n,uint8_t rs,uint8_t *out){uint8_t v=(n<<4)|8|rs;out[0]=v;out[1]=v|4;out[2]=v;}
static int send(uint8_t byte,uint8_t rs,int half){uint8_t out[6];nib(byte>>4,rs,out);nib(byte&15,rs,out+3);return bus_write(LCD_ADDR7,out,half?3:6);}
void lcd_run(void){
    static const uint8_t init[]={0x30,0x30,0x30,0x20,0x28,0x08,0x01,0x06,0x0c};
    uint32_t t=SCH_Now();if((int32_t)(t-next)<0)return;
    if(stage<sizeof(init)){
        if(send(init[stage],0,stage<4)){stage++;next=t+1;if(stage==sizeof(init))memset(shown,0,sizeof(shown));return;}
    }else{
        for(unsigned i=0;i<32;i++){uint8_t pos=scan;scan=(scan+1)%32;
            if(shown[pos]==wanted[pos])continue;
            if(!send(0x80+(pos<16?pos:0x40+pos-16),0,0)||!send(wanted[pos],1,0))break;
            shown[pos]=wanted[pos];return;
        }
        if(!memcmp(shown,wanted,32))return;
    }
    stage=0;next=t+100;
}
static void row(int i,const char *s){for(int j=0;j<16;j++)wanted[16*i+j]=*s?*s++:' ';}
void display_run(void){
    static const char *const phases[]={"G/R","A/R","R/G","R/A","R/R"};
    char s[32];if(mode==TUNING){
        unsigned v=field==0?draft.red:field==1?draft.green:draft.amber;
        snprintf(s,sizeof(s),"SET %s %us",field==0?"RED":field==1?"GREEN":"AMBER",v);row(0,s);
        if(invalid){row(1,"Need R = G + A");return;}
    }else{snprintf(s,sizeof(s),"%s %s",mode==AUTOMATIC?"AUTO":"MAN",phases[phase]);row(0,s);}
    if(climate_valid){int t=temp10<0?-temp10:temp10;
        snprintf(s,sizeof(s),"%s%d.%dC %u.%u%%",temp10<0?"-":"",t/10,t%10,rh10/10,rh10%10);
        row(1,s);
    }else row(1,"T: --   RH: --");
}
