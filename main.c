#include <stdio.h>
#include <unistd.h>
#include "fsm.h"
#include "fsm_tlc.h"
#include "fsm_car.h"
#include "logging.h"

static int debug_level = LOG_NONE;

int main(void)
{
    fsm_t fsm_tlc = fsm_tlc_new();
    //fsm_t *fsm_car = fsm_car_get_obj();

    int cnt = 0;
    while(1) {
        LOG(LOG_DBG, "--- Loop cycle %d", cnt);
        fsm_tick(fsm_tlc);
        //fsm_tick(fsm_car);
        sleep(1);
        fsm_send_event(fsm_tlc, TLC_EVENT_NEXT);
        cnt++;
    }

    return 0;
}
