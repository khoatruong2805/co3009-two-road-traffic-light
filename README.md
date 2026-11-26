# CO3009 Two-Road Traffic Light Controller

The XNUCLEO-F103RB controls two sets of traffic lights at an intersection. Four buttons let you switch modes, step through phases and set the timings. A 16×2 LCD shows the mode and readings from a DHT20 temperature and humidity sensor.

## Build and flash

1. Clone this repository. In STM32CubeIDE, choose **File > Import > General > Existing Projects into Workspace** and select [TrafficRB](TrafficRB).
2. Open `TrafficRB.ioc`. Check that the board is **NUCLEO-F103RB** and **SYS > Debug** is set to **Serial Wire**. This frees PB3 and PB4 for the lights while keeping SWD available.
3. Select the Debug configuration and **Project > Build Project**. Connect ST-LINK and flash `TrafficRB/Debug/TrafficRB.elf` over SWD.

The project includes HAL and CMSIS. The recorded build used STM32CubeIDE 1.7.0, STM32CubeF1 1.8.7 and ARM GCC 9.3.1.

## Board setup

| Header | STM32 pins | Function |
|---|---|---|
| D3, D4 | PB3, PB5 | Road 1 outputs A, B |
| D5, D6 | PB4, PB10 | Road 2 outputs A, B |
| D7, D8 | PA8, PA9 | MODE, NEXT |
| D9, D10 | PC7, PB6 | PLUS, MINUS |
| SCL, SDA | PB8, PB9 | I2C1 at 100 kHz |
| SWDIO, SWCLK | PA13, PA14 | ST-LINK |

For the first board run, set `BOARD_BRINGUP` to `1` in [board_config.h](TrafficRB/Core/Inc/board_config.h), rebuild and flash. Open **Live Expressions** in the debugger:

| Watch | What to record |
|---|---|
| `diag_ab` | Each A:B code stays on for 2 s. Write down the color on each road for all four codes. |
| `diag_buttons_raw`, `diag_user_raw` | Press and release each button. Bits 0–3 correspond to buttons 1–4. USER has its own reading. |
| `diag_i2c_count`, `diag_i2c_addr` | Allow about 2 s for the scan. The DHT20 uses 7-bit address `0x38`. Check the LCD address against the default `0x27`. |

Update the two color tables, button polarity mask and LCD settings from those measurements. Set `BOARD_BRINGUP` back to `0`, rebuild and flash to run the controller.

## Controls

| Mode | How it works |
|---|---|
| Automatic | Road 1 gets 5 s green and 2 s amber while road 2 stays red. The roads then swap. Press MODE for Manual. |
| Manual | Both roads start red. Press NEXT for each successive phase, or MODE to adjust the timings. |
| Timing adjustment | Both roads stay red. NEXT selects red, green or amber. Each PLUS or MINUS press changes the value by 1 s within the 1–99 s range. |

Press MODE to save the timings. If red equals green plus amber, road 1 starts a full green phase. Otherwise the LCD shows `Need R = G + A` and the menu stays open. Reset restores the defaults. The second LCD row normally shows temperature and humidity, with dashes when a reading is unavailable.

## Proteus and tests

Open [TrafficC6.pdsprj](simulation/TrafficC6.pdsprj) and press **F12**. Keep `firmware/traffic_demo.hex` next to the schematic in its supplied subfolder. This HEX targets the simulated C6. Use the RB build for the physical board.

BT0 changes mode and BT1 advances the phase or menu field. Release BT2 after a short press to add one second, or hold it for at least 0.8 s and release to subtract one. The four digits show the mode, field and two-digit phase number or duration.

To rebuild the demo, run `python simulation/build_sim.py`. If needed, pass `--bin "<GNU Tools for STM32 bin directory>"`. The [assignment schematic](simulation/Assignment.pdsprj) needs two simulation models missing from our Proteus installation.

Run `python tests/run.py` from the repository root with Python 3 and GCC (C11). Set `CC` to the compiler path if needed. On Windows, add the MinGW bin directory to `PATH`. See the [test results and remaining board checks](docs/verification.md).

## Submission

Submit the report PDF and repository link on LMS by **4 December 2026**. Each member commits their own work. Tag the submitted revision on `main` and put its commit hash, the repository link, names, student IDs and group on the report cover. Use the course template and check LMS for the filename, as A4 and Step 7 give different formats. The board demo is scheduled for week 15/16.
