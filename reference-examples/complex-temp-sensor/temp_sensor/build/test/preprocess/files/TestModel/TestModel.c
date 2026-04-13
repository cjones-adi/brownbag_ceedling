// CEEDLING NOTICE: This generated file only to be consumed for test runner creation

#include "build/vendor/unity/src/unity.h"
#include "src/Types.h"
#include "src/Model.h"
#include "MockTaskScheduler.h"
#include "MockTemperatureFilter.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void testInitShouldCallSchedulerAndTemperatureFilterInit(void)
{
  TaskScheduler_Init_CMockExpect(24);
  TemperatureFilter_Init_CMockExpect(25);
  Model_Init();
}