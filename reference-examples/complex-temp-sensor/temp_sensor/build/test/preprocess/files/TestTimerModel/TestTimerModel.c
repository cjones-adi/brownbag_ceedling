// CEEDLING NOTICE: This generated file only to be consumed for test runner creation

#include "build/vendor/unity/src/unity.h"
#include "src/Types.h"
#include "src/TimerModel.h"
#include "MockTaskScheduler.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void testUpdateTimeShouldDelegateToTaskScheduler(void)
{
  TaskScheduler_Update_CMockExpect(23, 19387L);
  TimerModel_UpdateTime(19387L);
}