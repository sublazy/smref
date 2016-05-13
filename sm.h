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

#define DBG_VAL_INT(identifier) \
    printf("value of %s: %d\n", #identifier, (int)identifier)

#define DBG_PRINT_ADDR(identifier) \
    printf("address of %s: %p\n", #identifier, (void*)&identifier)

#define DBG_VAL_PTR(identifier) \
    printf("value of %s: %p\n", #identifier, (void*)identifier)

#endif // SM_H
