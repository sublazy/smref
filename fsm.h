#ifndef FSM_H
#define FSM_H

#include <stdint.h>

// Number of state machines in the system (size of the pool)
#define NOF_STATEMACHINES 1

// Max number of transitions out of a single state.
#define NOF_TRANSITIONS_MAX     6

typedef struct fsm_state_s {
    int id;
    void (*on_entry)(void);
    void (*on_tick)(void);
    void (*on_exit)(void);
    int transitions[NOF_TRANSITIONS_MAX];
} fsm_state_t;

typedef struct fsm_s *fsm_t;

fsm_t fsm_new(fsm_state_t* state_tbl, fsm_state_t* start_state);
void fsm_tick(fsm_t fsm);
void fsm_send_event(fsm_t fsm, int event);
int fsm_get_state_id(fsm_t fsm);
struct fsm_state_s* fsm_get_state(fsm_t fsm);

#endif // FSM_H
