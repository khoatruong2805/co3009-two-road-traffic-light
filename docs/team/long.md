# Long - DHT20, LCD and Timing Adjustment

Trace initialization, calibration-register recovery, the AC 33 00 measurement command, conversion wait, seven-byte read, status/CRC checks and fixed-point conversion. Explain how the LCD displays a timing error and how invalid values are prevented from becoming active.

The host tests inspect PCF8574 enable pulses and LCD text, timing limits, CRC errors and bus failures. Reproduce these tests and compare the assumed backpack wiring with the physical LCD.

On the board, record the actual LCD address, observe real sensor measurements, check recovery after a sensor disconnect and measure the time taken to update the display. Commit the measured settings and report observations under your own account.

Relevant files include tests/test_controller.c, TrafficRB/Core/Src/dht20.c, TrafficRB/Core/Src/lcd.c, TrafficRB/Core/Inc/board_config.h and report/body.tex. A suitable branch name is feature/environment-board-verification.
