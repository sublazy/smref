#include "fsm.h"
#include "logging.h"
#include <stdio.h>
#include <assert.h>

static int debug_level = LOG_NONE;

#define NO_EVENT 0
#define NO_STATE 0

/* Private types
 * ========================================================================== */
#ifdef SPARSE
struct fsm_s{
    uint32_t id;
    char * name;
    struct fsm_state *transition_lut;
    struct fsm_state *current_state;
    int pending_event;
    void *user_data;
};
#endif

#ifdef DENSE

typedef void (*action_ptr_t)(void*);

struct fsm_s{
    uint32_t id;
    char *name;

    // Pointer to the full state transition LUT
    uint8_t (*transition_lut)[];

    // Pointer to the full state transition LUT
    action_ptr_t (*action_lut)[];

    // Pointer to actions associated with the current state. Points to somewhere in
    // the action_lut, so that we don't need to scan it from start each time.
    action_ptr_t (*state_actions)[];

    uint8_t *current_state;

    int current_state_id;
    int pending_event_id;
    void *user_data;
};
#endif

/* Private data
 * ========================================================================== */
static struct fsm_s
fsm_pool[NOF_STATEMACHINES];

static uint32_t
nof_fsms_in_use = 0;


/* Private functions
 * ========================================================================== */
#ifdef SPARSE
static void
fsm_try_entry_action(fsm_t *fsm)
{
    if (fsm->current_state->on_entry != NULL) {
        fsm->current_state->on_entry(fsm->user_data);
    }
}

static void
fsm_try_exit_action(fsm_t *fsm)
{
    if (fsm->current_state->on_exit != NULL) {
        fsm->current_state->on_exit(fsm->user_data);
    }
}

static void
fsm_try_tick_action(fsm_t *fsm)
{
    if (fsm->current_state->on_tick != NULL) {
        fsm->current_state->on_tick(fsm->user_data);
    }
}

static void
fsm_try_transition(fsm_t *fsm)
{
    if (fsm->pending_event != 0) {

        int next_state_id = fsm->current_state->transitions[fsm->pending_event];

        fsm->pending_event = 0;

        if (next_state_id != 0) {
            LOG(LOG_INFO, "FSM #%u: transition %d -> %d",
                   fsm->id, fsm->current_state->id, next_state_id);

            fsm_try_exit_action(fsm);
            fsm->current_state = &fsm->transition_lut[next_state_id];
            fsm_try_entry_action(fsm);
        }
    }
}
#endif

#ifdef DENSE

#define SIZEOF_LUT_HEADER     2
#define SIZEOF_STATE_HEADER   4
#define SIZEOF_EVENT_ENTRY    2

#define IDX_FIRST_STATE_HEADER  (0 + SIZEOF_LUT_HEADER)

// Offsets relative to LUT header
#define OFFSET_LUT_SIZE    0
#define OFFSET_NUM_STATES  1

// Offsets relative to state header
#define OFFSET_STATE_ID     0
#define OFFSET_NUM_EXITS    1
#define OFFSET_ACTION_MAP   2
#define OFFSET_ACTIONS      3

// Offsets relative to event entry
#define OFFSET_EVENT_ID  0
#define OFFSET_DST_STATE 1

#define ACTION_MASK_ENTRY 0x1
#define ACTION_MASK_EXIT  0x2
#define ACTION_MASK_TICK  0x4

#define OFFSET_ACTION_ENTRY 0
#define OFFSET_ACTION_EXIT  1
#define OFFSET_ACTION_TICK  2


static int num_exits(uint8_t *state)
{
    return *(state + OFFSET_NUM_EXITS);
}

static int id(uint8_t *state)
{
    return *state;
}

static bool is_state_within_lut(const fsm_t *fsm, uint8_t *state)
{
    int transition_lut_size = (*fsm->transition_lut)[OFFSET_LUT_SIZE];
    int offset = state - (uint8_t*) fsm->transition_lut;
    if (offset < transition_lut_size)
        return true;
    else
        return false;
}

static uint8_t* first_state(const fsm_t *fsm)
{
    return &((*fsm->transition_lut)[IDX_FIRST_STATE_HEADER]);
}

