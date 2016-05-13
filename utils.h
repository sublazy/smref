#ifndef UTILS_H
#define UTILS_H

#define DBG_VAL_INT(identifier) \
    printf("value of %s: %d\n", #identifier, (int)identifier)

#define DBG_PRINT_ADDR(identifier) \
    printf("address of %s: %p\n", #identifier, (void*)&identifier)

#define DBG_VAL_PTR(identifier) \
    printf("value of %s: %p\n", #identifier, (void*)identifier)


#endif // UTILS_H
