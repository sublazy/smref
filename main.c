#include <stdio.h>
#include <unistd.h>
#include "sm.h"
#include "sm_tlc.h"
#include "sm_tlc2.h"

int main(void)
{
    printf("state machine test\n");

    sm_t *sm_tlc = sm_tlc_get_obj();
    sm_t *sm_tlc2 = sm_tlc2_get_obj();
    sm_print_tx_table(sm_tlc);

    while(1) {
        puts(".");
        sm_run(sm_tlc);
        sm_run(sm_tlc2);
        sleep(1);
        sm_send_event(sm_tlc, TLC_EVENT_NEXT);
        sm_send_event(sm_tlc2, TLC_EVENT_NEXT);
    }

    return 0;
}
