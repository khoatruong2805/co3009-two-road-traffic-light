#pragma once
#include <stdint.h>
enum { KEY_MODE, KEY_NEXT, KEY_PLUS, KEY_MINUS };
void button_run(void);
uint8_t button_take(unsigned n);
uint8_t button_long(unsigned n);
void button_clear(void);
