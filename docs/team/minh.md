# Minh - Board, buttons and mode switching

Compare TrafficRB.ioc with Figure 2 in the announcement. Verify the D3-D10 mapping, PB8/PB9 I2C remap, and Serial Wire on PA13/PA14. Trace the transition from an amber phase to Manual, including a simultaneous MODE and NEXT press.

The host tests cover debounce, long presses, simultaneous keys and stale timers. Reproduce them, then use BOARD_BRINGUP=1 on the bench to measure both modules' color codes and every button's active level. Record the I2C scan and verify repeated flashing through SWD.

Update board_config.h with measured values and add dated evidence to docs/verification.md. Keep test observations and calibration changes in meaningful commits.

Relevant files include tests/test_controller.c, TrafficRB/TrafficRB.ioc, TrafficRB/Core/Inc/board_config.h and docs/verification.md. A suitable branch name is feature/board-calibration.
