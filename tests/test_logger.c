/*
 * test_logger.c
 *
 * Minimal test harness for the diagnostic logger implementation. 
 * No frameworks are used to keep it simple and lightweight.
 */

#include <stdio.h>
#include <string.h>
#include "../logger.h"

/* 
* Track test results globally 
*/
static int testsRun = 0;
static int testsFailed = 0;

/******************************************************************************
*               Test macros for assertions and reporting                      *
*******************************************************************************/

/* 
* Basic assertion
* If condition fails:
* - Print location and condition that failed
* - Increment test failure count
* - Return from the test function to prevent further execution
*/
#define ASSERT_TRUE(condition)                                                \
    do {                                                                      \
        if (!(condition)) {                                                   \
            printf(                                                           \
                "Assertion failed: %s, in file %s, line %d\n",                \
                #condition, __FILE__, __LINE__);                              \
            testsFailed++;                                                    \
            return;                                                           \
        }                                                                     \
    } while(0)

/*
* Equality assertion for integers / enums
* If equality fails:
* - Print location and expected vs actual values
* - Increment test failure count
* - Return from the test function to prevent further execution
*/
#define ASSERT_EQUAL(expected, actual)                                  \
    do {                                                                \
        if ((expected) != (actual)) {                                   \
            printf("Equality failed: expected %lld, got %lld,           \
                in file %s, line %d\n",                                 \
                (long long)(expected), (long long)(actual),             \
                __FILE__, __LINE__);                                    \
            testsFailed++;                                              \
            return;                                                     \
        }                                                               \
    } while(0)

    /*
    * Test runner macro
    * - Prints the name of the test being run
    * - Executes the test function
    * - Increments total tests run count
    */
#define RUN_TEST(fn)                                                    \
    do {                                                                \
        printf("Running test: %s\n", #fn);                              \
        testsRun++;                                                     \
        fn();                                                           \
    } while(0)                                                          \


/******************************************************************************
*                 MC/DC Tests (Logic Coverage)                                *
*******************************************************************************/

/* ---------------------------------------------------------------------------*
*  MC/DC Analysis: loggerInit                                                 *
*  Decision: (logger == NULL)                                                 *
*                                                                             *
*  1. [Baseline]:         test_loggerInit_valid_returns_ok          (F)       *
*  2. [Pair A] (logger):  test_loggerInit_null_logger_returns_error (T) vs #1 *
* --------------------------------------------------------------------------- */

/*
* 1. loggerInit [MC/DC Baseline] (False Case)
*
* Tests that loggerInit:
* - returns LOGGER_OK status
* - initializes the Logger struct to an empty state (head and tail set to 0)
*/
static void test_loggerInit_valid_returns_ok(void) {
    // Arrange:
    Logger logger;

    // Act: (logger == NULL) is FALSE
    LoggerStatus status = loggerInit(&logger);

    // Assert
    ASSERT_EQUAL(LOGGER_OK, status);  // Should return LOGGER_OK status
}

/*
* 2. loggerInit [MC/DC Independence Pair A] (True Case)
*
* Tests that loggerInit: 
* - returns a LOGGER_ERR_NULL_LOGGER error when passed a NULL Logger pointer
*/
static void test_loggerInit_null_logger_returns_error(void) {
    // Act: (logger == NULL) is TRUE
    LoggerStatus status = loggerInit(NULL);

    // Assert: Should return LOGGER_ERR_NULL_LOGGER error
    ASSERT_EQUAL(LOGGER_ERR_NULL_LOGGER, status); 
}

/* ---------------------------------------------------------------------------*
*  MC/DC Analysis: loggerClear                                                *
*  Decision: (logger == NULL)                                                 *
*                                                                             *
*  1. [Baseline]:        test_loggerClear_valid_returns_ok          (F)       *
*  2. [Pair A] (logger): test_loggerClear_null_logger_returns_error (T) vs #1 *
* --------------------------------------------------------------------------- */

/*
* 1. loggerClear [MC/DC Baseline] (False Case)
*
* Tests that loggerClear:
* - returns LOGGER_OK status
* - clears the Logger struct (sets head and tail to 0)
*/
static void test_loggerClear_valid_returns_ok(void) {
    // Arrange: Initialize logger to a non-empty state 
    // to ensure that clear has an effect
    Logger logger = { .head = 5U, .tail = 3U }; 

    // Act: (logger == NULL) is FALSE
    LoggerStatus status = loggerClear(&logger);

    // Assert: Should return LOGGER_OK status
    ASSERT_EQUAL(LOGGER_OK, status);

    // Side-Effect Check: Ensure that head and tail are reset to 0
    ASSERT_EQUAL(0U, logger.head);
    ASSERT_EQUAL(0U, logger.tail);
}

/*
* 2. loggerClear [MC/DC Independence Pair A] (True Case)
*
* Tests that loggerClear: 
* - returns a LOGGER_ERR_NULL_LOGGER error when passed a NULL Logger pointer
*/
static void test_loggerClear_null_logger_returns_error(void) {
    // Act: (logger == NULL) is TRUE
    LoggerStatus status = loggerClear(NULL);

    // Assert: Should return LOGGER_ERR_NULL_LOGGER error
    ASSERT_EQUAL(LOGGER_ERR_NULL_LOGGER, status); 
}

/* ---------------------------------------------------------------------------*
*  MC/DC Analysis: loggerGetCount                                             *
*  Decision: (logger == NULL) || (outCount == NULL)                           *
*                                                                             *
* 1. [Baseline/Functional]:                                                   *
*       test_loggerGetCount_valid_returns_ok            (F, F)                *
* 2. [Pair A] (logger):                                                       *
*       test_loggerGetCount_null_logger_returns_error   (T, F) vs #1          *
* 3. [Pair B] (outCount):                                                     *
*       test_loggerGetCount_null_count_returns_error    (F, T) vs #1          *
* --------------------------------------------------------------------------- */

/*
* 1. loggerGetCount [MC/DC Baseline] (False, False Case)
*
* Test that loggerGetCount:
* - returns LOGGER_OK status
* - sets the count pointer to the number of entries 
*   in the logger (calculated as head - tail)
*/
static void test_loggerGetCount_valid_returns_ok(void) {
    // Arrange: Initialize with known state;
    /* Head and tail values are not reflective of 
       capacity and ring buffer constraints at this point */
    size_t count = 0;
    uint32_t head = 7U;
    uint32_t tail = 2U;
    Logger logger = { .head = head, .tail = tail };

    // Act: (logger == NULL) is FALSE and (outCount == NULL) is FALSE
    LoggerStatus status = loggerGetCount(&logger, &count);

    // Assert
    ASSERT_EQUAL(LOGGER_OK, status); // Should return LOGGER_OK status
    ASSERT_EQUAL((size_t)(head - tail), count); // Count should be head - tail
}

/*
* 2. loggerGetCount [MC/DC Independence Pair A] (True, False Case)
*
* Test that loggerGetCount:
* - returns LOGGER_ERR_NULL_LOGGER error when passed a NULL Logger pointer
* - does not modify the count pointer value on error 
*   (i.e., it should remain unchanged)
*/
static void test_loggerGetCount_null_logger_returns_error(void) {
    // Arrange: Initialize count to a known value to check for side effects
    size_t count = 123U;

    // Act: (logger == NULL) is TRUE and (outCount == NULL) is FALSE
    LoggerStatus status = loggerGetCount(NULL, &count);

    // Assert: Should return LOGGER_ERR_NULL_LOGGER error
    ASSERT_EQUAL(LOGGER_ERR_NULL_LOGGER, status);

    // Side-Effect Check
    ASSERT_EQUAL(123U, count); // Ensure count is unchanged on error
}

/*
* 3. loggerGetCount [MC/DC Independence Pair B] (True, False Case)
*
* Test that loggerGetCount:
* - returns LOGGER_ERR_NULL_COUNT error when passed a NULL count pointer
*/
static void test_loggerGetCount_null_count_returns_error(void) {
    // Arrange
    // Initialize logger to a non-empty state to ensure no side effects
    Logger logger = { .head = 5, .tail = 2 };

    // Act: (logger == NULL) is FALSE and (outCount == NULL) is TRUE
    LoggerStatus countStatus = loggerGetCount(&logger, NULL);

    // Assert: Should return LOGGER_ERR_NULL_COUNT error
    ASSERT_EQUAL(LOGGER_ERR_NULL_COUNT, countStatus); 
}

/* ---------------------------------------------------------------------------*
* MC/DC Analysis: loggerAppend                                                *
* Decision: (logger == NULL) || (message == NULL) || (level is invalid)       *
*                                                                             *
* 1. [Baseline/Functional]:                                                   *
*       test_loggerAppend_valid_returns_ok             (F, F, F)              *
* 2. [Pair A] (logger):                                                       *
*       test_loggerAppend_null_logger_returns_error    (T, F, F) vs #1        *
* 3. [Pair B] (message):                                                      *
*       test_loggerAppend_null_message_returns_error   (F, T, F) vs #1        *
* 4. [Pair C] (level):                                 (F, F, T) vs #1        *
*    a. test_loggerAppend_level_too_low_returns_error    (level < 0)          *
*    b. test_loggerAppend_level_too_high_returns_error   (level >= COUNT)     *
* --------------------------------------------------------------------------- */

/*
* 1. loggerAppend [MC/DC Baseline] (False, False, False Case)
*
* Tests that loggerAppend:
* - returns LOGGER_OK status
* - appends the log entry to the logger's ring buffer
* - updates the head index correctly
*/
static void test_loggerAppend_valid_returns_ok(void) {
    // Arrange: Initialize logger and set up valid parameters
    Logger logger = {0}; // Start with an empty logger
    LogLevel level = LOG_LEVEL_WARNING;
    uint32_t timestamp = 1234567890U;
    const char *message = "Valid log entry";

    // Act: (logger == NULL) is FALSE, (message == NULL) is FALSE, 
    // and level is invalid
    LoggerStatus status = 
        loggerAppend(&logger, timestamp, level, message);

    // Assert: Should return LOGGER_OK status
    ASSERT_EQUAL(LOGGER_OK, status);

    // Side-Effect Checks: 

    // Verify that the entry was appended correctly (count should be 1)
    size_t count;
    loggerGetCount(&logger, &count);
    ASSERT_EQUAL(1U, count);

    // Verify that the log entry was stored correctly
    LogEntry retrievedEntry;
    loggerGetEntry(&logger, 0U, &retrievedEntry);
    ASSERT_EQUAL(timestamp, retrievedEntry.timestamp);
    ASSERT_EQUAL(level, retrievedEntry.level); 
    ASSERT_TRUE(strcmp(message, retrievedEntry.message) == 0);
}

/*
* 2. loggerAppend [MC/DC Independence Pair A] (True, False, False Case)
*
* Tests that loggerAppend: 
* - returns a LOGGER_ERR_NULL_LOGGER error when passed a NULL Logger pointer
*/
static void test_loggerAppend_null_logger_returns_error(void) {
    // Arrange: Set up valid parameters for loggerAppend
    LogLevel level = LOG_LEVEL_INFO;
    uint32_t timestamp = 1234567890U;
    const char *message = "Test message";

    // Act: Call loggerAppend with a NULL Logger pointer
    LoggerStatus status = 
        loggerAppend(NULL, timestamp, level, message);

    // Assert: Should return LOGGER_ERR_NULL_LOGGER error
    ASSERT_EQUAL(LOGGER_ERR_NULL_LOGGER, status);

    // Side-Effect Check: Ensure that input parameters are unchanged on error
    ASSERT_EQUAL(LOG_LEVEL_INFO, level);
    ASSERT_EQUAL(1234567890U, timestamp); 
    ASSERT_TRUE(strcmp(message, "Test message") == 0); 
}

/*
* 3. loggerAppend [MC/DC Independence Pair B] (False, True, False Case)
*
* Tests that loggerAppend:
* - returns a LOGGER_ERR_NULL_MESSAGE error when passed a NULL message pointer
*/
static void test_loggerAppend_null_message_returns_error(void) {
    // Arrange: Initialize logger and set up valid parameters except message
    Logger logger = {0}; // Start with an empty logger
    LogLevel level = LOG_LEVEL_ERROR;
    uint32_t timestamp = 1234567890U;
    const char *message = NULL; // Set message to NULL to trigger the error

    // Act: Call loggerAppend with a NULL message pointer
    LoggerStatus status = 
        loggerAppend(&logger, timestamp, level, message);

    // Assert: Should return LOGGER_ERR_NULL_MESSAGE error
    ASSERT_EQUAL(LOGGER_ERR_NULL_MESSAGE, status);

    // Side-Effect Check: Ensure that input parameters are unchanged on error
    ASSERT_EQUAL(LOG_LEVEL_ERROR, level);
    ASSERT_EQUAL(1234567890U, timestamp);
    ASSERT_TRUE(message == NULL);
}

/*
* 4a. loggerAppend [MC/DC Independence Pair C-1] (False, False, True Case)
*
* Tests that loggerAppend:
* - returns a LOGGER_ERR_INVALID_LEVEL error when passed an invalid log level
*
* Independence: logger is valid (F), message is valid (F), level is invalid (T)
*/
static void test_loggerAppend_level_too_low_returns_error(void) {
    // Arrange: Initialize logger and set up valid parameters except level
    Logger logger = {0}; // Start with an empty logger
    uint32_t timestamp = 1234567890U;
    const char *message = "Test message";
    LogLevel invalidLevel = -3; // Set to an invalid level that is too low

    // Act: Call loggerAppend with an invalid log level
    LoggerStatus status = 
        loggerAppend(&logger, timestamp, invalidLevel, message);
    
    // Assert: Should return LOGGER_ERR_INVALID_LEVEL error
    ASSERT_EQUAL(LOGGER_ERR_INVALID_LEVEL, status);

    // Side-Effect Check: Ensure that input parameters are unchanged on error
    ASSERT_EQUAL(1234567890U, timestamp);
    ASSERT_TRUE(strcmp(message, "Test message") == 0);
    ASSERT_EQUAL(-3, (int)invalidLevel);
}

/*
* 4b. loggerAppend [MC/DC Independence Pair C-2] (False, False, True Case)
*
* Tests that loggerAppend:
* - returns a LOGGER_ERR_INVALID_LEVEL error when passed an invalid log level
*
* Independence: logger is valid (F), message is valid (F), level is invalid (T)
*/
static void test_loggerAppend_level_too_high_returns_error(void) {
    // Arrange: Initialize logger and set up valid parameters except level
    Logger logger = {0}; // Start with an empty logger
    uint32_t timestamp = 1234567890U;
    const char *message = "Test message";
    // Set to an invalid level that is too high
    LogLevel invalidLevel = LOG_LEVEL_COUNT;

    // Act: Call loggerAppend with an invalid log level
    LoggerStatus status = 
        loggerAppend(&logger, timestamp, invalidLevel, message);
    
    // Assert: Should return LOGGER_ERR_INVALID_LEVEL error
    ASSERT_EQUAL(LOGGER_ERR_INVALID_LEVEL, status);

    // Side-Effect Check: Ensure that input parameters are unchanged on error
    ASSERT_EQUAL(1234567890U, timestamp);
    ASSERT_TRUE(strcmp(message, "Test message") == 0);
    ASSERT_EQUAL(LOG_LEVEL_COUNT, (int)invalidLevel);
}

/* ---------------------------------------------------------------------------*
* MC/DC Analysis: loggerGetEntry                                              *
* Decision: (logger is NULL || indices are invalid || outEntry is NULL)       *
*                                                                             *
* 1. [Baseline]:                                                              *
*       test_loggerGetEntry_valid_returns_ok              (F, F, F)           *
* 2. [Pair A] (logger):                                                       *
*       test_loggerGetEntry_null_logger_returns_error     (T, F, F) vs #1     *
* 3. [Pair B] (indices):                                  (F, T, F) vs #1     *
*    a. test_loggerGetEntry_index_too_old_returns_error   (index < tail)      *
*    b. test_loggerGetEntry_index_too_new_returns_error   (index >= head)     *
* 4. [Pair C] (outEntry):                                                     *
*       test_loggerGetEntry_null_outEntry_returns_error   (F, F, T) vs #1     *
* --------------------------------------------------------------------------- */

/*
* 1. loggerGetEntry [MC/DC Baseline] (False, False, False Case)
*
* Tests that loggerGetEntry:
* - returns LOGGER_OK status
* - retrieves the correct log entry for a valid index
*/
static void test_loggerGetEntry_valid_returns_ok(void) {
    // Arrange: Initialize logger and add a known entry
    Logger logger = {0}; // Start with an empty logger
    uint32_t timestamp = 1234567890U;
    LogLevel level = LOG_LEVEL_DEBUG;
    const char *message = "Test message";
    loggerAppend(&logger, timestamp, level, message);

    // Act: Call loggerGetEntry with a valid index (0)
    LogEntry retrievedEntry;
    LoggerStatus status = loggerGetEntry(&logger, 0U, &retrievedEntry);

    // Assert: Should return LOGGER_OK status
    ASSERT_EQUAL(LOGGER_OK, status);

    // Verify that the retrieved entry matches the one that was appended
    ASSERT_EQUAL(timestamp, retrievedEntry.timestamp);
    ASSERT_EQUAL(level, retrievedEntry.level);
    ASSERT_TRUE(strcmp(message, retrievedEntry.message) == 0);
}

/*
* 2. loggerGetEntry [MC/DC Independence Pair A] (True, False, False Case)
*
* Tests that loggerGetEntry: 
* - returns a LOGGER_ERR_NULL_LOGGER error when passed a NULL Logger pointer
*/
static void test_loggerGetEntry_null_logger_returns_error(void) {
    // Arrange: Set up valid parameters for loggerGetEntry except logger
    size_t index = 0U;
    LogEntry outEntry;
    Logger *nullLogger = NULL;

    // Act: Call loggerGetEntry with a NULL logger pointer
    LoggerStatus status = loggerGetEntry(nullLogger, index, &outEntry);

    // Assert: Should return LOGGER_ERR_NULL_LOGGER error
    ASSERT_EQUAL(LOGGER_ERR_NULL_LOGGER, status);
}

/*
* 3a. loggerGetEntry [MC/DC Pair B-1] (False, True, False Case)
*
* Tests that loggerGetEntry:
* - returns a LOGGER_ERR_INVALID_INDEX error when 
*   passed an index that is out of bounds (index >= head)
*/
static void test_loggerGetEntry_index_too_new_returns_error(void) {
    // Arrange: Initialize logger and add a known entry to ensure it's not empty
    Logger logger = {0}; // Start with an empty logger
    loggerAppend(&logger, 1234567890U, LOG_LEVEL_INFO, "Test message");
    uint32_t invalidIndex = 3U; // Set to an index that is out of bounds

    // Act: Call loggerGetEntry with an invalid index 
    //      (only index 0 is valid since we added one entry)
    LogEntry retrievedEntry;
    LoggerStatus status = 
        loggerGetEntry(&logger, invalidIndex, &retrievedEntry);

    // Assert: Should return LOGGER_ERR_INVALID_INDEX error
    ASSERT_EQUAL(LOGGER_ERR_INVALID_INDEX, status);
}

/*
* 3b. loggerGetEntry [MC/DC Pair B-2] (False, True, False Case)
*
* Tests that loggerGetEntry:
* - returns a LOGGER_ERR_INVALID_INDEX error when 
*   passed an index that is out of bounds (index < tail)
*/
static void test_loggerGetEntry_index_too_old_returns_error(void) {
    // Arrange: Initialize logger and force an overwrite state
    Logger logger = {0};

    // Set head and tail to simulate a state where index 0 is valid 
    // but index 1 is too old. Valid range: [2, 4]
    logger.tail = 2U;
    logger.head = 5U;
    uint32_t invalidIndex = 1U;
    LogEntry outEntry;

    // Act: Call loggerGetEntry with an invalid index that is too old (< tail)
    LoggerStatus status = 
        loggerGetEntry(&logger, invalidIndex, &outEntry);

    // Assert: Should return LOGGER_ERR_INVALID_INDEX error
    ASSERT_EQUAL(LOGGER_ERR_INVALID_INDEX, status);
}

/*
* 4. loggerGetEntry [MC/DC Pair C] (False, False, True Case)
*
* Tests that loggerGetEntry:
* - returns a LOGGER_ERR_NULL_ENTRY error when 
*   passed a NULL pointer for the output entry
*/
static void test_loggerGetEntry_null_outEntry_returns_error(void) {
    // Arrange: Initialize logger and add a known entry
    Logger logger = {0}; // Start with an empty logger
    loggerAppend(&logger, 1234567890U, LOG_LEVEL_INFO, "Test message");

    // Act: Call loggerGetEntry with a NULL output entry pointer
    LoggerStatus status = loggerGetEntry(&logger, 0U, NULL);

    // Assert: Should return LOGGER_ERR_NULL_ENTRY error
    ASSERT_EQUAL(LOGGER_ERR_NULL_ENTRY, status);
}


/******************************************************************************
*               Functional Tests (Requirements Coverage)                      *
*******************************************************************************/

/*
* Test that loggerGetCount:
*   - returns LOGGER_OK status
*   - sets the count pointer to 0 
* when called on an initialized logger
*/
static void test_loggerGetCount_after_init_returns_zero(void) {
    // Arrange: Initialize logger and count to a known non-zero value
    Logger logger = {0}; // Start with an empty logger
    size_t count = 999U;

    // Act
    loggerInit(&logger);
    LoggerStatus status = loggerGetCount(&logger, &count);

    // Assert
    ASSERT_EQUAL(LOGGER_OK, status); // Should return LOGGER_OK status
    ASSERT_EQUAL(0U, count); // Initialized logger should have a count of 0
}

/*
* Empty logger edge case
*
* Test that loggerGetEntry:
* - returns LOGGER_ERR_INVALID_INDEX
*   when called with index 0 on an empty logger (head == tail)
*/
static void test_loggerGetEntry_empty_logger_returns_error(void) {
    // Arrange: Initialize logger and set head equal to tail
    Logger logger = {0};
    logger.head = 0U;
    logger.tail = 0U;

    // Act: Call loggerGetEntry with index 0 on an empty logger
    LogEntry outEntry;
    LoggerStatus status = loggerGetEntry(&logger, 0U, &outEntry);

    // Assert: Should return LOGGER_ERR_INVALID_INDEX error
    ASSERT_EQUAL(LOGGER_ERR_INVALID_INDEX, status);
}

/******************************************************************************
*                       Main function to run all tests                        *
*******************************************************************************/

int main(void) {
    /** MC/DC Tests **/

    printf("\n");

    // loggerInit
    RUN_TEST(test_loggerInit_valid_returns_ok);
    RUN_TEST(test_loggerInit_null_logger_returns_error);

    printf("\n");

    // loggerClear
    RUN_TEST(test_loggerClear_valid_returns_ok);
    RUN_TEST(test_loggerClear_null_logger_returns_error);

    printf("\n");

    // loggerGetCount
    RUN_TEST(test_loggerGetCount_valid_returns_ok);
    RUN_TEST(test_loggerGetCount_null_logger_returns_error);
    RUN_TEST(test_loggerGetCount_null_count_returns_error);

    printf("\n");

    // loggerAppend
    RUN_TEST(test_loggerAppend_valid_returns_ok);
    RUN_TEST(test_loggerAppend_null_logger_returns_error);
    RUN_TEST(test_loggerAppend_null_message_returns_error);
    RUN_TEST(test_loggerAppend_level_too_low_returns_error);
    RUN_TEST(test_loggerAppend_level_too_high_returns_error);

    printf
    ("\n");

    // loggerGetEntry
    RUN_TEST(test_loggerGetEntry_valid_returns_ok);
    RUN_TEST(test_loggerGetEntry_null_logger_returns_error);
    RUN_TEST(test_loggerGetEntry_index_too_new_returns_error);
    RUN_TEST(test_loggerGetEntry_index_too_old_returns_error);
    RUN_TEST(test_loggerGetEntry_null_outEntry_returns_error);

    /** Functional Tests **/
    RUN_TEST(test_loggerGetCount_after_init_returns_zero);
    RUN_TEST(test_loggerGetEntry_empty_logger_returns_error);

    printf("\n");

    printf("Tests run: %d\n", testsRun);
    printf("Tests failed: %d\n", testsFailed);

    printf("\n");

    // Return non-zero exit code if any tests failed
    return testsFailed ? 1 : 0;
}
