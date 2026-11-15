#pragma once
#include <stdint.h>
typedef enum { AUTOMATIC, MANUAL, TUNING } Mode;
typedef enum { R1_GREEN, R1_AMBER, R2_GREEN, R2_AMBER, BOTH_RED } Phase;
typedef struct {uint8_t red, green, amber;} Timing;
extern Mode mode; extern Phase phase; extern Timing active,draft;
extern uint8_t field, invalid;
void mode_run(void);
void app_init(void);
