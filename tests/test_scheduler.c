/* Include the implementation to place its clock near wraparound without
 * adding a test-only clock setter to the firmware API. */
#include "../TrafficRB/Core/Src/scheduler.c"
#include <assert.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
static void assertion_failed(int signal_number) { (void)signal_number; _Exit(1); }

static unsigned calls[16];
#define CALLBACK(number) static void callback_##number(void) { calls[number]++; }
CALLBACK(0) CALLBACK(1) CALLBACK(2) CALLBACK(3)
CALLBACK(4) CALLBACK(5) CALLBACK(6) CALLBACK(7)
CALLBACK(8) CALLBACK(9) CALLBACK(10) CALLBACK(11)
CALLBACK(12) CALLBACK(13) CALLBACK(14) CALLBACK(15)
static void (*callbacks[16])(void) = {
    callback_0, callback_1, callback_2, callback_3,
    callback_4, callback_5, callback_6, callback_7,
    callback_8, callback_9, callback_10, callback_11,
    callback_12, callback_13, callback_14, callback_15
};

typedef struct {
    uint32_t id;
    uint32_t deadline;
    uint32_t period;
    unsigned expected_calls;
} ReferenceTask;

static uint32_t random_state = 0x3009;
static uint32_t random_value(void)
{
    random_state = random_state * 1664525u + 1013904223u;
    return random_state;
}

static uint32_t self_id;
static void delete_self(void)
{
    calls[0]++;
    assert(SCH_Delete_Task(self_id));
}

/* An overdue head and a maximum-delay task can be more than INT32_MAX
 * ticks apart even though both individual delays are valid. */
static void test_distant_deadline(void)
{
    const uint32_t starts[] = {0, UINT32_MAX - 8};
    for (unsigned i = 0; i < 2; ++i) {
        SCH_Init();
        now = starts[i];
        calls[0] = calls[1] = 0;
        assert(SCH_Add_Task(callback_0, 0, 0));
        SCH_Update();
        SCH_Update();
        uint32_t due = SCH_Now() + INT32_MAX;
        assert(SCH_Add_Task(callback_1, INT32_MAX, 0));
        SCH_Dispatch_Tasks();
        assert(calls[0] == 1 && calls[1] == 0);
        now = due - 1;
        refresh();
        SCH_Dispatch_Tasks();
        assert(calls[1] == 0);
        SCH_Update();
        SCH_Dispatch_Tasks();
        assert(calls[1] == 1 && count == 0);
    }
    calls[0] = calls[1] = 0;
    puts("PASS scheduler: overdue head with INT32_MAX delay, including clock wrap");
}

int main(void)
{
    signal(SIGABRT, assertion_failed);
    test_distant_deadline();
    SCH_Init();
    assert(!SCH_Add_Task(NULL, 0, 1));
    assert(!SCH_Add_Task(callback_0, UINT32_MAX, 0));
    assert(!SCH_Add_Task(callback_0, 0, UINT32_MAX));
    assert(!SCH_Delete_Task(123));

    self_id = SCH_Add_Task(delete_self, 0, 1);
    SCH_Dispatch_Tasks();
    for (unsigned i = 0; i < 10; ++i) {
        SCH_Update();
        SCH_Dispatch_Tasks();
    }
    assert(calls[0] == 1 && count == 0);

    SCH_Init();
    uint32_t first = SCH_Add_Task(callback_1, 5, 0);
    SCH_Add_Task(callback_2, 5, 0);
    SCH_Add_Task(callback_3, 9, 0);
    assert(SCH_Delete_Task(first));
    for (unsigned i = 0; i < 5; ++i) SCH_Update();
    SCH_Dispatch_Tasks();
    assert(calls[1] == 0 && calls[2] == 1 && calls[3] == 0);
    for (unsigned i = 0; i < 4; ++i) SCH_Update();
    SCH_Dispatch_Tasks();
    assert(calls[3] == 1 && count == 0);

    SCH_Init();
    now = UINT32_MAX - 8;
    ReferenceTask reference[16] = {0};
    for (unsigned i = 0; i < 16; ++i) calls[i] = 0;

    for (unsigned step = 0; step < 20000; ++step) {
        unsigned slot = random_value() % 16;
        ReferenceTask *task = &reference[slot];
        unsigned action = (random_value() >> 12) % 3;
        if (action == 0 && task->id) {
            assert(SCH_Delete_Task(task->id));
            task->id = 0;
        } else if (!task->id) {
            unsigned delay = random_value() % 25;
            task->period = random_value() % 16;
            task->deadline = SCH_Now() + delay;
            task->id = SCH_Add_Task(callbacks[slot], delay, task->period);
            assert(task->id);
        }

        unsigned elapsed = random_value() % 5;
        for (unsigned i = 0; i < elapsed; ++i) SCH_Update();
        uint32_t current = SCH_Now();
        for (unsigned i = 0; i < 16; ++i) {
            task = &reference[i];
            if (!task->id || (int32_t)(current - task->deadline) < 0) continue;
            task->expected_calls++;
            if (task->period) {
                do {
                    task->deadline += task->period;
                } while ((int32_t)(current - task->deadline) >= 0);
            } else {
                task->id = 0;
            }
        }
        SCH_Dispatch_Tasks();
        for (unsigned i = 0; i < 16; ++i) {
            assert(calls[i] == reference[i].expected_calls);
        }
    }
    puts("PASS scheduler: self-delete, equal-deadline head deletion, clock wrap,"
         " 20000 randomized operations against an absolute-deadline reference");
    return 0;
}
