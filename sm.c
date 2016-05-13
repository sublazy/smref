#include "sm.h"
#include <stdio.h>
#include <assert.h>

/* Private functions
 * ========================================================================== */

static void
sm_do_transitions(sm_t *sm)
{
    printf("do transitions of sm #%d\n", sm->id);

    if (sm->pending_event != 0) {
        printf("state now: %d\n", sm->state);
        printf("received event #%d\n", sm->pending_event);

        int state_next =
            sm->transition_table[
                sm->state * sm->numof_events + sm->pending_event
            ];

        sm->pending_event = 0;

        if (state_next != 0) {
            printf("requested transition to state #%d\n", state_next);
            sm->state = state_next;
        }
    }
}

static void
sm_run_state(sm_t *sm)
{
    printf("run state of #%d\n", sm->id);
    printf("    current state: %d\n", sm->state);
}


/* Public functions
 * ========================================================================== */

void sm_run(sm_t *sm)
{
    assert(sm);

    printf("running sm #%d (%s)\n", sm->id, sm->name);
    sm_run_state(sm);
    sm_do_transitions(sm);
}

void sm_send_event(sm_t *sm, int event)
{
    sm->pending_event = event;
}

void sm_print_tx_table(sm_t *sm)
{
    int *table = sm->transition_table;

    for (int s = 0; s < 5; s++) {
        printf("state %d:\t", s);
        for (int e = 0; e < 3; e++) {
            printf("  %d\t", table[3*s+e]);
        }
        printf("\n");
    }
}
