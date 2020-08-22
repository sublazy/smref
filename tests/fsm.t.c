#include "wvtest.h"
#include <stdio.h>

#include "../fsm.h"

/* Types
 * ---------------------------------------------------------------------- */
enum state {STATE_NULL, STATE_WAITING, STATE_PROCESSING};
enum event {EVENT_NULL, EVENT_DATA_AVAILABLE, EVENT_PROCESSING_DONE};

struct dummy_processor_s {
    fsm_t *fsm;
    int num_of_cores;
    int generation;
};

/* Forward declarations
 * ---------------------------------------------------------------------- */
void processing_on_entry(void *user_data);
void processing_on_exit(void *user_data);
void processing_on_tick(void *user_data);

/* Data
 * ---------------------------------------------------------------------- */
static int active_cpu_cores = 0;
static int executed_ticks = 0;

static fsm_state_t dummy_processor_table [] = {
    [STATE_WAITING] = {
        .id = STATE_WAITING,
        .transitions = {
            [EVENT_DATA_AVAILABLE] = STATE_PROCESSING,
        },
    },
    [STATE_PROCESSING] = {
        .id = STATE_PROCESSING,
        .transitions = {
            [EVENT_PROCESSING_DONE] = STATE_WAITING,
        },
        .on_entry = processing_on_entry,
        .on_exit = processing_on_exit,
        .on_tick = processing_on_tick,
    },
};

/* Helper functions
 * ---------------------------------------------------------------------- */
void processing_on_entry(void *user_data)
{
    struct dummy_processor_s *self = user_data;
    active_cpu_cores += self->num_of_cores;
}

void processing_on_exit(void *user_data)
{
    struct dummy_processor_s *self = user_data;
    active_cpu_cores -= self->num_of_cores;
}

void processing_on_tick(void *user_data)
{
    struct dummy_processor_s *self = user_data;
    executed_ticks++;
}

/* Tests
 * ---------------------------------------------------------------------- */
WVTEST_MAIN("FSM-jinn engine tests")
{
    struct dummy_processor_s cpu_desktop = {};
    cpu_desktop.num_of_cores = 4;
    cpu_desktop.generation = 7;
    cpu_desktop.fsm = fsm_new(
            dummy_processor_table, &dummy_processor_table[STATE_WAITING],
            (void*) &cpu_desktop);

	WVFAIL(cpu_desktop.fsm == NULL);
	WVPASS(fsm_get_state_id(cpu_desktop.fsm) == STATE_WAITING);

	// Just ticking it shouldn't cause state change.
	fsm_tick(cpu_desktop.fsm);
	WVPASS(fsm_get_state_id(cpu_desktop.fsm) == STATE_WAITING);

    // Make sure that we can also obtain the full state struct, not just the id.
    WVPASS(fsm_get_state(cpu_desktop.fsm) == &dummy_processor_table[STATE_WAITING]);

	// Let's trigger a state transition.
	fsm_send_event(cpu_desktop.fsm, EVENT_DATA_AVAILABLE);
	// It's not supposed to change state without a tick.
	WVPASS(fsm_get_state_id(cpu_desktop.fsm) == STATE_WAITING);

    // Processing-on-entry action should not be executed yet.
    WVPASSEQ(active_cpu_cores, 0);
    WVPASSEQ(executed_ticks, 0);

	fsm_tick(cpu_desktop.fsm);
	// After the tick we expect the new state.
	WVPASS(fsm_get_state_id(cpu_desktop.fsm) == STATE_PROCESSING);

    // Processing-on-entry action should have ran by now.
    WVPASSEQ(active_cpu_cores, cpu_desktop.num_of_cores);
    WVPASSEQ(executed_ticks, 0);

	fsm_tick(cpu_desktop.fsm);
    WVPASSEQ(executed_ticks, 1);
	WVPASS(fsm_get_state_id(cpu_desktop.fsm) == STATE_PROCESSING);

	fsm_tick(cpu_desktop.fsm);
    WVPASSEQ(executed_ticks, 2);
	WVPASS(fsm_get_state_id(cpu_desktop.fsm) == STATE_PROCESSING);
    WVPASS(fsm_get_state(cpu_desktop.fsm) == &dummy_processor_table[STATE_PROCESSING]);

	fsm_send_event(cpu_desktop.fsm, EVENT_PROCESSING_DONE);
	WVPASS(fsm_get_state_id(cpu_desktop.fsm) == STATE_PROCESSING);
    WVPASSEQ(active_cpu_cores, cpu_desktop.num_of_cores);

	fsm_tick(cpu_desktop.fsm);
    // Just before state transition, on_tick action of the previous state runs
    // one last time.
    WVPASSEQ(executed_ticks, 3);
    WVPASSEQ(active_cpu_cores, 0);
	WVPASS(fsm_get_state_id(cpu_desktop.fsm) == STATE_WAITING);

	fsm_tick(cpu_desktop.fsm);
    WVPASSEQ(executed_ticks, 3);
	WVPASS(fsm_get_state_id(cpu_desktop.fsm) == STATE_WAITING);

	fsm_tick(cpu_desktop.fsm);
    WVPASSEQ(executed_ticks, 3);
	WVPASS(fsm_get_state_id(cpu_desktop.fsm) == STATE_WAITING);
}

