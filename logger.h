#ifndef LOGGER_H
    #define LOGGER_H

    #include <stdint.h>
    #include <stddef.h>

    #define LOGGER_MESSAGE_MAX 64
    #define LOGGER_CAPACITY 16
    

    // Type Definitions

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
        uint32_t timestamp;
        LogLevel level;
        char message[LOGGER_MESSAGE_MAX];
    } LogEntry;

    typedef struct {
        LogEntry entries[LOGGER_CAPACITY];
        size_t head;
        size_t tail;
    } Logger;


    // Function Prototypes

    LoggerStatus loggerInit(Logger *logger);

    LoggerStatus loggerAppend(
        Logger *logger, 
        uint32_t timestamp, 
        LogLevel level, 
        const char *message
    );

    LoggerStatus loggerGetEntry(
        const Logger *logger, 
        size_t index, 
        LogEntry *outEntry
    );

    size_t loggerGetCount(const Logger *logger);

    void loggerClear(Logger *logger);

#endif // LOGGER_H
