#pragma once
/* Set to 1 only to measure raw levels/codes on the bench. */
#ifndef BOARD_BRINGUP
#define BOARD_BRINGUP 0
#endif
/* Replace with measured A:B codes on the actual two Grove boards. */
#define LIGHT_CODES_R1 {1,2,3,0} /* red, green, amber, off */
#define LIGHT_CODES_R2 {1,2,3,0}
#define BUTTON_ACTIVE_LOW_MASK 0x0fu
#define LCD_ADDR7 0x27u
/* Typical PCF8574 backpack; verify RS=P0, RW=P1, EN=P2, BL=P3, D4..D7=P4..P7. */
