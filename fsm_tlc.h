/* Traffic Lights Controller (TLC) State Machine. */

#ifndef FSM_TLC_H
#define FSM_TLC_H

#include "fsm.h"

typedef enum {
    TLC_STATE_NULL,
    TLC_STATE_OFF,
    TLC_STATE_RED,
    TLC_STATE_YELLOW,
    TLC_STATE_GREEN,
    TLC_NUMOF_STATES
} fsm_tlc_state_t;

typedef enum {
    TLC_EVENT_NULL,
    TLC_EVENT_RESET,
    TLC_EVENT_NEXT,
    TLC_NUMOF_EVENTS
} fsm_tlc_event_t;

fsm_t* fsm_tlc_new(void *user_data);

#endif // FSM_TLC_H
