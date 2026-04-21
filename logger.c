#include "logger.h"

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
