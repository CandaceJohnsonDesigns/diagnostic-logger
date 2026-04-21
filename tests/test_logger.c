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

/*******************************************
* Test macros for assertions and reporting *
********************************************/

/* 
* Basic assertion
* If condition fails:
* - Print location and condition that failed
* - Increment test failure count
* - Return from the test function to prevent further execution
*/
#define ASSERT_TRUE(condition)                                                  \
    do {                                                                        \
        if (!(condition)) {                                                     \
            printf("Assertion failed: %s, in function %s, file %s, line %d\n",  \
                #condition, __func__, __FILE__, __LINE__);                      \
            testsFailed++;                                                      \
            return;                                                             \
        }                                                                       \
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

/**************************************
* Test cases for Logger functionality *
***************************************/

/*
* Test that loggerInit: 
* - returns a LOGGER_ERR_NULL_LOGGER error when passed a NULL Logger pointer
*/
static void test_loggerInit_null_logger_returns_error(void) {
    ASSERT_EQUAL(LOGGER_ERR_NULL_LOGGER, loggerInit(NULL));
}

/*
* Test that loggerInit:
* - returns LOGGER_OK status
* - initializes the Logger struct to an empty state (head and tail set to 0)
*/
static void test_loggerInit_sets_empty_state(void) {
    Logger logger;
    LoggerStatus status = loggerInit(&logger);

    ASSERT_EQUAL(LOGGER_OK, status);

    ASSERT_EQUAL(0U, logger.head);
    ASSERT_EQUAL(0U, logger.tail);
}

/*
* Test that loggerGetCount:
* - returns LOGGER_OK status
* - sets the count pointer to 0 when called on an initialized logger
*/
static void test_loggerGetCount_after_init_returns_zero(void) {
    Logger logger;
    size_t count = 999U; // Initialize to a non-zero value to ensure it gets set by the function

    loggerInit(&logger);

    LoggerStatus status = loggerGetCount(&logger, &count);

    ASSERT_EQUAL(LOGGER_OK, status);
    ASSERT_EQUAL(0U, count);
}

/**
* Test that loggerGetCount:
* - returns LOGGER_ERR_NULL_LOGGER error when passed a NULL Logger pointer
* - does not modify the count pointer value on error (i.e., it should remain unchanged)
*/
static void test_loggerGetCount_null_logger_returns_error(void) {
    size_t count = 123U; // Initialize to a non-zero value to ensure it doesn't get modified on error

    LoggerStatus status = loggerGetCount(NULL, &count);

    ASSERT_EQUAL(LOGGER_ERR_NULL_LOGGER, status);
    ASSERT_EQUAL(123U, count); // Ensure count is unchanged on error
}

/*
* Test that loggerGetCount:
* - returns LOGGER_ERR_NULL_COUNT error when passed a NULL count pointer
*/
static void test_loggerGetCount_null_count_returns_error(void) {
    Logger logger;
    
    LoggerStatus initStatus = loggerInit(&logger);
    ASSERT_EQUAL(LOGGER_OK, initStatus);

    LoggerStatus countStatus = loggerGetCount(&logger, NULL);
    ASSERT_EQUAL(LOGGER_ERR_NULL_COUNT, countStatus);
}

/*********************************
* Main function to run all tests *
**********************************/

int main(void) {
    RUN_TEST(test_loggerInit_null_logger_returns_error);
    RUN_TEST(test_loggerInit_sets_empty_state);
    RUN_TEST(test_loggerGetCount_after_init_returns_zero);
    RUN_TEST(test_loggerGetCount_null_logger_returns_error);
    RUN_TEST(test_loggerGetCount_null_count_returns_error);

    printf("\n"); // Add a newline for better readability of results

    printf("Tests run: %d\n", testsRun);
    printf("Tests failed: %d\n", testsFailed);

    // Return non-zero exit code if any tests failed
    return testsFailed ? 1 : 0;
}
