#pragma once
#include <stdint.h>
/* Debugger Live Expressions. Diag mode is for the bench only. */
extern volatile uint8_t diag_ab, diag_buttons_raw, diag_user_raw, diag_i2c_count, diag_i2c_addr[8];
void bringup_start(void);
