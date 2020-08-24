#include "wvtest.h"
#include <stdio.h>

#include "../fsm.h"
#include "../fsm.c"

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
void waiting_on_tick(void *user_data);

/* Data
 * ---------------------------------------------------------------------- */
static int active_cpu_cores = 0;
static int executed_ticks = 0;

#ifdef SPARSE
static struct fsm_state dummy_processor_lut [] = {
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
#endif

#ifdef DENSE
static uint8_t dense_processor_transition_lut [] = {
    // LUT header: size of the array, number of states,
    14, 2,

    // state header: state id, number of exits, state actions, action lut offset,
    [2] = STATE_WAITING, 1, (ACTION_MASK_TICK), 0,
        // event entries: exit event, destination state,
        EVENT_DATA_AVAILABLE, STATE_PROCESSING,

    // state header: state id, number of exits, state actions, action lut offset,
    [8] = STATE_PROCESSING, 1, (ACTION_MASK_ENTRY | ACTION_MASK_EXIT | ACTION_MASK_TICK), 3,
        // event entries: exit event, destination state,
        EVENT_PROCESSING_DONE, STATE_WAITING,
};

static fsm_action_ptr_t dense_processor_action_lut[] = {
    // 1 actions hooks of STATE_WAITING:
    NULL,
    NULL,
    waiting_on_tick,
    // 3 actions hooks of STATE_PROCESSING:
    processing_on_entry,
    processing_on_exit,
    processing_on_tick,
};
#endif



/* Helper functions
 * ---------------------------------------------------------------------- */
void processing_on_entry(void *user_data)
{
    printf("--- processing_on_entry\n");
    struct dummy_processor_s *self = user_data;
    active_cpu_cores += self->num_of_cores;
}

void processing_on_exit(void *user_data)
{
    printf("--- processing_on_exit\n");
    struct dummy_processor_s *self = user_data;
    active_cpu_cores -= self->num_of_cores;
}

void processing_on_tick(void *user_data)
{
    printf("--- processing_on_tick\n");
    struct dummy_processor_s *self = user_data;
    executed_ticks++;
}

void waiting_on_tick(void *user_data)
{
    (void)user_data;
    printf(">>> waiting...\n");
}

/* Tests
 * ---------------------------------------------------------------------- */
#if 0
WVTEST_MAIN("FSM-jinn engine tests")
{
    struct dummy_processor_s cpu_desktop = {};
    cpu_desktop.num_of_cores = 4;
    cpu_desktop.generation = 7;
    cpu_desktop.fsm = fsm_new(
            dummy_processor_lut, &dummy_processor_lut[STATE_WAITING],
            (void*) &cpu_desktop);

	WVFAIL(cpu_desktop.fsm == NULL);
	WVPASS(fsm_get_state_id(cpu_desktop.fsm) == STATE_WAITING);

	// Just ticking it shouldn't cause state change.
	fsm_tick(cpu_desktop.fsm);
	WVPASS(fsm_get_state_id(cpu_desktop.fsm) == STATE_WAITING);

    // Make sure that we can also obtain the full state struct, not just the id.
    WVPASS(fsm_get_state(cpu_desktop.fsm) == &dummy_processor_lut[STATE_WAITING]);

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
    WVPASS(fsm_get_state(cpu_desktop.fsm) == &dummy_processor_lut[STATE_PROCESSING]);

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
#endif



WVTEST_MAIN("FSM-jinn dense LUT tests")
{
    // Reset the global state
    executed_ticks = 0;

    struct dummy_processor_s cpu_desktop = {};
    cpu_desktop.num_of_cores = 4;
    cpu_desktop.generation = 7;
    cpu_desktop.fsm = fsm_new((uint8_t*) &dense_processor_transition_lut,
            (void (**)(void*))&dense_processor_action_lut, STATE_WAITING, (void*) &cpu_desktop);

    // shortcut. m == "machine"
    fsm_t *m = cpu_desktop.fsm;

	WVFAIL(m == NULL);
	WVPASS(fsm_get_state_id(m) == STATE_WAITING);

	// Just ticking it shouldn't cause state change.
	fsm_tick(m);
	WVPASS(fsm_get_state_id(m) == STATE_WAITING);

	// Let's trigger a state transition.
	fsm_send_event(m, EVENT_DATA_AVAILABLE);
	// It's not supposed to change state without a tick.
	WVPASS(fsm_get_state_id(m) == STATE_WAITING);

    // Processing-on-entry action should not be executed yet.
    WVPASSEQ(active_cpu_cores, 0);
    WVPASSEQ(executed_ticks, 0);

	fsm_tick(m);
	// After the tick we expect the new state.
	WVPASS(fsm_get_state_id(m) == STATE_PROCESSING);

    // Processing-on-entry action should have ran by now.
    WVPASSEQ(active_cpu_cores, cpu_desktop.num_of_cores);
    WVPASSEQ(executed_ticks, 0);

	fsm_tick(m);
    WVPASSEQ(executed_ticks, 1);
	WVPASS(fsm_get_state_id(m) == STATE_PROCESSING);

	fsm_tick(m);
    WVPASSEQ(executed_ticks, 2);
	WVPASS(fsm_get_state_id(m) == STATE_PROCESSING);

	fsm_send_event(m, EVENT_PROCESSING_DONE);
	WVPASS(fsm_get_state_id(m) == STATE_PROCESSING);
    WVPASSEQ(active_cpu_cores, cpu_desktop.num_of_cores);

	fsm_tick(m);
    // Just before state transition, on_tick action of the previous state runs
    // one last time.
    WVPASSEQ(executed_ticks, 3);
    WVPASSEQ(active_cpu_cores, 0);
	WVPASS(fsm_get_state_id(m) == STATE_WAITING);

	fsm_tick(m);
    WVPASSEQ(executed_ticks, 3);
	WVPASS(fsm_get_state_id(m) == STATE_WAITING);

	fsm_tick(m);
    WVPASSEQ(executed_ticks, 3);
	WVPASS(fsm_get_state_id(m) == STATE_WAITING);

    WVPASSEQ(*state_descriptor(m, STATE_WAITING), STATE_WAITING);
    WVPASSEQ(*state_descriptor(m, STATE_PROCESSING), STATE_PROCESSING);

    WVPASS(num_exits(state_descriptor(m, STATE_WAITING)) == 1);
    WVPASS(num_exits(state_descriptor(m, STATE_PROCESSING)) == 1);

    WVPASS(first_state(m) == state_descriptor(m, STATE_WAITING));
    WVPASS(next_state(m, state_descriptor(m, STATE_WAITING)) == state_descriptor(m, STATE_PROCESSING));
    WVPASS(next_state(m, state_descriptor(m, STATE_PROCESSING)) == NULL);
}

