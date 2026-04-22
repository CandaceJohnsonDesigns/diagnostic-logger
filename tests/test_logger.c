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
                "Assertion failed: %s, in function %s, file %s, line %d\n",   \
                #condition, __func__, __FILE__, __LINE__);                    \
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
                in function %s, file %s, line %d\n",                    \
                (long long)(expected), (long long)(actual),             \
                __func__, __FILE__, __LINE__);                          \
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
*       test_loggerGetCount_null_logger_returns_error   (T, X) vs #1          *
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
    size_t count = 0U;
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
* 2. loggerGetCount [MC/DC Independence Pair A] (True, X Case)
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
*  MC/DC Analysis: loggerAppend                                             *
*  Decision: (logger == NULL) || (message == NULL)                           *
*                                                                             *
* 1. [Baseline/Functional]:                                                   *
*       test_loggerAppend_valid_returns_ok            (F, F)                *
* 2. [Pair A] (logger):                                                       *
*       test_loggerAppend_null_logger_returns_error   (T, X) vs #1          *
* 3. [Pair B] (message):                                                     *
*       test_loggerAppend_null_count_returns_error    (F, T) vs #1          *
* --------------------------------------------------------------------------- */

/*
* 2. loggerAppend [MC/DC Independence Pair A] (True, X Case)
*
* Tests that loggerAppend: 
* - returns a LOGGER_ERR_NULL_LOGGER error when passed a NULL Logger pointer
*/
static void test_loggerAppend_null_logger_returns_error(void) {
    // Arrange: Set up valid parameters for loggerAppend
    LogLevel level = LOG_LEVEL_INFO;
    uint32_t timestamp = 1234567890;
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

/******************************************************************************
*               Functional Tests (Requirements Coverage)                      *
*******************************************************************************/

/*
* Test that loggerGetCount:
* - returns LOGGER_OK status
* - sets the count pointer to 0 when called on an initialized logger
*/
static void test_loggerGetCount_after_init_returns_zero(void) {
    // Arrange: Initialize logger and count to a known non-zero value
    Logger logger;
    size_t count = 999U;

    // Act
    loggerInit(&logger);
    LoggerStatus status = loggerGetCount(&logger, &count);

    // Assert
    ASSERT_EQUAL(LOGGER_OK, status); // Should return LOGGER_OK status
    ASSERT_EQUAL(0U, count); // Initialized logger should have a count of 0
}

/*********************************
* Main function to run all tests *
**********************************/

int main(void) {
    /** MC/DC Tests **/

    // loggerInit
    RUN_TEST(test_loggerInit_valid_returns_ok);
    RUN_TEST(test_loggerInit_null_logger_returns_error);

    // loggerClear
    RUN_TEST(test_loggerClear_valid_returns_ok);
    RUN_TEST(test_loggerClear_null_logger_returns_error);

    // loggerGetCount
    RUN_TEST(test_loggerGetCount_valid_returns_ok);
    RUN_TEST(test_loggerGetCount_null_logger_returns_error);
    RUN_TEST(test_loggerGetCount_null_count_returns_error);

    // loggerAppend
    RUN_TEST(test_loggerAppend_null_logger_returns_error);

    /** Functional Tests **/
    RUN_TEST(test_loggerGetCount_after_init_returns_zero);


    printf("\n"); // Add a newline for better readability of results

    printf("Tests run: %d\n", testsRun);
    printf("Tests failed: %d\n", testsFailed);

    // Return non-zero exit code if any tests failed
    return testsFailed ? 1 : 0;
}