// Return a state descriptor following the given one,
// or NULL if the given state is the last one in the LUT.
static uint8_t* next_state(const fsm_t *fsm, uint8_t *state)
{
    assert(state >= first_state(fsm));
    uint8_t *next_state =
        state + SIZEOF_STATE_HEADER + num_exits(state) * SIZEOF_EVENT_ENTRY;

    if (is_state_within_lut(fsm, next_state))
        return next_state;
    else
        return NULL;
}

static uint8_t* state_descriptor(fsm_t *fsm, int wanted_state_id)
{
    uint8_t *state = first_state(fsm);
    while ((id(state) != wanted_state_id) && (state != NULL)) {
        state = next_state(fsm, state);
    }
    return state;
}

// Return pointer to an event entry if the event causes a transition out of the
// given state.  If the event doesn't cause the transition, return NULL.
static uint8_t* event_descriptor(uint8_t *state, int event_id)
{
    int num_exit_events = num_exits(state);
    if (num_exit_events == 0)
        return NULL;

    uint8_t *candidate = state + SIZEOF_STATE_HEADER;
    int checked_events_cnt = 0;

    while ((*candidate != event_id) && (checked_events_cnt < num_exit_events)) {
        candidate += SIZEOF_EVENT_ENTRY;
        checked_events_cnt++;
    }

    if (*candidate == event_id)
        return candidate;
    else
        return NULL;
}

static int target_state_id(uint8_t *event)
{
    if (event != NULL)
        return *(event + OFFSET_DST_STATE);
    else
        return NO_STATE;
}

// Return target state, if there's a pending event to trigger a transition.
// Return 0 if the event doesn't cause any transitions.
static int get_target_state_id(fsm_t *fsm)
{
    // We can asume there is a pending event, because the caller checks it.
    assert(fsm->pending_event_id != 0);
    uint8_t *event = event_descriptor(fsm->current_state, fsm->pending_event_id);
    return target_state_id(event);
}


static action_ptr_t get_state_entry_action(fsm_t *fsm, uint8_t *state)
{
    uint8_t action_map = *(state + OFFSET_ACTION_MAP);

    if (action_map & ACTION_MASK_ENTRY) {
        printf("----- found entry action\n");
        return (*fsm->state_actions)[OFFSET_ACTION_ENTRY];
    }
    else {
        printf("------- entry action not found\n");
        return NULL;
    }
}

static action_ptr_t get_state_exit_action(fsm_t *fsm, uint8_t *state)
{
    uint8_t action_map = *(state + OFFSET_ACTION_MAP);

    if (action_map & ACTION_MASK_EXIT) {
        printf("----- found exit action\n");
        return (*fsm->state_actions)[OFFSET_ACTION_EXIT];
    }
    else {
        printf("------- exit action not found\n");
        return NULL;
    }
}

static action_ptr_t get_state_tick_action(fsm_t *fsm, uint8_t *state)
{
    uint8_t action_map = *(state + OFFSET_ACTION_MAP);

    if (action_map & ACTION_MASK_TICK) {
        printf("----- found tick action\n");
        return (*fsm->state_actions)[OFFSET_ACTION_TICK];
    }
    else {
        printf("------- tick action not found\n");
        return NULL;
    }
}

static void
fsm_try_entry_action(fsm_t *fsm)
{
    uint8_t *state = fsm->current_state;
    action_ptr_t action = get_state_entry_action(fsm, state);
    if (action != NULL) {
        (*action)(fsm->user_data);
    }
}

static void
fsm_try_exit_action(fsm_t *fsm)
{
    uint8_t *state = fsm->current_state;
    action_ptr_t action = get_state_exit_action(fsm, state);
    if (action != NULL) {
        (*action)(fsm->user_data);
    }
}

static void
fsm_try_tick_action(fsm_t *fsm)
{
    action_ptr_t action = get_state_tick_action(fsm, fsm->current_state);
    if (action != NULL) {
        (*action)(fsm->user_data);
    }
}

