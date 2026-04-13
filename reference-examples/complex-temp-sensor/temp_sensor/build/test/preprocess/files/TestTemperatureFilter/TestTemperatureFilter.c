// CEEDLING NOTICE: This generated file only to be consumed for test runner creation

#include "build/vendor/unity/src/unity.h"
#include "src/Types.h"
#include "src/TemperatureFilter.h"

void setUp(void)
{
  TemperatureFilter_Init();
}

void tearDown(void)
{
}

void testShouldInitializeTemeratureToInvalidValue(void)
{
  TemperatureFilter_Init();
  UnityAssertFloatSpecial((UNITY_FLOAT)((TemperatureFilter_GetTemperatureInCelcius())), (
 ((void *)0)
 ), (UNITY_UINT)(25), UNITY_FLOAT_IS_NEG_INF);
}

void testShouldInitializeTemperatureAfterCallToInit(void)
{
  TemperatureFilter_Init();
  TemperatureFilter_ProcessInput(17.8f);
  UnityAssertFloatsWithin((UNITY_FLOAT)((0.0001f)), (UNITY_FLOAT)((17.8f)), (UNITY_FLOAT)((TemperatureFilter_GetTemperatureInCelcius())), (
 ((void *)0)
 ), (UNITY_UINT)(32));

  TemperatureFilter_Init();
  TemperatureFilter_ProcessInput(32.6f);
  UnityAssertFloatsWithin((UNITY_FLOAT)((0.0001f)), (UNITY_FLOAT)((32.6f)), (UNITY_FLOAT)((TemperatureFilter_GetTemperatureInCelcius())), (
 ((void *)0)
 ), (UNITY_UINT)(36));
}

void setValueAndVerifyResponse(float input, float response)
{
  float actual;
  TemperatureFilter_ProcessInput(input);
  actual = TemperatureFilter_GetTemperatureInCelcius();

  if (input == +
               (__builtin_inff ())
                        ||
      input == -
               (__builtin_inff ())
                        ||

     __builtin_isnan (
     input
     )
                 )
{
    UnityAssertFloatSpecial((UNITY_FLOAT)((actual)), (
   ((void *)0)
   ), (UNITY_UINT)(49), UNITY_FLOAT_IS_NEG_INF);
  }
  else
  {
    UnityAssertFloatsWithin((UNITY_FLOAT)((0.0001f)), (UNITY_FLOAT)((response)), (UNITY_FLOAT)((actual)), (
   ((void *)0)
   ), (UNITY_UINT)(53));
  }
}

void testShouldWeightEachSubsequentValueBy25PercentAfterInitialValue(void)
{
  TemperatureFilter_Init();
  setValueAndVerifyResponse(0.0f, 0.0f);
  setValueAndVerifyResponse(10.0f, 2.5f);
  setValueAndVerifyResponse(10.0f, 4.375f);
  setValueAndVerifyResponse(10.0f, 5.78125f);

  TemperatureFilter_Init();
  setValueAndVerifyResponse(100.0f, 100.0f);
  setValueAndVerifyResponse(0.0f, 75.0f);
  setValueAndVerifyResponse(0.0f, 56.25f);
  setValueAndVerifyResponse(0.0f, 42.1875f);
}

void setInvalidTemperatureAndVerifyReinitialized(float invalidTemperature)
{
  TemperatureFilter_Init();
  setValueAndVerifyResponse(100.0f, 100.0f);
  setValueAndVerifyResponse(invalidTemperature, -
                                                (__builtin_inff ())
                                                        );
  setValueAndVerifyResponse(14.3f, 14.3f);
}

void testShouldResetAverageIfPassedInfinityOrInvalidValue(void)
{
  setInvalidTemperatureAndVerifyReinitialized(-
                                              (__builtin_inff ())
                                                      );
  setInvalidTemperatureAndVerifyReinitialized(+
                                              (__builtin_inff ())
                                                      );
  setInvalidTemperatureAndVerifyReinitialized(+
                                              (__builtin_nanf (""))
                                                 );
  setInvalidTemperatureAndVerifyReinitialized(-
                                              (__builtin_nanf (""))
                                                 );
}