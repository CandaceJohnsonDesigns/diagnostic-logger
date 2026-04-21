#ifndef LOGGER_H
    #define LOGGER_H

    #include <stdint.h>
    #include <stddef.h>

    #define LOGGER_MESSAGE_MAX 64U
    #define LOGGER_CAPACITY 16U

    _Static_assert(LOGGER_CAPACITY > 0U, "LOGGER_CAPACITY must be greater than 0");
    _Static_assert((LOGGER_CAPACITY & (LOGGER_CAPACITY - 1U)) == 0U,
                "LOGGER_CAPACITY must be a power of two");


    // Type Definitions

    typedef enum {
        LOG_LEVEL_DEBUG,
        LOG_LEVEL_INFO,
        LOG_LEVEL_WARNING,
        LOG_LEVEL_ERROR
    } LogLevel;

    typedef enum {
        LOGGER_OK = 0,
        LOGGER_ERR_NULL_LOGGER,
        LOGGER_ERR_NULL_MESSAGE,
        LOGGER_ERR_NULL_ENTRY,
        LOGGER_ERR_NULL_COUNT,
        LOGGER_ERR_INVALID_INDEX,
        LOGGER_ERR_INVALID_LEVEL
    } LoggerStatus;

    typedef struct {
        uint32_t timestamp;
        LogLevel level;
        char message[LOGGER_MESSAGE_MAX];
    } LogEntry;

    /*
     * Logger state and storage.
     *
     * The Logger object is caller-owned and may be allocated in static, stack, or other 
     * caller-managed memory. The structure is publicly defined to keep allocation simple 
     * and deterministic for embedded-style use.
     * 
     * The members of this structure are internal implementation state. Callers must not modify 
     * any fields directly after initialization. Use the public logger API for all normal interaction.
     */
    typedef struct {
        LogEntry entries[LOGGER_CAPACITY];
        uint32_t head;
        uint32_t tail;
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

    LoggerStatus loggerGetCount(const Logger *logger);

    LoggerStatus loggerClear(Logger *logger);

#endif // LOGGER_H
