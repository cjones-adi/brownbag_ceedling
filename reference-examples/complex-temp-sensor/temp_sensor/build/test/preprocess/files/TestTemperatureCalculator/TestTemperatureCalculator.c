// CEEDLING NOTICE: This generated file only to be consumed for test runner creation

#include "build/vendor/unity/src/unity.h"
#include "src/Types.h"

TEST_SOURCE_FILE("TemperatureCalculator.c")

extern float TemperatureCalculator_Calculate(uint16_t val);

void setUp(void)
{
}

void tearDown(void)
{
}

void testTemperatureCalculatorShouldCalculateTemperatureFromMillivolts(void)
{
  float result;

  result = TemperatureCalculator_Calculate(1000);
  UnityAssertFloatsWithin((UNITY_FLOAT)((0.01f)), (UNITY_FLOAT)((25.0f)), (UNITY_FLOAT)((result)), (
 ((void *)0)
 ), (UNITY_UINT)(31));

  result = TemperatureCalculator_Calculate(2985);
  UnityAssertFloatsWithin((UNITY_FLOAT)((0.01f)), (UNITY_FLOAT)((68.317f)), (UNITY_FLOAT)((result)), (
 ((void *)0)
 ), (UNITY_UINT)(34));

  result = TemperatureCalculator_Calculate(3);
  UnityAssertFloatsWithin((UNITY_FLOAT)((0.01f)), (UNITY_FLOAT)((-19.96f)), (UNITY_FLOAT)((result)), (
 ((void *)0)
 ), (UNITY_UINT)(37));
}

void testShouldReturnNegativeInfinityWhen_0_millivoltsInput(void)
{
  UnityAssertFloatSpecial((UNITY_FLOAT)((TemperatureCalculator_Calculate(0))), (
 ((void *)0)
 ), (UNITY_UINT)(42), UNITY_FLOAT_IS_NEG_INF);
}