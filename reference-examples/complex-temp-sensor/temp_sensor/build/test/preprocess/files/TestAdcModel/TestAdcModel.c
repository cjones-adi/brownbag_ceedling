// CEEDLING NOTICE: This generated file only to be consumed for test runner creation

#include "build/vendor/unity/src/unity.h"
#include "src/Types.h"
#include "src/AdcModel.h"
#include "MockTaskScheduler.h"
#include "MockTemperatureCalculator.h"
#include "MockTemperatureFilter.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void testDoGetSampleShouldReturn_FALSE_WhenTaskSchedulerReturns_FALSE(void)
{
  TaskScheduler_DoAdc_CMockExpectAndReturn(25, (0));
  do { if (!(AdcModel_DoGetSample()))
{ } else { UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((26))); } } while (0);
}

void testDoGetSampleShouldReturn_TRUE_WhenTaskSchedulerReturns_TRUE(void)
{
  TaskScheduler_DoAdc_CMockExpectAndReturn(31, (1));
  do { if ((AdcModel_DoGetSample()))
{ } else { UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((32))); } } while (0);
}

void testProcessInputShouldDelegateToTemperatureCalculatorAndPassResultToFilter(void)
{
  TemperatureCalculator_Calculate_CMockExpectAndReturn(37, 21473, 23.5f);
  TemperatureFilter_ProcessInput_CMockExpect(38, 23.5f);
  AdcModel_ProcessInput(21473);
}