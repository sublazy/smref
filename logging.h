#ifndef LOGGING_H
#define LOGGING_H

#define LOG_NONE     0
#define LOG_FATAL    1
#define LOG_ERR      2
#define LOG_WARN     3
#define LOG_INFO     4
#define LOG_DBG      5

#define LOG(level, str, args...) \
    do {  \
        if (level <= debug_level) { \
            printf("LOG: "str"\r\n", ##args); \
        } \
    } while (0)

#endif // LOGGING_H
