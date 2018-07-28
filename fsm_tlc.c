/* Traffic Lights Controller (TLC) State Machine. */

#include "sm_tlc.h"
#include "sm_tlc_actions.h"

/* Transitions
 * -------------------------------------------------------------------------- */
static sm_state_t
tlc_tx_table [TLC_NUMOF_STATES] = {
    [TLC_STATE_OFF] = {
        .id = TLC_STATE_OFF,
        .transitions = {
            [TLC_EVENT_NEXT] = TLC_STATE_RED,
        },
        .on_entry = tlc_off_on_entry,
        .on_exit  = tlc_off_on_exit,
        .run      = tlc_off_run,
    },
    [TLC_STATE_RED] = {
        .id = TLC_STATE_RED,
        .transitions = {
            [TLC_EVENT_NEXT] = TLC_STATE_YELLOW,
            [TLC_EVENT_RESET] = TLC_STATE_OFF,
        },
        .on_entry = tlc_red_on_entry,
        .on_exit  = tlc_red_on_exit,
        .run      = tlc_red_run,
    },
    [TLC_STATE_YELLOW] = {
        .id = TLC_STATE_YELLOW,
        .transitions = {
            [TLC_EVENT_NEXT] = TLC_STATE_GREEN,
            [TLC_EVENT_RESET] = TLC_STATE_OFF,
        },
        .on_entry = tlc_yellow_on_entry,
        .on_exit  = tlc_yellow_on_exit,
        .run      = tlc_yellow_run,
    },
    [TLC_STATE_GREEN] = {
        .id = TLC_STATE_GREEN,
        .transitions = {
            [TLC_EVENT_NEXT] = TLC_STATE_RED,
            [TLC_EVENT_RESET] = TLC_STATE_OFF,
        },
        .on_entry = tlc_green_on_entry,
        .on_exit  = tlc_green_on_exit,
        .run      = tlc_green_run,
    },
};

/* Public functions
 * -------------------------------------------------------------------------- */
sm_t sm_tlc_new(void)
{
    return sm_new(tlc_tx_table, &tlc_tx_table[TLC_STATE_OFF]);
}

