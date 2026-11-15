#include "dht20.h"
#include "board_io.h"
#include "scheduler.h"
int16_t temp10;uint16_t rh10;uint8_t climate_valid;
static uint32_t next=20;static uint8_t reading,polls;
enum { CHECK, RESTORE_SELECT, RESTORE_READ, READY };
static uint8_t init_state, reg_index;
static const uint8_t calibration_regs[]={0x1b,0x1c,0x1e};
static uint8_t crc(const uint8_t *p){uint8_t c=0xff;for(unsigned i=0;i<6;i++){
    c^=p[i];for(int j=0;j<8;j++)c=(c&128)?(uint8_t)((c<<1)^0x31):(uint8_t)(c<<1);
}return c;}
void dht20_run(void){
    const uint8_t measure[]={0xac,0x33,0};uint8_t data[7];
    uint32_t t=SCH_Now();if((int32_t)(t-next)<0)return;
    /* Recheck after a disconnect. Restore factory bytes, never invent calibration data. */
    if(init_state!=READY){
        uint8_t cmd[3]={0x71,0,0};next=t+1;
        if(init_state==CHECK){
            if(!bus_write(0x38,cmd,1)||!bus_read(0x38,data,1))goto failed;
            if((data[0]&0x18)==0x18)init_state=READY;
            else {reg_index=0;init_state=RESTORE_SELECT;}
        }else if(init_state==RESTORE_SELECT){
            cmd[0]=calibration_regs[reg_index];
            if(!bus_write(0x38,cmd,3))goto failed;
            init_state=RESTORE_READ;
        }else{
            if(!bus_read(0x38,data,3))goto failed;
            cmd[0]=0xb0|calibration_regs[reg_index];cmd[1]=data[1];cmd[2]=data[2];
            if(!bus_write(0x38,cmd,3))goto failed;
            if(++reg_index==3){init_state=CHECK;next=t+20;}else init_state=RESTORE_SELECT;
        }
        return;
    }
    if(!reading && bus_write(0x38,measure,3)){reading=1;polls=0;next=t+9;return;}
    if(reading && bus_read(0x38,data,7)){
        if((data[0]&0x80) && polls++<4){next=t+2;return;}
        if(!(data[0]&0x80) && (data[0]&0x18)==0x18 && crc(data)==data[6]){
            uint32_t h=((uint32_t)data[1]<<12)|(data[2]<<4)|(data[3]>>4);
            uint32_t c=((uint32_t)(data[3]&15)<<16)|(data[4]<<8)|data[5];
            rh10=(h*1000u+524288u)>>20;temp10=(int)((c*2000u+524288u)>>20)-500;
            climate_valid=1;reading=0;next=t+200;return;
        }
    }
failed:
    climate_valid=0;reading=0;init_state=CHECK;next=t+200;
}
