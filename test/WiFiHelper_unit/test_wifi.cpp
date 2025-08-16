#include <unity.h>

void test_wifi_simple();

void run_wifi_tests()
{
    RUN_TEST(test_wifi_simple);
}

void test_wifi_simple()
{
    TEST_ASSERT_EQUAL(2, 2);
}
