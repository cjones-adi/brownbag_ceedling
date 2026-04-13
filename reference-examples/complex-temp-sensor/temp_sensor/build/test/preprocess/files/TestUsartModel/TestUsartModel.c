// CEEDLING NOTICE: This generated file only to be consumed for test runner creation

#include "build/vendor/unity/src/unity.h"
#include "src/Types.h"
#include "src/UsartModel.h"
#include "src/ModelConfig.h"
#include "MockTemperatureFilter.h"
#include "MockUsartBaudRateRegisterCalculator.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void testGetBaudRateRegisterSettingShouldReturnAppropriateBaudRateRegisterSetting(void)
{
  uint8 dummyRegisterSetting = 17;
  UsartModel_CalculateBaudRateRegisterSetting_CMockExpectAndReturn(27, 48054857, 115200, dummyRegisterSetting);

  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT8 )((dummyRegisterSetting)), (UNITY_INT)(UNITY_UINT8 )((UsartModel_GetBaudRateRegisterSetting())), (
 ((void *)0)
 ), (UNITY_UINT)(29), UNITY_DISPLAY_STYLE_UINT8);
}

void testGetFormattedTemperatureFormatsTemperatureFromCalculatorAppropriately(void)
{
  TemperatureFilter_GetTemperatureInCelcius_CMockExpectAndReturn(34, 25.0f);
  UnityAssertEqualString((const char*)(("25.0 C\n")), (const char*)((UsartModel_GetFormattedTemperature())), (
 ((void *)0)
 ), (UNITY_UINT)(35));
}

void testShouldReturnErrorMessageUponInvalidTemperatureValue(void)
{
  TemperatureFilter_GetTemperatureInCelcius_CMockExpectAndReturn(40, -
 (__builtin_inff ())
 );
  UnityAssertEqualString((const char*)(("Temperature sensor failure!\n")), (const char*)((UsartModel_GetFormattedTemperature())), (
 ((void *)0)
 ), (UNITY_UINT)(41));
}

void testShouldReturnWakeupMessage(void)
{
  UnityAssertEqualString((const char*)(("It's Awesome Time!\n")), (const char*)((UsartModel_GetWakeupMessage())), (
 ((void *)0)
 ), (UNITY_UINT)(46));
}