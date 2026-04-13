// CEEDLING NOTICE: This generated file only to be consumed for test runner creation

#include "build/vendor/unity/src/unity.h"
#include "src/Types.h"
#include "src/UsartConductor.h"
#include "src/UsartModel.h"
#include "src/UsartHardware.h"
#include "src/ModelConfig.h"
#include "MockTaskScheduler.h"
#include "MockUsartConfigurator.h"
#include "MockUsartPutChar.h"
#include "MockTemperatureFilter.h"
#include "MockUsartBaudRateRegisterCalculator.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void testShouldInitializeHardwareWhenInitCalled(void)
{
  size_t i;
  const char* test_str = "It's Awesome Time!\n";

  UsartModel_CalculateBaudRateRegisterSetting_CMockExpectAndReturn(43, 48054857, 115200, 4);
  Usart_ConfigureUsartIO_CMockExpect(44);
  Usart_EnablePeripheralClock_CMockExpect(45);
  Usart_Reset_CMockExpect(46);
  Usart_ConfigureMode_CMockExpect(47);
  Usart_SetBaudRateRegister_CMockExpect(48, 4);
  Usart_Enable_CMockExpect(49);
  for (i=0; i < strlen(test_str); i++)
{
    Usart_PutChar_CMockExpect(52, test_str[i]);
  }

  UsartConductor_Init();
}

void testRunShouldNotDoAnythingIfSchedulerSaysItIsNotTimeYet(void)
{
  TaskScheduler_DoUsart_CMockExpectAndReturn(60, (0));

  UsartConductor_Run();
}