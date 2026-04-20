#ifndef LOGGER_H
    #define LOGGER_H

    #define LOG_MESSAGE_MAX 64

    typedef enum {
        LOG_LEVEL_DEBUG,
        LOG_LEVEL_INFO,
        LOG_LEVEL_WARNING,
        LOG_LEVEL_ERROR
    } LogLevel;

    typedef enum {
        LOGGER_OK = 0,
        LOGGER_ERR_NULL,
        LOGGER_ERR_INVALID_INDEX
    } LoggerStatus;

    typedef struct {
        __UINT32_TYPE__ timestamp;
        LogLevel level;
        char message[LOG_MESSAGE_MAX];
    } LogEntry;
    
#endif // LOGGER_H
