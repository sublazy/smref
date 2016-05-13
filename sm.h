#ifndef SM_H
#define SM_H

#include <stdint.h>

typedef struct {
    uint32_t id;
    char * name;
    int state;
    int *transition_table;
    int pending_event;
} sm_t;

void sm_run(sm_t *sm);
void sm_send_event(sm_t *sm, int event);
void sm_print_tx_table(sm_t *sm);

#endif // SM_H
