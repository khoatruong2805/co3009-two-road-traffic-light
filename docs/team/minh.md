# Minh — Board setup and mode switching

Check [TrafficRB.ioc](../../TrafficRB/TrafficRB.ioc) against Figure 2 in the assignment. Flash twice over SWD. In bring-up mode, record the color for each A:B code on each road, the idle and pressed levels of every button, and both I2C addresses. Update [board_config.h](../../TrafficRB/Core/Inc/board_config.h) with the measured values.

Switch to Manual during amber. Both roads should stay red until the next NEXT press, even if MODE and NEXT were pressed together.
