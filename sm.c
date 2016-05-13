#include "sm.h"
#include <stdio.h>

/* Private functions
 * ========================================================================== */

static void
sm_check_events(sm_t *sm)
{
    printf("check events of #%d\n", sm->id);
}

static void
sm_do_transitions(sm_t *sm)
{
    printf("do transitions of sm #%d\n", sm->id);
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
    printf("running sm #%d (%s)\n", sm->id, sm->name);
    sm_run_state(sm);
    sm_check_events(sm);
    sm_do_transitions(sm);
}
