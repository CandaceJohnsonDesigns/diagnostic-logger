#include <stdint.h>
#include <stddef.h>

#ifndef LOGGER_H
    #define LOGGER_H

    #define LOG_MESSAGE_MAX 64
    #define LOG_CAPACITY 16

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
        char message[LOG_MESSAGE_MAX];
    } LogEntry;

    typedef struct {
        LogEntry entries[LOG_CAPACITY];
        size_t head;
        size_t tail;
    } Logger;


    // Function Prototypes

    LoggerStatus loggerInit(Logger *logger);

    LoggerStatus loggerLog(
        Logger *logger, 
        uint32_t timestamp, 
        LogLevel level, 
        const char *message
    );

    LoggerStatus loggerGetEntry(
        const Logger *logger, 
        size_t index, 
        LogEntry *entry
    );

    size_t loggerGetCount(const Logger *logger);

    void loggerClear(Logger *logger);

#endif // LOGGER_H
