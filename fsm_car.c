/*
 * A State Machine modelling a car on the road crossing.
 */

#include "fsm_car.h"
#include <unistd.h> // for NULL
#include <stdio.h> // for puts

/* Action declarations
 * ========================================================================== */
static void
    car_approaching_on_entry(void);

static void
    watch_the_lights(void);

/* Transitions
 * ========================================================================== */
static fsm_state_t
states_car [CAR_NUMOF_STATES] = {
    [CAR_STATE_GONE] = {
        .id = CAR_STATE_GONE,
        .transitions = {
            [CAR_EVENT_SPAWN] = CAR_STATE_APPROACHING,
        },
        .on_entry = NULL,
        .on_exit = NULL,
        .run = NULL,
    },
    [CAR_STATE_APPROACHING] = {
        .id = CAR_STATE_APPROACHING,
        .transitions = {
            [CAR_EVENT_LIGHT_GREEN] = CAR_STATE_GO,
            [CAR_EVENT_LIGHT_YELLOW] = CAR_STATE_WAIT,
            [CAR_EVENT_LIGHT_RED] = CAR_STATE_WAIT,
        },
        .on_entry = car_approaching_on_entry,
        .on_exit = NULL,
        .run = watch_the_lights,
    },
};

/* FSM object
 * ========================================================================== */
static fsm_t
car = {
    .id = 3,
    .name = "car on the crossing",
    .all_states = states_car,
    .current_state = &states_car[CAR_STATE_GONE],
    .pending_event = CAR_EVENT_NULL,
    .numof_states = CAR_NUMOF_STATES,
    .numof_events = CAR_NUMOF_EVENTS,
};

fsm_t*
fsm_car_get_obj(void)
{
    return &car;
}

/* Actions
 * ========================================================================== */
static void
car_approaching_on_entry(void)
{
    puts("The car is approaching.");
}

static void
watch_the_lights(void)
{
    // todo
    return;
}

