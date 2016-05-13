#include <stdio.h>
#include <unistd.h>
#include "sm.h"

typedef enum {
    TLC_STATE_RED,
    TLC_STATE_YELLOW,
    TLC_STATE_GREEN,
    TLC_NUMOF_STATES
} states_tlc;

sm_t tlc = {
    .id = 0,
    .name = "traffic lights controller",
    .state = TLC_STATE_RED,
};

int main(void)
{
    printf("state machine test\n");

    while(1) {
        puts(".");
        sm_run(&tlc);
        sleep(1);
    }

    return 0;
}


/*
 * states:
 *      red, yellow, green
 *
 * transitions:
 *      red -> yellow
 *      yellow -> green
 *      green -> red
 *
 * events:
 *      evt_next
 */
