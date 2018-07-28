#include "sm.h"
#include "logging.h"
#include <stdio.h>
#include <assert.h>

static int debug_level = LOG_NONE;

/* Private types
 * ========================================================================== */
struct sm_s{
    uint32_t id;
    char * name;
    sm_state_t *all_states;
    sm_state_t *current_state;
    int pending_event;
};

/* Private data
 * ========================================================================== */
static struct sm_s
sm_pool[NOF_STATEMACHINES];

static uint32_t
nof_sms_in_use = 0;

/* Private functions
 * ========================================================================== */
static void
sm_do_transitions(sm_t sm)
{
    if (sm->pending_event != 0) {

        int next_state_id = sm->current_state->transitions[sm->pending_event];

        sm->pending_event = 0;

        if (next_state_id != 0) {
            LOG(LOG_INFO, "SM #%d: transition %d -> %d",
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
sm_run_state(sm_t sm)
{
    if (sm->current_state->run != NULL) {
        sm->current_state->run();
    }
}


/* Public functions
 * ========================================================================== */
// TODO start_state belongs in the xml model
sm_t sm_new(sm_state_t* state_tbl, sm_state_t* start_state)
{
    assert (state_tbl != NULL);
    assert (start_state != NULL);

    uint32_t new_sm_idx = nof_sms_in_use;
    struct sm_s *new_sm = &sm_pool[new_sm_idx];
    assert (new_sm != NULL);
    nof_sms_in_use++;
    assert (nof_sms_in_use <= NOF_STATEMACHINES);

    new_sm->id = new_sm_idx;
    new_sm->name = "N/A";
    new_sm->all_states = state_tbl;
    new_sm->current_state = start_state;
    new_sm->pending_event = 0;

    LOG(LOG_INFO, "Created state machine #%d", new_sm_idx);

    return new_sm;
}

void sm_run(sm_t sm)
{
    assert(sm);

    LOG(LOG_DBG, "SM:\tid\tstate\tevent\tname\n"
           "\t\t%d\t%d\t%d\t%s",
           sm->id, sm->current_state->id, sm->pending_event, sm->name);
    sm_run_state(sm);
    sm_do_transitions(sm);
}

void sm_send_event(sm_t sm, int event)
{
    sm->pending_event = event;
}
