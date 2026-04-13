// CEEDLING NOTICE: This generated file only to be consumed for test runner creation

#include "build/vendor/unity/src/unity.h"
#include "src/Types.h"
#include "src/TimerHardware.h"
#include "MockTimerConfigurator.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void testInitShouldDelegateAppropriatelyToConfigurator(void)
{
  Timer_EnablePeripheralClocks_CMockExpect(23);
  Timer_Reset_CMockExpect(24);
  Timer_ConfigureMode_CMockExpect(25);
  Timer_ConfigurePeriod_CMockExpect(26);
  Timer_EnableOutputPin_CMockExpect(27);
  Timer_Enable_CMockExpect(28);
  Timer_ConfigureInterruptHandler_CMockExpect(29);
  Timer_Start_CMockExpect(30);

  TimerHardware_Init();
}