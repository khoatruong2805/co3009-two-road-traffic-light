#pragma once
#include <stdint.h>
void timer_set(uint32_t ticks);
void timer_stop(void);
void timer_run(void);
uint8_t timer_expired(void);
