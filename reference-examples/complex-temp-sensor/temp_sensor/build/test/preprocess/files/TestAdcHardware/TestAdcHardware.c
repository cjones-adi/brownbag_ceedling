// CEEDLING NOTICE: This generated file only to be consumed for test runner creation

#include "build/vendor/unity/src/unity.h"
#include "src/Types.h"
#include "src/AdcHardware.h"
#include "MockAdcHardwareConfigurator.h"
#include "MockAdcTemperatureSensor.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void testInitShouldDelegateToConfiguratorAndTemperatureSensor(void)
{
  Adc_Reset_CMockExpect(24);
  Adc_ConfigureMode_CMockExpect(25);
  Adc_EnableTemperatureChannel_CMockExpect(26);
  Adc_StartTemperatureSensorConversion_CMockExpect(27);

  AdcHardware_Init();
}

void testGetSampleCompleteShouldReturn_FALSE_WhenTemperatureSensorSampleReadyReturns_FALSE(void)
{
  Adc_TemperatureSensorSampleReady_CMockExpectAndReturn(34, (0));
  do { if ((!AdcHardware_GetSampleComplete()))
{ } else { UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((35))); } } while (0);
}

void testGetSampleCompleteShouldReturn_TRUE_WhenTemperatureSensorSampleReadyReturns_TRUE(void)
{
  Adc_TemperatureSensorSampleReady_CMockExpectAndReturn(40, (1));
  do { if ((AdcHardware_GetSampleComplete()))
{ } else { UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((41))); } } while (0);
}

void testGetSampleShouldDelegateToAdcTemperatureSensor(void)
{
  uint16 sample;
  Adc_ReadTemperatureSensor_CMockExpectAndReturn(47, 847);

  sample = AdcHardware_GetSample();
  UnityAssertEqualNumber((UNITY_INT)((847)), (UNITY_INT)((sample)), (
 ((void *)0)
 ), (UNITY_UINT)(50), UNITY_DISPLAY_STYLE_INT);
}