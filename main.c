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
} states_tlc;

typedef enum {
    TLC_EVENT_NULL,
    TLC_EVENT_NEXT,
    TLC_EVENT_RESET,
    TLC_NUMOF_EVENTS
} events_tlc;

int transitions_tlc [TLC_NUMOF_STATES] [TLC_NUMOF_EVENTS] = {
    [TLC_STATE_RED] = {
        [TLC_EVENT_NEXT] = TLC_STATE_YELLOW,
        [TLC_EVENT_RESET] = TLC_STATE_OFF,
    },
    [TLC_STATE_YELLOW] = {
        [TLC_EVENT_NEXT] = TLC_STATE_GREEN,
        [TLC_EVENT_RESET] = TLC_STATE_OFF,
    },
    [TLC_STATE_GREEN]= {
        [TLC_EVENT_NEXT] = TLC_STATE_RED,
        [TLC_EVENT_RESET] = TLC_STATE_OFF,
    },
};

sm_t tlc = {
    .id = 0,
    .name = "traffic lights controller",
    .transition_table = transitions_tlc[0],
    .state = TLC_STATE_RED,
    .pending_event = TLC_EVENT_NULL,
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
