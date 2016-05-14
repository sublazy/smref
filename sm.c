#include "sm.h"
#include "logging.h"
#include <stdio.h>
#include <assert.h>

static int debug_level = LOG_NONE;

/* Private functions
 * ========================================================================== */

static void
sm_do_transitions(sm_t *sm)
{
    if (sm->pending_event != 0) {

        int next_state_id = sm->current_state->transitions[sm->pending_event];

        sm->pending_event = 0;

        if (next_state_id != 0) {
            printf("SM #%d: transition %d -> %d\n",
                   sm->id, sm->current_state->id, next_state_id);

            if (sm->current_state->on_exit != NULL) {
                sm->current_state->on_exit();
            }

            sm->current_state = &sm->all_states[next_state_id];

            if (sm->current_state->on_entry != NULL) {
                sm->current_state->on_entry();
            }
        }
    }
}

static void
sm_run_state(sm_t *sm)
{
    if (sm->current_state->run != NULL) {
        sm->current_state->run();
    }
}


/* Public functions
 * ========================================================================== */

void sm_run(sm_t *sm)
{
    assert(sm);

    printf("SM:\tid\tstate\tevent\tname\n");
    printf("\t%d\t%d\t%d\t%s\n",
           sm->id, sm->current_state->id, sm->pending_event, sm->name);
    sm_run_state(sm);
    sm_do_transitions(sm);
}

void sm_send_event(sm_t *sm, int event)
{
    sm->pending_event = event;
}

void sm_print_tx_table(sm_t *sm)
{
    for (int s = 0; s < sm->numof_states; s++) {
        sm_state_t state = sm->all_states[s];
        printf("state %d:\t", state.id);

        for (int e = 0; e < sm->numof_events; e++) {
            printf("  %d\t", state.transitions[e]);
        }
        printf("\n");
    }
}
