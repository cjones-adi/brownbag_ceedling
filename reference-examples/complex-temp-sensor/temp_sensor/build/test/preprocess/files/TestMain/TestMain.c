// CEEDLING NOTICE: This generated file only to be consumed for test runner creation

#include "build/vendor/unity/src/unity.h"
#include "src/Types.h"
#include "src/Main.h"
#include "MockExecutor.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void testMainShouldCallExecutorInitAndContinueToCallExecutorRunUntilHalted(void)
{
  Executor_Init_CMockExpect(23);
  Executor_Run_CMockExpectAndReturn(24, (1));
  Executor_Run_CMockExpectAndReturn(25, (1));
  Executor_Run_CMockExpectAndReturn(26, (1));
  Executor_Run_CMockExpectAndReturn(27, (1));
  Executor_Run_CMockExpectAndReturn(28, (0));

  AppMain();
}