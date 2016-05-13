#include <stdio.h>
#include <unistd.h>

int main(void)
{
    printf("state machine test\n");

    while(1) {
        // run a state
        // check events
        // do transitions
        puts(".");
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
