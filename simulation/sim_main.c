/* C6 adapter for the lab schematic: common application code is compiled unchanged. */
#include "stm32f1xx.h"
#include "board_io.h"
#include "tasks.h"
#include "scheduler.h"
#include "global.h"
#include "traffic_light.h"
static void pin(GPIO_TypeDef *p,unsigned n,unsigned cfg){
    volatile uint32_t *cr=(n<8)?&p->CRL:&p->CRH;unsigned s=4*(n%8);
    *cr=(*cr&~(15u<<s))|(cfg<<s);
}
void board_init(void){
    RCC->APB2ENR|=RCC_APB2ENR_IOPAEN|RCC_APB2ENR_IOPBEN|RCC_APB2ENR_AFIOEN;
    AFIO->MAPR=AFIO_MAPR_SWJ_CFG_DISABLE; /* C6 schematic only; PA13/14 drive segments. */
    GPIOA->BSRR=0x7f7e; GPIOB->BSRR=0xef;
    for(unsigned i=1;i<=6;i++)pin(GPIOA,i,2);
    for(unsigned i=8;i<=14;i++)pin(GPIOA,i,2);
    for(unsigned i=0;i<4;i++)pin(GPIOB,i,2);
    for(unsigned i=5;i<=7;i++)pin(GPIOB,i,8);
}
void board_lamps(uint8_t a,uint8_t b){
    const unsigned lut[]={2,4,8,0};uint32_t on=lut[a]|(lut[b]<<3);
    GPIOA->BSRR=(0x7eu&~on)|(on<<16); /* six active-low LED lines */
}
uint8_t board_buttons(void){
    static uint8_t hold,pulse,mask;
    uint8_t raw=(~GPIOB->IDR>>5)&7;
    if(raw&4){if(hold<100)hold++;}
    else if(hold){if(hold>=3){pulse=4;mask=hold>=80?8:4;}hold=0;}
    if(pulse){pulse--;return (raw&3)|mask;}
    return raw&3;
}
static void digits(void){
    static const uint8_t font[]={0x3f,6,0x5b,0x4f,0x66,0x6d,0x7d,7,0x7f,0x6f};
    static unsigned digit;
    unsigned v=mode==TUNING?(field==0?draft.red:field==1?draft.green:draft.amber):phase+1;
    unsigned d[]={mode+1,mode==TUNING?field+1:0,v/10,v%10};
    uint32_t seg=font[d[digit]]<<8;
    GPIOB->BSRR=15;GPIOA->BSRR=(0x7f00&~seg)|(seg<<16);GPIOB->BRR=1u<<digit;
    digit=(digit+1)%4;
}
void TIM2_IRQHandler(void){if(TIM2->SR&TIM_SR_UIF){TIM2->SR=0;SCH_Update();}}
int main(void){
    tasks_start();SCH_Add_Task(digits,1,1);
    RCC->APB1ENR|=RCC_APB1ENR_TIM2EN;TIM2->PSC=7999;TIM2->ARR=9;TIM2->EGR=1;TIM2->SR=0;
    NVIC_EnableIRQ(TIM2_IRQn);TIM2->DIER=1;TIM2->CR1=1;
    for(;;) SCH_Dispatch_Tasks();
}
