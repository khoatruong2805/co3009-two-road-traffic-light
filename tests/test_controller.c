#include "scheduler.h"
#include "global.h"
#include "button.h"
#include "tasks.h"
#include "dht20.h"
#include "lcd.h"
#include "traffic_light.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
static void assertion_failed(int signal_number) { (void)signal_number; _Exit(1); }
static uint8_t keys,a,b,frame[7],bus_ok=1,lcd_seen,start_seen;
static unsigned phase_writes;
static char lcd_cells[32];
static uint8_t lcd_address, lcd_init_count;
static void observe_lcd(const uint8_t *bytes, unsigned length)
{
    for (unsigned i = 0; i < length; i += 3) {
        assert((bytes[i] & 0x0e) == 8);
        assert(bytes[i + 1] == (bytes[i] | 4));
        assert(bytes[i + 2] == bytes[i]);
    }
    if (length == 3) {
        static const uint8_t expected[] = {0x30, 0x30, 0x30, 0x20};
        if (lcd_init_count == 4) lcd_init_count = 0;
        assert((bytes[0] & 0xf1) == expected[lcd_init_count++]);
        return;
    }
    assert(length == 6 && lcd_init_count == 4);
    uint8_t value = (bytes[0] & 0xf0) | (bytes[3] >> 4);
    assert((bytes[0] & 1) == (bytes[3] & 1));
    if (bytes[0] & 1) {
        unsigned position = lcd_address >= 0x40 ? lcd_address - 0x40 + 16 : lcd_address;
        assert(position < 32);
        lcd_cells[position] = value;
        lcd_address++;
    } else if (value & 0x80) {
        lcd_address = value & 0x7f;
    } else if (value == 1) {
        memset(lcd_cells, ' ', sizeof(lcd_cells));
        lcd_address = 0;
    }
}
uint8_t board_buttons(void){return keys;}
void board_init(void){}
void board_lamps(uint8_t x,uint8_t y){assert(x==L_RED||y==L_RED);a=x;b=y;phase_writes++;}
static unsigned restored;static const uint8_t regs[]={0x1b,0x1c,0x1e};
int bus_read(uint8_t addr,uint8_t *p,unsigned n){
    assert(addr==0x38);
    if(n==1)p[0]=restored==3?0x18:0;
    else if(n==3){p[0]=0;p[1]=0x51;p[2]=0x37;}
    else{assert(n==7);memcpy(p,frame,7);}
    return bus_ok;
}
int bus_write(uint8_t addr,const uint8_t *p,unsigned n){
    if(addr==0x38){
        if(n==1)assert(p[0]==0x71);
        else if(p[0]==0xac){assert(n==3&&p[1]==0x33&&p[2]==0);start_seen++;}
        else{assert(n==3&&restored<3);if(p[0]==regs[restored])assert(!p[1]&&!p[2]);
             else{assert(p[0]==(0xb0|regs[restored])&&p[1]==0x51&&p[2]==0x37);restored++;}}
    }
    else{assert(addr==0x27&&(n==3||n==6));lcd_seen=1;if(bus_ok)observe_lcd(p,n);}
    return bus_ok;
}
static void tick(unsigned n){while(n--){SCH_Update();SCH_Dispatch_Tasks();}}
static unsigned logv[64],used;
static void f1(void){logv[used++]=1;}
static void f2(void){logv[used++]=2;}
static void f3(void){logv[used++]=3;}
static void key(int i){keys=1<<i;tick(4);keys=0;tick(4);}
static void test_scheduler(void){
    SCH_Init();assert(SCH_Add_Task(f3,9,0));
    uint32_t cancel=SCH_Add_Task(f2,5,0);assert(SCH_Add_Task(f1,2,3));assert(SCH_Add_Task(f2,2,0));
    assert(SCH_Delete_Task(cancel));tick(1);assert(!used);tick(1);assert(used==2&&logv[0]==1&&logv[1]==2);
    tick(7);assert(used==5&&logv[2]==1&&logv[3]==1&&logv[4]==3);
    SCH_Init();used=0;uint32_t id=SCH_Add_Task(f1,1,2);for(unsigned i=0;i<10;i++)SCH_Update();
    SCH_Dispatch_Tasks();assert(used==1);tick(1);assert(used==2);assert(SCH_Delete_Task(id));tick(8);assert(used==2);
    SCH_Init();for(int i=0;i<16;i++)assert(SCH_Add_Task(f1,1,0));assert(!SCH_Add_Task(f1,1,0));tick(5);assert(used==18);
    puts("PASS scheduler: delta queue, equal deadlines, cancellation, full queue, one-shot, delayed dispatch");
}
int main(void){
    signal(SIGABRT, assertion_failed);
    test_scheduler();tasks_start();SCH_Dispatch_Tasks();
    assert(phase==R1_GREEN&&a==L_GREEN&&b==L_RED);
    /* 50% RH and 25.0 C, then valid CRC computed independently for fixed bytes. */
    const uint8_t fixed[]={0x18,0x80,0,0x06,0,0,0};memcpy(frame,fixed,7);
    unsigned c=255;for(int i=0;i<6;i++){c^=frame[i];for(int j=0;j<8;j++)c=(c&128)?(c*2^49)&255:(c*2)&255;}frame[6]=c;
    tick(95);assert(restored==3&&start_seen&&climate_valid&&rh10==500&&temp10==250&&lcd_seen);
    assert(memcmp(lcd_cells, "AUTO G/R", 8) == 0);
    assert(memcmp(lcd_cells + 16, "25.0C 50.0%", 11) == 0);
    frame[6]^=1;tick(220);assert(!climate_valid);
    tick(184);assert(phase==R1_GREEN);tick(1);assert(phase==R1_AMBER);tick(200);assert(phase==R2_GREEN);
    tick(500);assert(phase==R2_AMBER);tick(200);assert(phase==R1_GREEN);
    tick(500);keys=1;tick(1);keys=0;tick(2);assert(mode==AUTOMATIC);key(KEY_MODE);
    assert(mode==MANUAL&&phase==BOTH_RED);key(KEY_NEXT);assert(phase==R1_GREEN);
    keys=1<<KEY_NEXT;tick(82);assert(phase==R1_AMBER&&button_long(KEY_NEXT)&&!button_long(KEY_NEXT));
    keys=0;tick(4);key(KEY_MODE);assert(mode==TUNING&&phase==BOTH_RED);
    key(KEY_PLUS);key(KEY_MODE);assert(mode==TUNING&&invalid&&active.red==7);
    key(KEY_NEXT);key(KEY_PLUS);key(KEY_MODE);assert(mode==AUTOMATIC&&active.red==8&&active.green==6);
    assert(phase==R1_GREEN);bus_ok=0;tick(610);assert(phase==R1_AMBER&&!climate_valid);
    puts("PASS FSM: four phases, amber exit, bounce, long press, invalid/valid tuning; DHT frame/CRC and missing I2C");

    /* Both keys are stable in the same sample. MODE must clear NEXT. */
    keys = (1 << KEY_MODE) | (1 << KEY_NEXT);
    tick(4); keys = 0; tick(4);
    assert(mode == MANUAL && phase == BOTH_RED);
    tick(250);
    assert(phase == BOTH_RED);
    key(KEY_MODE);
    assert(mode == TUNING && phase == BOTH_RED);
    bus_ok = 1;
    tick(220);
    assert(memcmp(lcd_cells, "SET RED 8s", 10) == 0);
    key(KEY_PLUS);
    key(KEY_MODE);
    tick(50);
    assert(invalid && mode == TUNING);
    assert(memcmp(lcd_cells + 16, "Need R = G + A", 14) == 0);
    key(KEY_MINUS);
    key(KEY_MODE);
    assert(mode == AUTOMATIC && phase == R1_GREEN);

    /* Every tuning field clamps at both limits without uint8_t wraparound. */
    key(KEY_MODE); key(KEY_MODE);
    for (unsigned selected = 0; selected < 3; ++selected) {
        uint8_t *value = selected == 0 ? &draft.red :
                         selected == 1 ? &draft.green : &draft.amber;
        assert(field == selected);
        for (unsigned i = 0; i < 110; ++i) key(KEY_MINUS);
        assert(*value == 1 && phase == BOTH_RED);
        for (unsigned i = 0; i < 110; ++i) key(KEY_PLUS);
        assert(*value == 99 && phase == BOTH_RED);
        key(KEY_NEXT);
    }
    key(KEY_MODE);
    assert(mode == TUNING && invalid && active.red == 8);
    puts("PASS integration: PCF8574 enable pulses and LCD text, MODE+NEXT,"
         " stale timers, display recovery, tuning limits for all three colors");

    /* Restore a valid set using the public button path, then exercise a busy sensor. */
    const uint8_t targets[] = {7, 5, 2};
    for (unsigned selected = 0; selected < 3; ++selected) {
        for (unsigned value = 99; value > targets[selected]; --value) key(KEY_MINUS);
        key(KEY_NEXT);
    }
    key(KEY_MODE);
    assert(mode == AUTOMATIC && active.red == 7 && active.green == 5 && active.amber == 2);
    frame[0] |= 0x80;
    tick(400);
    assert(!climate_valid && mode == AUTOMATIC);
    frame[0] &= ~0x80;
    frame[6] ^= 1; /* Restore the original valid CRC. */
    tick(250);
    assert(climate_valid && temp10 == 250 && rh10 == 500);
    assert(memcmp(lcd_cells + 16, "25.0C 50.0%", 11) == 0);

    unsigned transitions_before = phase_writes;
    tick(360000); /* One hour of virtual scheduler time. */
    assert(mode == AUTOMATIC && climate_valid && phase_writes - transitions_before >= 1000);
    puts("PASS stability: busy-sensor timeout/recovery and 360000 ticks"
         " (one virtual hour), with a red opposing road on every phase write");
}
