// CEEDLING NOTICE: This generated file only to be consumed for test runner creation

#include "build/vendor/unity/src/unity.h"
#include "src/Types.h"
#include "src/TimerConductor.h"
#include "MockTimerHardware.h"
#include "MockTimerModel.h"
#include "MockTimerInterruptHandler.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void testInitShouldCallHardwareInit(void)
{
  TimerHardware_Init_CMockExpect(25);

  TimerConductor_Init();
}

void testRunShouldGetSystemTimeAndPassOnToModelForEventScheduling(void)
{
  Timer_GetSystemTime_CMockExpectAndReturn(32, 1230);
  TimerModel_UpdateTime_CMockExpect(33, 1230);
  TimerConductor_Run();

  Timer_GetSystemTime_CMockExpectAndReturn(36, 837460);
  TimerModel_UpdateTime_CMockExpect(37, 837460);
  TimerConductor_Run();
}