/*
 * Traffic Light Controller (TLC2) State Machine.
 */

#ifndef SM_TLC2_H
#define SM_TLC2_H

#include "sm.h"
#include "sm_tlc.h"

// Get the pointer to the state machine object.
// Use it to send events to the SM.
sm_t* sm_tlc2_get_obj(void);

#endif // SM_TLC2_H
