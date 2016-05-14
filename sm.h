#ifndef SM_H
#define SM_H

#include <stdint.h>

// Max number of transitions out of a single state.
#define NOF_TRANSITIONS_MAX     6

typedef struct {
    int id;
    void (*on_entry)(void);
    void (*run)(void);
    void (*on_exit)(void);
    int transitions[NOF_TRANSITIONS_MAX];
} sm_state_t;

typedef struct {
    uint32_t id;
    char * name;
    sm_state_t *all_states;
    sm_state_t *current_state;
    int pending_event;
    int numof_states;
    int numof_events;
} sm_t;

void sm_run(sm_t *sm);
void sm_send_event(sm_t *sm, int event);
void sm_print_tx_table(sm_t *sm);

#endif // SM_H
