/*
 * Traffic Light Controller (TLC) State Machine.
 */

#ifndef SM_TLC_H
#define SM_TLC_H

#include "sm.h"

typedef enum {
    TLC_STATE_NULL,
    TLC_STATE_OFF,
    TLC_STATE_RED,
    TLC_STATE_YELLOW,
    TLC_STATE_GREEN,
    TLC_NUMOF_STATES
} sm_tlc_state_t;

typedef enum {
    TLC_EVENT_NULL,
    TLC_EVENT_NEXT,
    TLC_EVENT_RESET,
    TLC_NUMOF_EVENTS
} sm_tlc_event_t;

// Get the pointer to the state machine object.
// Use it to send events to the SM.
sm_t* sm_tlc_get_obj(void);

#endif // SM_TLC_H
