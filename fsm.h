#ifndef SM_H
#define SM_H

#include <stdint.h>

// Number of state machines in the system (size of the pool)
#define NOF_STATEMACHINES 1

// Max number of transitions out of a single state.
#define NOF_TRANSITIONS_MAX     6

typedef struct sm_state_s {
    int id;
    void (*on_entry)(void);
    void (*run)(void);
    void (*on_exit)(void);
    int transitions[NOF_TRANSITIONS_MAX];
} sm_state_t;

typedef struct sm_s *sm_t;

sm_t sm_new(sm_state_t* state_tbl, sm_state_t* start_state);
void sm_run(sm_t sm);
void sm_send_event(sm_t sm, int event);

#endif // SM_H
