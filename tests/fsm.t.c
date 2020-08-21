#include "wvtest.h"

#include "../fsm.h"

enum state {STATE_NULL, STATE_WAITING, STATE_PROCESSING};
enum event {EVENT_NULL, EVENT_DATA_AVAILABLE, EVENT_PROCESSING_DONE};

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
    },
};


WVTEST_MAIN("FSM-jinn engine tests")
{
    fsm_t *dummy_processor = fsm_new(
            dummy_processor_table, &dummy_processor_table[STATE_WAITING]);

	WVFAIL(dummy_processor == NULL);
	WVPASS(fsm_get_state_id(dummy_processor) == STATE_WAITING);

	// Just ticking it shouldn't cause state change.
	fsm_tick(dummy_processor);
	WVPASS(fsm_get_state_id(dummy_processor) == STATE_WAITING);

    // Make sure that we can also obtain the full state struct, not just the id.
    WVPASS(fsm_get_state(dummy_processor) == &dummy_processor_table[STATE_WAITING]);

	// Let's trigger a state transition.
	fsm_send_event(dummy_processor, EVENT_DATA_AVAILABLE);
	// It's not supposed to change state without a tick.
	WVPASS(fsm_get_state_id(dummy_processor) == STATE_WAITING);

	fsm_tick(dummy_processor);
	// After the tick we expect the new state.
	WVPASS(fsm_get_state_id(dummy_processor) == STATE_PROCESSING);
	fsm_tick(dummy_processor);
	WVPASS(fsm_get_state_id(dummy_processor) == STATE_PROCESSING);
	fsm_tick(dummy_processor);
	WVPASS(fsm_get_state_id(dummy_processor) == STATE_PROCESSING);

    WVPASS(fsm_get_state(dummy_processor) == &dummy_processor_table[STATE_PROCESSING]);

	fsm_send_event(dummy_processor, EVENT_PROCESSING_DONE);
	WVPASS(fsm_get_state_id(dummy_processor) == STATE_PROCESSING);

	fsm_tick(dummy_processor);
	WVPASS(fsm_get_state_id(dummy_processor) == STATE_WAITING);
	fsm_tick(dummy_processor);
	WVPASS(fsm_get_state_id(dummy_processor) == STATE_WAITING);
	fsm_tick(dummy_processor);
	WVPASS(fsm_get_state_id(dummy_processor) == STATE_WAITING);
}

