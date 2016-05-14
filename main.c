#include <stdio.h>
#include <unistd.h>
#include "sm.h"
#include "sm_tlc.h"
#include "sm_car.h"
#include "logging.h"

static int debug_level = LOG_NONE;

int main(void)
{
    sm_t *sm_tlc = sm_tlc_get_obj();
    sm_t *sm_car = sm_car_get_obj();

    int cnt = 0;
    while(1) {
        LOG(LOG_DBG, "--- Loop cycle %d", cnt);
        sm_run(sm_tlc);
        sm_run(sm_car);
        sleep(1);
        sm_send_event(sm_tlc, TLC_EVENT_NEXT);
        cnt++;
    }

    return 0;
}
