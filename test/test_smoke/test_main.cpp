#include <Arduino.h>
#include <unity.h>

void test_smoke_boots_framework()
{
    TEST_ASSERT_TRUE(true);
}

void setup()
{
    delay(2000);
    UNITY_BEGIN();
    RUN_TEST(test_smoke_boots_framework);
    UNITY_END();
}

void loop()
{
}
