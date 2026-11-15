#include "board_io.h"
#include "board_config.h"
#include "main.h"
#include "traffic_light.h"
extern I2C_HandleTypeDef hi2c1;
void board_init(void){}
uint8_t board_buttons(void){
    uint8_t v=(HAL_GPIO_ReadPin(BTN_1_GPIO_Port,BTN_1_Pin)?1:0)
              |(HAL_GPIO_ReadPin(BTN_2_GPIO_Port,BTN_2_Pin)?2:0)
              |(HAL_GPIO_ReadPin(BTN_3_GPIO_Port,BTN_3_Pin)?4:0)
              |(HAL_GPIO_ReadPin(BTN_4_GPIO_Port,BTN_4_Pin)?8:0);
    return v^BUTTON_ACTIVE_LOW_MASK;
}
void board_lamps(uint8_t a,uint8_t b){
    static const uint8_t codes[2][4]={LIGHT_CODES_R1,LIGHT_CODES_R2};
    uint8_t x=codes[0][a],y=codes[1][b];
    uint32_t mask=TL1_A_Pin|TL1_B_Pin|TL2_A_Pin|TL2_B_Pin;
    uint32_t bits=((x&2)?TL1_A_Pin:0)|((x&1)?TL1_B_Pin:0)
                 |((y&2)?TL2_A_Pin:0)|((y&1)?TL2_B_Pin:0);
    GPIOB->BSRR=((mask&~bits)<<16)|bits; /* both roads change in one write */
}
int bus_write(uint8_t a,const uint8_t *p,unsigned n){return HAL_I2C_Master_Transmit(&hi2c1,a<<1,(uint8_t*)p,n,4)==HAL_OK;}
int bus_read(uint8_t a,uint8_t *p,unsigned n){return HAL_I2C_Master_Receive(&hi2c1,a<<1,p,n,4)==HAL_OK;}
