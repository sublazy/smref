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
    fsm_t dummy_processor = fsm_new(dummy_processor_table, &dummy_processor_table[STATE_WAITING]);
	WVFAIL(dummy_processor == NULL);

	fsm_tick(dummy_processor);
	fsm_send_event(dummy_processor, EVENT_DATA_AVAILABLE);
}

