#include <stdio.h>
#include <unistd.h>
#include "sm.h"
#include "sm_tlc.h"

int main(void)
{
    printf("state machine test\n");

    sm_t *sm_tlc = sm_tlc_get_obj();
    sm_print_tx_table(sm_tlc);

    while(1) {
        puts(".");
        sm_run(sm_tlc);
        sleep(1);
        sm_send_event(sm_tlc, TLC_EVENT_NEXT);
    }

    return 0;
}
