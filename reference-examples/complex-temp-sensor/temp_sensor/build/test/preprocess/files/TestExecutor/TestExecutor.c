// CEEDLING NOTICE: This generated file only to be consumed for test runner creation

#include "build/vendor/unity/src/unity.h"
#include "src/Types.h"
#include "src/Executor.h"
#include "MockModel.h"
#include "MockUsartConductor.h"
#include "MockAdcConductor.h"
#include "MockTimerConductor.h"
#include "MockIntrinsicsWrapper.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void testInitShouldCallInitOfAllConductorsAndTheModel(void)
{
  Model_Init_CMockExpect(27);
  UsartConductor_Init_CMockExpect(28);
  AdcConductor_Init_CMockExpect(29);
  TimerConductor_Init_CMockExpect(30);
  Interrupt_Enable_CMockExpect(31);

  Executor_Init();
}

void testRunShouldCallRunForEachConductorAndReturnTrueAlways(void)
{
  UsartConductor_Run_CMockExpect(38);
  TimerConductor_Run_CMockExpect(39);
  AdcConductor_Run_CMockExpect(40);

  do { if ((Executor_Run()))
{ } else { UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((42))); } } while (0);
}