/*
 * A State Machine modelling a car on the road crossing.
 */

#ifndef SM_CAR
#define SM_CAR

#include "sm.h"

typedef enum {
    CAR_STATE_NULL,
    CAR_STATE_GONE,
    CAR_STATE_APPROACHING,
    CAR_STATE_WAIT,
    CAR_STATE_GO,
    CAR_NUMOF_STATES
} sm_car_state_t;

typedef enum {
    CAR_EVENT_NULL,
    CAR_EVENT_SPAWN,
    CAR_EVENT_LIGHT_RED,
    CAR_EVENT_LIGHT_YELLOW,
    CAR_EVENT_LIGHT_GREEN,
    CAR_EVENT_CLEARED,
    CAR_NUMOF_EVENTS
} sm_car_event_t;

// Get the pointer to the state machine object.
// Use it to send events to the SM.
sm_t* sm_car_get_obj(void);

#endif // SM_CAR_H