static void
fsm_try_transition(fsm_t *fsm)
{
    if (fsm->pending_event_id != 0) {

        int new_state_id = get_target_state_id(fsm);

        fsm->pending_event_id = 0;

        if (new_state_id != 0) {
            /* LOG(LOG_INFO, "FSM #%u: transition %d -> %d", */
            /*        fsm->id, fsm->current_state->id, new_state_id); */
            /* printf("making transition from state %d to %d\n", fsm->current_state_id, new_state_id); */

            fsm_try_exit_action(fsm);
            fsm->current_state_id = new_state_id;
            uint8_t *state = state_descriptor(fsm, new_state_id);
            fsm->current_state = state;
            fsm->state_actions = (action_ptr_t (*)[]) &((*fsm->action_lut)[*(state + OFFSET_ACTIONS)]);
            fsm_try_entry_action(fsm);
        }
    }
}
#endif

/* Public functions
 * ========================================================================== */
// TODO start_state belongs in the xml model
#ifdef SPARSE
fsm_t*
fsm_new(struct fsm_state* transition_lut, struct fsm_state* start_state,
        void *user_data)
{
    assert (transition_lut != NULL);
    assert (start_state != NULL);

    uint32_t new_fsm_idx = nof_fsms_in_use;
    struct fsm_s *new_fsm = &fsm_pool[new_fsm_idx];
    nof_fsms_in_use++;
    assert (nof_fsms_in_use <= NOF_STATEMACHINES);

    new_fsm->id = new_fsm_idx;
    new_fsm->name = "N/A";
    new_fsm->transition_lut = transition_lut;
    new_fsm->current_state = start_state;
    new_fsm->pending_event = 0;
    new_fsm->user_data = user_data;

    LOG(LOG_INFO, "Created state machine #%u", new_fsm_idx);

    return new_fsm;
}

void fsm_tick(fsm_t *fsm)
{
    assert(fsm);

    LOG(LOG_DBG, "FSM:\tid\tstate\tevent\tname\n"
           "\t\t%u\t%d\t%d\t%s",
           fsm->id, fsm->current_state->id, fsm->pending_event, fsm->name);
    fsm_try_tick_action(fsm);
    fsm_try_transition(fsm);
}

int fsm_get_state_id(fsm_t *fsm)
{
	return fsm->current_state->id;
}
#endif

void fsm_send_event(fsm_t *fsm, int event_id)
{
    fsm->pending_event_id = event_id;
}

#ifdef DENSE
fsm_t*
fsm_new(uint8_t (*transition_lut)[], void (*(*action_lut)[])(void*),
         int start_state_id, void *user_data)
{
    assert (transition_lut != NULL);
    assert (action_lut != NULL);
    assert (start_state_id != 0);

    uint32_t new_fsm_idx = nof_fsms_in_use;
    struct fsm_s *fsm = &fsm_pool[new_fsm_idx];
    nof_fsms_in_use++;
    assert (nof_fsms_in_use <= NOF_STATEMACHINES);

    fsm->id = new_fsm_idx;
    fsm->name = "N/A";
    fsm->transition_lut = transition_lut;
    fsm->current_state_id = start_state_id;
    fsm->current_state = state_descriptor(fsm, start_state_id);
    fsm->action_lut = action_lut;
    fsm->pending_event_id = 0;
    fsm->user_data = user_data;

    fsm->state_actions = (action_ptr_t (*)[])
                        &((*action_lut)[*(fsm->current_state + OFFSET_ACTIONS)]);
    LOG(LOG_INFO, "Created state machine #%u", new_fsm_idx);

    return fsm;
}

void fsm_tick(fsm_t *fsm)
{
    assert(fsm);

    /* LOG(LOG_DBG, "FSM:\tid\tstate\tevent\tname\n" */
    /*        "\t\t%u\t%d\t%d\t%s", */
    /*        fsm->id, fsm->current_state, fsm->pending_event, fsm->name); */
    fsm_try_tick_action(fsm);
    fsm_try_transition(fsm);
}

int fsm_get_state_id(fsm_t *fsm)
{
	return fsm->current_state_id;
}

#endif
