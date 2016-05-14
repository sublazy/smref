#include <stdio.h>
#include <unistd.h>
#include "sm.h"
#include "sm_tlc.h"
#include "sm_tlc2.h"
#include "logging.h"

static int debug_level = LOG_NONE;

int main(void)
{
    LOG(LOG_INFO, "Entering main");
    sm_t *sm_tlc = sm_tlc_get_obj();
    sm_t *sm_tlc2 = sm_tlc2_get_obj();
    sm_print_tx_table(sm_tlc);
    int cnt = 0;

    while(1) {
        LOG(LOG_DBG, "--- Loop cycle %d", cnt);
        sm_run(sm_tlc);
        sm_run(sm_tlc2);
        sleep(1);
        sm_send_event(sm_tlc, TLC_EVENT_NEXT);
        sm_send_event(sm_tlc2, TLC_EVENT_NEXT);
        cnt++;
    }

    return 0;
}
