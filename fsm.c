#include "fsm.h"
#include "logging.h"
#include <stdio.h>
#include <assert.h>

static int debug_level = LOG_NONE;

/* Private types
 * ========================================================================== */
struct fsm_s{
    uint32_t id;
    char * name;
    fsm_state_t *all_states;
    fsm_state_t *current_state;
    int pending_event;
};

/* Private data
 * ========================================================================== */
static struct fsm_s
fsm_pool[NOF_STATEMACHINES];

static uint32_t
nof_fsms_in_use = 0;

/* Private functions
 * ========================================================================== */
static void
fsm_try_entry_action(fsm_t *fsm)
{
    if (fsm->current_state->on_entry != NULL) {
        fsm->current_state->on_entry();
    }
}

static void
fsm_try_exit_action(fsm_t *fsm)
{
    if (fsm->current_state->on_exit != NULL) {
        fsm->current_state->on_exit();
    }
}

static void
fsm_try_tick_action(fsm_t *fsm)
{
    if (fsm->current_state->on_tick != NULL) {
        fsm->current_state->on_tick();
    }
}

static void
fsm_try_transition(fsm_t *fsm)
{
    if (fsm->pending_event != 0) {

        int next_state_id = fsm->current_state->transitions[fsm->pending_event];

        fsm->pending_event = 0;

        if (next_state_id != 0) {
            LOG(LOG_INFO, "FSM #%d: transition %d -> %d",
                   fsm->id, fsm->current_state->id, next_state_id);

            fsm_try_exit_action(fsm);
            fsm->current_state = &fsm->all_states[next_state_id];
            fsm_try_entry_action(fsm);
        }
    }
}

/* Public functions
 * ========================================================================== */
// TODO start_state belongs in the xml model
fsm_t* fsm_new(fsm_state_t* state_tbl, fsm_state_t* start_state)
{
    assert (state_tbl != NULL);
    assert (start_state != NULL);

    uint32_t new_fsm_idx = nof_fsms_in_use;
    struct fsm_s *new_fsm = &fsm_pool[new_fsm_idx];
    assert (new_fsm != NULL);
    nof_fsms_in_use++;
    assert (nof_fsms_in_use <= NOF_STATEMACHINES);

    new_fsm->id = new_fsm_idx;
    new_fsm->name = "N/A";
    new_fsm->all_states = state_tbl;
    new_fsm->current_state = start_state;
    new_fsm->pending_event = 0;

    LOG(LOG_INFO, "Created state machine #%d", new_fsm_idx);

    return new_fsm;
}

void fsm_tick(fsm_t *fsm)
{
    assert(fsm);

    LOG(LOG_DBG, "FSM:\tid\tstate\tevent\tname\n"
           "\t\t%d\t%d\t%d\t%s",
           fsm->id, fsm->current_state->id, fsm->pending_event, fsm->name);
    fsm_try_tick_action(fsm);
    fsm_try_transition(fsm);
}

void fsm_send_event(fsm_t *fsm, int event)
{
    fsm->pending_event = event;
}

int fsm_get_state_id(fsm_t *fsm)
{
	return fsm->current_state->id;
}

struct fsm_state_s* fsm_get_state(fsm_t *fsm)
{
	return fsm->current_state;
}

