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

    ASSERT_EQUAL(0, logger.head);
    ASSERT_EQUAL(0, logger.tail);
}

/*********************************
* Main function to run all tests *
**********************************/

int main(void) {
    RUN_TEST(test_loggerInit_null_logger_returns_error);
    RUN_TEST(test_loggerInit_sets_empty_state);

    printf("\n"); // Add a newline for better readability of results

    printf("Tests run: %d\n", testsRun);
    printf("Tests failed: %d\n", testsFailed);

    // Return non-zero exit code if any tests failed
    return testsFailed ? 1 : 0;
}