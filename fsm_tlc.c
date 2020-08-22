/* Traffic Lights Controller (TLC) State Machine. */

#include "fsm_tlc.h"
#include "fsm_tlc_actions.h"

/* Transitions
 * -------------------------------------------------------------------------- */
static struct fsm_state
tlc_tx_table [TLC_NUMOF_STATES] = {
    [TLC_STATE_OFF] = {
        .id = TLC_STATE_OFF,
        .transitions = {
            [TLC_EVENT_NEXT] = TLC_STATE_RED,
        },
        .on_entry = tlc_off_on_entry,
        .on_exit  = tlc_off_on_exit,
        .on_tick  = tlc_off_on_tick,
    },
    [TLC_STATE_RED] = {
        .id = TLC_STATE_RED,
        .transitions = {
            [TLC_EVENT_NEXT] = TLC_STATE_YELLOW,
            [TLC_EVENT_RESET] = TLC_STATE_OFF,
        },
        .on_entry = tlc_red_on_entry,
        .on_exit  = tlc_red_on_exit,
        .on_tick  = tlc_red_on_tick,
    },
    [TLC_STATE_YELLOW] = {
        .id = TLC_STATE_YELLOW,
        .transitions = {
            [TLC_EVENT_NEXT] = TLC_STATE_GREEN,
            [TLC_EVENT_RESET] = TLC_STATE_OFF,
        },
        .on_entry = tlc_yellow_on_entry,
        .on_exit  = tlc_yellow_on_exit,
        .on_tick  = tlc_yellow_on_tick,
    },
    [TLC_STATE_GREEN] = {
        .id = TLC_STATE_GREEN,
        .transitions = {
            [TLC_EVENT_NEXT] = TLC_STATE_RED,
            [TLC_EVENT_RESET] = TLC_STATE_OFF,
        },
        .on_entry = tlc_green_on_entry,
        .on_exit  = tlc_green_on_exit,
        .on_tick  = tlc_green_on_tick,
    },
};

/* Public functions
 * -------------------------------------------------------------------------- */
fsm_t* fsm_tlc_new(void *user_data)
{
    return fsm_new(tlc_tx_table, &tlc_tx_table[TLC_STATE_OFF], user_data);
}

