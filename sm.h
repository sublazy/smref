#ifndef SM_H
#define SM_H

#include <stdint.h>

typedef struct {
    uint32_t id;
    char * name;
    uint32_t state;
} sm_t;

void sm_run(sm_t *sm);

#endif // SM_H
