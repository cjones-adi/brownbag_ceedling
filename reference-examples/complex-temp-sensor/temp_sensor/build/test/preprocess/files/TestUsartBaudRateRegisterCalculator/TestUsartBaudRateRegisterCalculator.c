// CEEDLING NOTICE: This generated file only to be consumed for test runner creation

#include "build/vendor/unity/src/unity.h"
#include "src/Types.h"
#include "src/UsartBaudRateRegisterCalculator.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void testCalculateBaudRateRegisterSettingShouldCalculateRegisterSettingAppropriately(void)
{
  UnityAssertEqualNumber((UNITY_INT)((26)), (UNITY_INT)((UsartModel_CalculateBaudRateRegisterSetting(48000000, 115200))), (
 ((void *)0)
 ), (UNITY_UINT)(23), UNITY_DISPLAY_STYLE_INT);
  UnityAssertEqualNumber((UNITY_INT)((6)), (UNITY_INT)((UsartModel_CalculateBaudRateRegisterSetting(3686400, 38400))), (
 ((void *)0)
 ), (UNITY_UINT)(24), UNITY_DISPLAY_STYLE_INT);
  UnityAssertEqualNumber((UNITY_INT)((23)), (UNITY_INT)((UsartModel_CalculateBaudRateRegisterSetting(14318180, 38400))), (
 ((void *)0)
 ), (UNITY_UINT)(25), UNITY_DISPLAY_STYLE_INT);
  UnityAssertEqualNumber((UNITY_INT)((20)), (UNITY_INT)((UsartModel_CalculateBaudRateRegisterSetting(12000000, 38400))), (
 ((void *)0)
 ), (UNITY_UINT)(26), UNITY_DISPLAY_STYLE_INT);
  UnityAssertEqualNumber((UNITY_INT)((13)), (UNITY_INT)((UsartModel_CalculateBaudRateRegisterSetting(12000000, 56800))), (
 ((void *)0)
 ), (UNITY_UINT)(27), UNITY_DISPLAY_STYLE_INT);
}