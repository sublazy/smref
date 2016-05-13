/*
 * Traffic Light Controller (TLC) State Machine.
 */

#include "sm_tlc2.h"
#include <unistd.h> // for NULL
#include <stdio.h> // for puts

/* Action declarations
 * ========================================================================== */
static void
    tlc_red_on_entry(void);

static void
    tlc_red_run(void);

static void
    tlc_red_on_exit(void);

/* Transitions
 * ========================================================================== */
static sm_state_t
states_tlc [TLC_NUMOF_STATES] = {
    [TLC_STATE_RED] = {
        .id = TLC_STATE_RED,
        .transitions = {
            [TLC_EVENT_NEXT] = TLC_STATE_YELLOW,
            [TLC_EVENT_RESET] = TLC_STATE_OFF,
        },
        .on_entry = tlc_red_on_entry,
        .on_exit = tlc_red_on_exit,
        .run = tlc_red_run,
    },
    [TLC_STATE_YELLOW] = {
        .id = TLC_STATE_YELLOW,
        .transitions = {
            [TLC_EVENT_NEXT] = TLC_STATE_GREEN,
            [TLC_EVENT_RESET] = TLC_STATE_OFF,
        },
        .on_entry = NULL,
        .on_exit = NULL,
        .run = NULL,
    },
    [TLC_STATE_GREEN] = {
        .id = TLC_STATE_GREEN,
        .transitions = {
            [TLC_EVENT_NEXT] = TLC_STATE_RED,
            [TLC_EVENT_RESET] = TLC_STATE_OFF,
        },
    },
};

/* SM object
 * ========================================================================== */
static sm_t
tlc = {
    .id = 777,
    .name = "the other traffic lights controller",
    .all_states = states_tlc,
    .current_state = &states_tlc[TLC_STATE_YELLOW],
    .pending_event = TLC_EVENT_NULL,
    .numof_states = TLC_NUMOF_STATES,
    .numof_events = TLC_NUMOF_EVENTS,
};

sm_t*
sm_tlc2_get_obj(void)
{
    return &tlc;
}

/* Actions
 * ========================================================================== */
static void
tlc_red_on_entry(void)
{
    puts("Red light says hello!");
}

static void
tlc_red_run(void)
{
    puts("Red light is on.");
}

static void
tlc_red_on_exit(void)
{
    puts("Red light says bye!");
}

