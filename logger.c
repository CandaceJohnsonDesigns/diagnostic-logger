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
