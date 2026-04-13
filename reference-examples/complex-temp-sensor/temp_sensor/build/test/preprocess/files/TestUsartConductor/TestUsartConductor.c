// CEEDLING NOTICE: This generated file only to be consumed for test runner creation

#include "build/vendor/unity/src/unity.h"
#include "src/Types.h"
#include "src/UsartConductor.h"
#include "MockUsartModel.h"
#include "MockUsartHardware.h"
#include "MockTaskScheduler.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void testShouldInitializeHardwareWhenInitCalled(void)
{
  UsartModel_GetBaudRateRegisterSetting_CMockExpectAndReturn(25, 4);
  UsartHardware_Init_CMockExpect(26, 4);
  UsartModel_GetWakeupMessage_CMockExpectAndReturn(27, "Hey there!");
  UsartHardware_TransmitString_CMockExpect(28, "Hey there!");

  UsartConductor_Init();
}

void testRunShouldNotDoAnythingIfSchedulerSaysItIsNotTimeYet(void)
{
  TaskScheduler_DoUsart_CMockExpectAndReturn(35, (0));

  UsartConductor_Run();
}

void testRunShouldGetCurrentTemperatureAndTransmitIfSchedulerSaysItIsTime(void)
{
  TaskScheduler_DoUsart_CMockExpectAndReturn(42, (1));
  UsartModel_GetFormattedTemperature_CMockExpectAndReturn(43, "hey there");
  UsartHardware_TransmitString_CMockExpect(44, "hey there");

  UsartConductor_Run();
}