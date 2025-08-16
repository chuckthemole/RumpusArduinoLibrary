#include <unity.h>

// Forward declaration (if you want to keep run_logger_tests() first)
void test_logger_simple();

// Function that runs all tests in this module
void run_logger_tests() {
    RUN_TEST(test_logger_simple);
}

// Actual test definitions
void test_logger_simple() {
    TEST_ASSERT_EQUAL(1, 1);
}
