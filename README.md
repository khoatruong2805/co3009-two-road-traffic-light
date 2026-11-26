# CO3009 - Two-Road Traffic Light Controller

STM32 firmware for the XNUCLEO-F103RB with Automatic, Manual and Timing Adjustment modes, DHT20 readings and a 16x2 LCD. A cooperative scheduler handles application tasks, with constant-time SCH_Update in the 10 ms timer interrupt.

The application builds in STM32CubeIDE and passes host-side functional tests. The C6 Proteus circuit runs the shared traffic-control code. Physical-board calibration and demonstration are pending. See the requirement-by-requirement [verification record](docs/verification.md).

| Component | Location |
|---|---|
| STM32CubeIDE board project | [TrafficRB](TrafficRB), STM32F103RBTx |
| Runnable Proteus traffic demo | [TrafficC6.pdsprj](simulation/TrafficC6.pdsprj) and [firmware](simulation/firmware) |
| Supplied assignment circuit | [Assignment.pdsprj](simulation/Assignment.pdsprj), power rail configured, two VSM models missing |
| Tests and verification evidence | [tests](tests) and [verification record](docs/verification.md) |

## Build and flash

1. Clone https://github.com/khoatruong2805/co3009-two-road-traffic-light.git.
2. In STM32CubeIDE, use **File > Import > General > Existing Projects into Workspace**, then select **TrafficRB**.
3. Open **TrafficRB.ioc** and verify **NUCLEO-F103RB**, **SYS > Debug > Serial Wire**. Generated sources and HAL/CMSIS are included. Preserve USER CODE regions when regenerating.
4. Use **Project > Build Project**. Debug output includes TrafficRB/Debug/TrafficRB.elf, .hex and .bin. Verified with STM32CubeIDE 1.7.0, STM32CubeF1 V1.8.7 and ARM GCC 9.3.1.
5. Connect the XNUCLEO-F103RB through ST-LINK. Create a **STM32 C/C++ Application** run/debug configuration using ST-LINK, SWD and the TrafficRB ELF.

Use RB firmware when flashing the physical board. The C6 HEX uses a different memory layout and GPIO adapter.

### Board calibration

Set **BOARD_BRINGUP=1** in TrafficRB/Core/Inc/board_config.h, build and flash. This bench diagnostic cycles all four A:B codes on both light modules, toggles PA5/PC9 every 0.5/1 s, samples raw buttons and scans I2C.

Watch these debugger Live Expressions:

- **diag_ab** cycles through 00, 01, 10 and 11, holding each for 2 s.
- **diag_buttons_raw** contains buttons 1 to 4 in bits 0 to 3. **diag_user_raw** is the USER button level.
- **diag_i2c_count** and **diag_i2c_addr** contain responding **7-bit** addresses. Allow approximately 2 s for the scan.

Record the observed colors independently for both modules. Update LIGHT_CODES_R1, LIGHT_CODES_R2, BUTTON_ACTIVE_LOW_MASK and LCD_ADDR7 from measurements. Check the PCF8574 wiring in board_config.h. Set **BOARD_BRINGUP=0**, rebuild and flash the application after calibration.

## Wiring

| Header | STM32 pin | Function |
|---|---|---|
| D3, D4 | PB3, PB5 | TL1_A, TL1_B |
| D5, D6 | PB4, PB10 | TL2_A, TL2_B |
| D7, D8 | PA8, PA9 | MODE, NEXT |
| D9, D10 | PC7, PB6 | PLUS, MINUS |
| SCL, SDA | PB8, PB9 | Remapped I2C1 at 100 kHz |
| SWDIO, SWCLK | PA13, PA14 | Serial Wire debugging |

JTAG is disabled to release PB3/PB4 while SWD remains enabled. TIM2 uses HSI at 8 MHz, prescaler 7999 and auto-reload 9 for a 10 ms tick. DHT20 uses address 0x38. The LCD defaults to 0x27, subject to the board scan.

## Operation

| Mode | Behavior and controls |
|---|---|
| Automatic | Road 1 green/road 2 red for 5 s, road 1 amber/road 2 red for 2 s, then the opposite direction for 5 s and 2 s. MODE enters Manual. |
| Manual | Entry sets both roads red. NEXT advances one phase through the same four-phase sequence. MODE enters Timing Adjustment. |
| Timing Adjustment | Both roads stay red. NEXT selects red, green or amber. PLUS/MINUS change the selected value by 1 s within 1 to 99 s. MODE applies values and returns to Automatic when red = green + amber. |

Invalid timing keeps the controller in Timing Adjustment and displays **Need R = G + A**. Valid settings restart a full road 1 green phase. Settings are held in RAM and return to 7/5/2 s after reset. Simultaneous MODE and other key events give MODE priority.

LCD row 1 shows the current mode or timing menu. Row 2 shows temperature/humidity or the timing error. Invalid or missing sensor data produces dashes. Each physical press produces one adjustment.

## Proteus

Open simulation/TrafficC6.pdsprj and press **Run (F12)**. U1's Program File is **firmware/traffic_demo.hex**, relative to the circuit. Keep those files together.

The C6 circuit has six active-low LED outputs and three buttons:

- BT0 selects MODE.
- BT1 selects NEXT.
- A short BT2 press followed by release selects PLUS. Holding BT2 for at least 0.8 s before release selects MINUS.

The four seven-segment digits show the mode (1/2/3), selected timing field (1/2/3, or 0 outside adjustment), and a two-digit value (phase number or selected duration). This adapter runs the shared scheduler and FSM sources. DHT20/LCD validation uses host mocks and later board measurements.

The included demo HEX can be rebuilt with:

```bash
python simulation/build_sim.py
# Or specify the installed ARM toolchain:
python simulation/build_sim.py --bin "<GNU Tools for STM32 bin directory>"
```

The assignment circuit has a corrected 3.3 V power-rail binding. Proteus 8.10 reports missing simulation models for U1 (STM32F103R8T6) and U12 (SEN-18364). See the [verification record](docs/verification.md) for the observed errors.

## Tests

```bash
python tests/run.py
```

Requires Python 3 and a C11 GCC compiler. Set **CC** to a compiler path if needed. On Windows, also add the compiler's bin directory to PATH so its DLLs can load. Tests return a nonzero exit code on failure.

## Report and submission


Complete the cover with names, student IDs, group, repository URL and the submitted commit hash. The announcement states **4 December 2026** as the report/source deadline and schedules the physical-board demo for week 15/16. Section A4 and Step 7 use different PDF filename patterns, so follow LMS instructions.

The announcement requests the supplied report template. After board verification and report completion, merge the agreed work into main, create the final submission tag and submit the PDF with the GitHub link on LMS.
