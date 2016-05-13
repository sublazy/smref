#include <stdio.h>
#include <unistd.h>
#include "sm.h"

/* SM instance definition
 * ========================================================================== */
/*
 * states:
 *      off, red, yellow, green
 *
 * transitions:
 *      red -> yellow
 *      yellow -> green
 *      green -> red
 *      *any -> off
 *
 * events:
 *      evt_next
 */

typedef enum {
    TLC_STATE_NULL,
    TLC_STATE_OFF,
    TLC_STATE_RED,
    TLC_STATE_YELLOW,
    TLC_STATE_GREEN,
    TLC_NUMOF_STATES
} state_tlc_t;

typedef enum {
    TLC_EVENT_NULL,
    TLC_EVENT_NEXT,
    TLC_EVENT_RESET,
    TLC_NUMOF_EVENTS
} event_tlc_t;

void tlc_red_on_entry(void)
{
    puts("Red light says hello!");
}

void tlc_red_run(void)
{
    puts("Red light is on.");
}

void tlc_red_on_exit(void)
{
    puts("Red light says bye!");
}

sm_state_t states_tlc [TLC_NUMOF_STATES] = {
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

sm_t tlc = {
    .id = 0,
    .name = "traffic lights controller",
    .all_states = states_tlc,
    .current_state = &states_tlc[TLC_STATE_RED],
    .pending_event = TLC_EVENT_NULL,
    .numof_states = TLC_NUMOF_STATES,
    .numof_events = TLC_NUMOF_EVENTS,
};

/* Main application
 * ========================================================================== */

int main(void)
{
    printf("state machine test\n");

    sm_print_tx_table(&tlc);

    while(1) {
        puts(".");
        sm_run(&tlc);
        sleep(1);
        sm_send_event(&tlc, TLC_EVENT_NEXT);
    }

    return 0;
}
