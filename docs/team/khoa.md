# Khoa — Scheduler and Automatic mode

Measure the PA5 and PC9 blink intervals, then time all four traffic phases over two cycles. Compare the measured intervals with the 10 ms scheduler tick and the configured green and amber durations.

For the report, use [scheduler.c](../../TrafficRB/Core/Src/scheduler.c) to show what runs in the timer interrupt and how a periodic task is rescheduled after a late dispatch. The [scheduler tests](../../tests/test_scheduler.c) cover clock wrap and equal deadlines.
