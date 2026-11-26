# Khoa - Scheduler and Automatic mode

Review the scheduler, software_timer and fsm_automatic modules in TrafficRB/Core. Explain why the interrupt updates only the clock and head readiness, while insertion and deletion run in foreground. Explain the periodic-task policy when foreground dispatch is late.

The existing tests cover self-deletion, equal deadlines, clock wraparound and randomized queue operations. Review those results, reproduce the tests and select a further contribution based on board behavior or an uncovered case.

During board testing, measure PA5/PC9 intervals and the four automatic phase durations. Record the method, actual values, firmware commit and your name. Update the scheduler and Automatic sections of the report with observations.

Relevant files include tests/test_scheduler.c, TrafficRB/Core/Src/scheduler.c, docs/verification.md. A suitable branch name is feature/scheduler-board-verification.
