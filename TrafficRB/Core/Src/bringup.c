#include "bringup.h"
#include "main.h"
#include "scheduler.h"
extern I2C_HandleTypeDef hi2c1;
volatile uint8_t diag_ab,diag_buttons_raw,diag_user_raw,diag_i2c_count,diag_i2c_addr[8];
static void blink_a(void){HAL_GPIO_TogglePin(LED_A_GPIO_Port,LED_A_Pin);}
static void blink_b(void){HAL_GPIO_TogglePin(LED_B_GPIO_Port,LED_B_Pin);}
static void lights(void){
    static unsigned seq;
    diag_ab=seq++&3;
    unsigned pins=TL1_A_Pin|TL2_A_Pin|TL1_B_Pin|TL2_B_Pin;
    unsigned value=((diag_ab&2)?(TL1_A_Pin|TL2_A_Pin):0)|((diag_ab&1)?(TL1_B_Pin|TL2_B_Pin):0);
    GPIOB->BSRR=((pins&~value)<<16)|value;
}
static void buttons(void){
    diag_buttons_raw=HAL_GPIO_ReadPin(BTN_1_GPIO_Port,BTN_1_Pin)
        |(HAL_GPIO_ReadPin(BTN_2_GPIO_Port,BTN_2_Pin)<<1)
        |(HAL_GPIO_ReadPin(BTN_3_GPIO_Port,BTN_3_Pin)<<2)
        |(HAL_GPIO_ReadPin(BTN_4_GPIO_Port,BTN_4_Pin)<<3);
    diag_user_raw=HAL_GPIO_ReadPin(USER_BTN_GPIO_Port,USER_BTN_Pin);
}
static void scan(void){
    static uint8_t addr=8; if(addr>0x77)return;
    if(HAL_I2C_IsDeviceReady(&hi2c1,addr<<1,1,3)==HAL_OK && diag_i2c_count<8)diag_i2c_addr[diag_i2c_count++]=addr;
    ++addr;
}
void bringup_start(void){SCH_Init();SCH_Add_Task(blink_a,50,50);SCH_Add_Task(blink_b,100,100);
    SCH_Add_Task(lights,0,200);SCH_Add_Task(buttons,1,1);SCH_Add_Task(scan,20,1);}
