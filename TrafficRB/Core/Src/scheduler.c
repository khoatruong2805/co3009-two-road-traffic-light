#include "scheduler.h"
#ifndef HOST_TEST
#include "stm32f1xx.h"
static uint32_t lock(void){ uint32_t p=__get_PRIMASK(); __disable_irq(); return p; }
static void unlock(uint32_t p){ __set_PRIMASK(p); }
#else
static uint32_t lock(void){return 0;}
static void unlock(uint32_t p){(void)p;}
#endif
/* Head deadline is absolute. Later entries hold deltas from their predecessor.
 * All delays must fit in INT32_MAX ticks. Foreground code alone invokes tasks. */
#define CAP 16
typedef struct { void (*fn)(void); uint32_t id, delay, period; } Task;
static Task q[CAP];
static volatile uint32_t now;
static volatile unsigned count,ready;
static uint32_t serial;
static void refresh(void){ready=count && (int32_t)(now-q[0].delay)>=0;}
static void insert(Task t,uint32_t due){
    unsigned i=0; uint32_t prev=0;
    for(;i<count;i++){uint32_t next=prev+q[i].delay;
        /* Compare offsets from now: subtracting two deadlines can exceed
         * INT32_MAX when an overdue task meets a valid maximum delay. */
        if((int32_t)(due-now)<(int32_t)(next-now)) break;
        prev=next;
    }
    t.delay=due-prev;
    if(i<count) q[i].delay-=t.delay;
    for(unsigned j=count;j>i;j--) q[j]=q[j-1];
    q[i]=t;count++;refresh();
}
static void pop(unsigned i){
    if(i+1<count) q[i+1].delay+=q[i].delay;
    for(;i+1<count;i++) q[i]=q[i+1];
    count--;refresh();
}
void SCH_Init(void){uint32_t p=lock();count=ready=serial=now=0;unlock(p);}
uint32_t SCH_Now(void){return now;}
void SCH_Update(void){++now;refresh();}
uint32_t SCH_Add_Task(void(*fn)(void),uint32_t delay,uint32_t period){
    uint32_t p=lock(),id=0;
    if(fn && count<CAP && delay<=INT32_MAX && period<=INT32_MAX){
        do{id=++serial;}while(!id);
        Task t={fn,id,0,period};insert(t,now+delay);
    }
    unlock(p);return id;
}
uint8_t SCH_Delete_Task(uint32_t id){
    uint32_t p=lock();for(unsigned i=0;i<count;i++) if(q[i].id==id){pop(i);unlock(p);return 1;}
    unlock(p);return 0;
}
void SCH_Dispatch_Tasks(void){
    for(;;){uint32_t p=lock();if(!ready){unlock(p);return;}
        Task t=q[0];pop(0);
        /* Maintain phase; skip missed releases instead of bursting GPIO samples. */
        if(t.period){uint32_t overdue=now-t.delay;insert(t,now+(t.period-overdue%t.period));}
        unlock(p);t.fn();
    }
}
