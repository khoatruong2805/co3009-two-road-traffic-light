# Assignment verification record

Last software review: **22 November 2026**.

**Conclusion:** the tested software behavior passes the checks below. Full assignment completion remains pending physical-board verification, individual member contributions, and the final report/submission. A successful host test is not proof of electrical correctness.

## Requirements and evidence

| Assignment requirement | Evidence | Status / remaining work |
|---|---|---|
| STM32F103RB, SWD, prescribed wiring, 10 ms tick | CubeIDE target and IOC checked; HSI 8 MHz, TIM2 PSC 7999 / ARR 9; PB8/PB9 I2C1 remap | Software configuration checked; confirm wiring and clock on the board |
| Add/delete/update/dispatch scheduler; O(1) interrupt update; one-shot tasks | Source review: interrupt increments time and checks only the head; host tests cover queue operations, equal deadlines, cancellation, capacity, one-shot, delayed dispatch and self-deletion | Passed software checks |
| Scheduler timing robustness | Clock-wrap check and 20,000 randomized operations against an absolute-deadline reference | Passed |
| Separate Auto, Manual and Tuning FSM tasks and supporting modules | Separate source/header pairs; no HAL calls in FSMs; nine application tasks | Passed source review |
| No blocking application delays | No HAL_Delay in application sources; I2C operations have bounded timeouts | Passed source review; measure actual timing under bus faults on hardware |
| Automatic four-phase cycle | Tick-based boundary assertions; opposing road red on each output request | Passed host tests; measure physical phase durations |
| Manual phase selection and safe mode transitions | Amber exit, MODE+NEXT priority, both-red entry, stale-timer cancellation | Passed host tests |
| Button debounce and long press | Mock GPIO samples, held/released presses | Passed host tests; measure actual button polarity |
| Adjust red/green/amber using buttons and LCD | All fields clamp at 1 and 99 s; invalid 8/5/2 rejected; valid 8/6/2 accepted; active values unchanged after invalid confirmation | Passed host tests |
| LCD output | PCF8574 enable-pulse decoder checks initialization, mode text, timing/error text and 25.0C / 50.0% readings | Passed mock-bus tests; actual address and backpack mapping unverified |
| DHT20 temperature/humidity and recovery | Calibration-register recovery, known frame, CRC error, missing I2C, busy timeout and recovery | Passed mock-bus tests; actual sensor unverified |
| Stability | 360,000 ticks = one virtual hour; traffic continued and every phase request had a red opposing road | Passed simulated-time logic test; physical endurance run pending |
| Board bring-up | Diagnostic mode implements code sweep and I2C scan | Not executed on the team's board |
| Each member commits using their own account | Member work sheets and CONTRIBUTING instructions available | Pending substantive personal commits; shared setup commits do not satisfy this requirement |
| FSM diagrams and report | English LaTeX body contains mode/Auto/Manual/Tuning diagrams and transition analysis | Body prepared; final cover, IDs, contribution evidence and PDF pending |
| Submission | README describes PDF, GitHub access and final commit/tag | Final PDF and matching submission revision pending |

## Tool runs

- **STM32CubeIDE 1.7.0:** RB clean builds completed with 0 errors and 0 warnings. Debug image: text 18,844 bytes, data 128 bytes, BSS 2,144 bytes. No ST-LINK was connected.
- **Host GCC:** all controller and scheduler suites pass. Assertions return a nonzero exit code on failure.
- **Proteus 8.10, supplied assignment schematic:** the original failed with “No power supply specified for net 3.3V in Power Rail Configuration.” The supplied copy fixes that rail, but still reports “No model specified for U1.” and “No model specified for U12.” U1 is STM32F103R8T6 and U12 is SEN-18364. Changing a HEX path cannot supply the missing executable models.
- **Proteus 8.10, C6 comparison circuit:** the shared firmware runs with ANIMATING status. Both vertical-green/horizontal-red and the opposite direction have been observed. The rerun on 22 November also started successfully. This circuit does not include the DHT20/LCD and is not a complete simulation of the assignment board.
- **Physical XNUCLEO-F103RB:** not tested here, as assigned to the team. No physical color table, button-level table, sensor readings, or endurance result is claimed.

![Shared firmware running in the C6 comparison circuit](proteus-c6.png)

## Reproduce the software tests

From the repository root:

~~~sh
python tests/run.py
~~~

Requires Python 3 and a C11 GCC compiler. On Windows, add the MinGW bin directory to PATH so its runtime DLLs can be found. CC may specify the full gcc.exe path. The test runner builds temporary executables outside the repository.

The virtual clock validates transitions in scheduler ticks. It does not measure wall-clock phase timing, electrical edges, I2C latency or interrupt jitter. On hardware, record successive phase edges against the same time base and compare their intervals with the configured durations.

The DHT20 protocol was checked against the [ASAIR DHT20 datasheet](https://asairsensors.com/wp-content/uploads/2021/09/Data-Sheet-DHT20-Humidity-and-Temperature-Module-ASAIR-V1.0.03.pdf). Tests include an initially uncalibrated status, restoring all three registers, and checking status again before measurement.

## Board verification sheet

Create a separate result record with the tested commit, date and operator.

1. Verify repeatable SWD flashing and the requested LED bring-up checks.
2. For each A:B combination 00, 01, 10 and 11, record the color of **each module separately**, including A/B order. Update the board tables from measurements.
3. Record released/pressed levels for D7 through D10 and USER. Complete the I2C scan and record both 7-bit device addresses; verify the LCD backpack mapping.
4. Measure phase boundaries over two complete cycles. Enter Manual during amber. Test invalid tuning 8/5/2, followed by valid 8/6/2.
5. Check real temperature/humidity and every LCD mode. Disconnect the sensor in a separate trial; confirm that measurements become unavailable while traffic continues.
6. Choose and document an endurance duration, start/end times, observed failures and recovery. The assignment leaves its minute count unspecified; do not invent a required duration or record a pass before performing it.
7. Each member commits their own implementation, measured results or report work. Finalize contribution percentages from actual work.
8. Merge the team's completed work, prepare the final report PDF and record its matching commit/tag on the cover. Confirm the repository is accessible to the lecturer.

The assignment names the PDF in two ways: A4 uses CO3009_StudenID1_StudentID2_..._Report.pdf, while Step 7 uses CO3009_Nhom<XX>_Report.pdf. Confirm the intended convention with the submission instructions. The explicit submission is the PDF plus repository link and revision; keep source and schematic files in the repository.
