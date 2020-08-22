#ifndef FSM_H
#define FSM_H

#include <stdint.h>

// Number of state machines in the system (size of the pool)
#define NOF_STATEMACHINES 1

// Max number of transitions out of a single state.
#define NOF_TRANSITIONS_MAX     6

struct fsm_state {
    int id;
    void (*on_entry)(void*);
    void (*on_tick)(void*);
    void (*on_exit)(void*);
    int transitions[NOF_TRANSITIONS_MAX];
};

typedef struct fsm_s fsm_t;

fsm_t* fsm_new(struct fsm_state *transition_lut, struct fsm_state *start_state, void *user_data);
void fsm_tick(fsm_t *fsm);
void fsm_send_event(fsm_t *fsm, int event);
int fsm_get_state_id(fsm_t *fsm);
struct fsm_state* fsm_get_state(fsm_t *fsm);

#endif // FSM_H
