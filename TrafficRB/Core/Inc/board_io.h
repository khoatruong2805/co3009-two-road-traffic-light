#pragma once
#include <stdint.h>
uint8_t board_buttons(void);
void board_lamps(uint8_t road1,uint8_t road2);
void board_init(void);
int bus_write(uint8_t address,const uint8_t *data,unsigned len);
int bus_read(uint8_t address,uint8_t *data,unsigned len);
