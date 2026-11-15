#pragma once
#include <stdint.h>
void SCH_Init(void);
uint32_t SCH_Add_Task(void (*fn)(void), uint32_t delay, uint32_t period);
uint8_t SCH_Delete_Task(uint32_t id);
void SCH_Update(void);
void SCH_Dispatch_Tasks(void);
uint32_t SCH_Now(void);
