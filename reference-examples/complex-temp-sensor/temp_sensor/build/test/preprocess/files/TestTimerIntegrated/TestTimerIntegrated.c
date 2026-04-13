// CEEDLING NOTICE: This generated file only to be consumed for test runner creation

#include "build/vendor/unity/src/unity.h"
#include "src/Types.h"
#include "src/TimerConductor.h"
#include "src/TimerHardware.h"
#include "src/TimerModel.h"
#include "MockTimerConfigurator.h"
#include "MockTimerInterruptHandler.h"
#include "MockTaskScheduler.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void testInitShouldCallHardwareInit(void)
{
  Timer_EnablePeripheralClocks_CMockExpect(32);
  Timer_Reset_CMockExpect(33);
  Timer_ConfigureMode_CMockExpect(34);
  Timer_ConfigurePeriod_CMockExpect(35);
  Timer_EnableOutputPin_CMockExpect(36);
  Timer_Enable_CMockExpect(37);
  Timer_ConfigureInterruptHandler_CMockExpect(38);
  Timer_Start_CMockExpect(39);

  TimerConductor_Init();
}

void testRunShouldGetSystemTimeAndPassOnToModelForEventScheduling(void)
{
  Timer_GetSystemTime_CMockExpectAndReturn(46, 1230);
  TaskScheduler_Update_CMockExpect(47, 1230);
  TimerConductor_Run();

  Timer_GetSystemTime_CMockExpectAndReturn(50, 837460);
  TaskScheduler_Update_CMockExpect(51, 837460);
  TimerConductor_Run();
}