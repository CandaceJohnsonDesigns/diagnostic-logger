#include "logger.h"
#include <string.h>
#include <stdbool.h> 
#include <stdio.h>

/*
* Initialize the logger
* - Set head and tail indices to 0
* - Return LOGGER_OK on success, or 
*   LOGGER_ERR_NULL_LOGGER if the logger pointer is NULL
*
* IMPORTANT: Callers must not modify `head`, `tail`, or entry storage 
*   directly after initialization. All normal interaction with the logger
*   must occur through the public API.
*/
LoggerStatus loggerInit(Logger *logger) {
    LoggerStatus status;

    if (logger == NULL) {
        status = LOGGER_ERR_NULL_LOGGER;
    } else {
        logger->head = 0U;
        logger->tail = 0U;

        status = LOGGER_OK;
    }

    return status;
}

/*
* Get the count of entries in the logger
* - Set the count pointer to the number of entries in the logger
* - Return LOGGER_OK on success, or 
*   LOGGER_ERR_NULL_LOGGER if the logger pointer is NULL
*   LOGGER_ERR_NULL_COUNT if the count pointer is NULL
*/
LoggerStatus loggerGetCount(const Logger *logger, size_t *outCount) {
    LoggerStatus status;
    
    if (logger == NULL) {
        status = LOGGER_ERR_NULL_LOGGER;
    } else if (outCount == NULL) {
        status = LOGGER_ERR_NULL_COUNT;
    } else {
        // Calculate count based on head and tail indices
        *outCount = logger->head - logger->tail;

        status = LOGGER_OK;
    }

    return status;
}

/*
* Clear the logger
* - Reset head and tail indices to 0
* - Return LOGGER_OK on success, or 
*   LOGGER_ERR_NULL_LOGGER if the logger pointer is NULL
*/
LoggerStatus loggerClear(Logger *logger) {
    LoggerStatus status;    

    if (logger == NULL) {
        status = LOGGER_ERR_NULL_LOGGER;
    } else {
        logger->head = 0U;
        logger->tail = 0U;

        status = LOGGER_OK;
    }

    return status;
}

/*
* Check if the logger is full
* - Return true if the logger is full, false otherwise
* - A logger is considered full if the number of entries (head - tail)
*   is greater than or equal to the capacity of the logger. 
* - If the logger pointer is NULL, return false (not full)
*
* Note: While the logger is full when (head - tail == LOGGER_CAPACITY), 
*   the condition is written as >= to handle any potential edge cases where
*   head and tail might not be perfectly managed.
*/
bool loggerIsFull(const Logger *logger) {
    bool isFull = false;

    if (logger != NULL) {
        isFull = (logger->head - logger->tail) >= LOGGER_CAPACITY;
    }

    return isFull;
}

/*
* Append a message to the logger
* - Add the message to the ring buffer
* - Update the head index
* - Return LOGGER_OK on success, or 
*   LOGGER_ERR_NULL_LOGGER if the logger pointer is NULL
*   LOGGER_ERR_NULL_MESSAGE if the message pointer is NULL
*/
LoggerStatus loggerAppend(
    Logger *logger, 
    uint32_t timestamp, 
    LogLevel level, 
    const char *message
) {
    LoggerStatus status;

    if (logger == NULL) {
        status = LOGGER_ERR_NULL_LOGGER;
    } else if (message == NULL) {
        status = LOGGER_ERR_NULL_MESSAGE;
    } else {

        LogEntry entry;
        entry.timestamp = timestamp;
        entry.level = level;
        
        // Truncate message and ensure null-termination
        strncpy(entry.message, message, LOGGER_MESSAGE_MAX - 1);
        entry.message[LOGGER_MESSAGE_MAX - 1] = '\0'; 
        
        // Check if the logger is full (head - tail == capacity)
        if ( loggerIsFull(logger) ) {
            // If full, advance the tail to overwrite the oldest entry
            logger->tail++;
        }

        // Append the entry to the logger's ring buffer
        logger->entries[logger->head & (LOGGER_CAPACITY - 1)] = entry;

        // Advance the head index to point to the next write position
        logger->head++;

        status = LOGGER_OK;
    }

    return status;
}
