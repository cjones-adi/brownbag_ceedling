// CEEDLING NOTICE: This generated file only to be consumed for test runner creation

#include "build/vendor/unity/src/unity.h"
#include "test/support/UnityHelper.h"
#include "src/Types.h"
#include "src/AdcConductor.h"
#include "MockAdcModel.h"
#include "MockAdcHardware.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void testInitShouldCallHardwareInit(void)
{
  AdcHardware_Init_CMockExpect(26);
  AdcConductor_Init();
}

void testRunShouldNotDoAnythingIfItIsNotTime(void)
{
  AdcModel_DoGetSample_CMockExpectAndReturn(32, (0));

  AdcConductor_Run();
}

void testRunShouldNotPassAdcResultToModelIfSampleIsNotComplete(void)
{
  AdcModel_DoGetSample_CMockExpectAndReturn(39, (1));
  AdcHardware_GetSampleComplete_CMockExpectAndReturn(40, (0));

  AdcConductor_Run();
}

void testRunShouldGetLatestSampleFromAdcAndPassItToModelAndStartNewConversionWhenItIsTime(void)
{
  AdcModel_DoGetSample_CMockExpectAndReturn(47, (1));
  AdcHardware_GetSampleComplete_CMockExpectAndReturn(48, (1));
  AdcHardware_GetSample_CMockExpectAndReturn(49, 293U);
  AdcModel_ProcessInput_CMockExpect(50, 293U);
  AdcHardware_StartConversion_CMockExpect(51);

  AdcConductor_Run();
}

void testJustHereToTest_Should_ProperlyPassAStructAndVerifyIt(void)
{
    EXAMPLE_STRUCT_T TestStruct;
    TestStruct.x = 5;
    TestStruct.y = 7;

    AdcModel_DoNothingExceptTestASpecialType_CMockExpectAndReturn(62, TestStruct, (1));

    do { if ((AdcConductor_JustHereToTest()))
{ } else { UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((64))); } } while (0);
}
void test_AdcConductor_AlsoHereToTest_Should_ProperlyReturnAStructAsExpected1(void)
{
    EXAMPLE_STRUCT_T TestStruct;
    TestStruct.x = 99;
    TestStruct.y = 1;

    AdcModel_DoNothingExceptReturnASpecialType_CMockExpectAndReturn(95, TestStruct);

    do { if ((AdcConductor_AlsoHereToTest()))
{ } else { UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((97))); } } while (0);
}

void test_AdcConductor_AlsoHereToTest_Should_ProperlyReturnAStructAsExpected2(void)
{
    EXAMPLE_STRUCT_T TestStruct;
    TestStruct.x = 98;
    TestStruct.y = 1;

    AdcModel_DoNothingExceptReturnASpecialType_CMockExpectAndReturn(106, TestStruct);

    do { if (!(AdcConductor_AlsoHereToTest()))
{ } else { UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((108))); } } while (0);
}

void test_AdcConductor_YetAnotherTest_Should_VerifyThatPointersToStructsAreTestable(void)
{
    uint32 TestNum = 3;

    AdModel_DoNothingExceptTestPointers_CMockExpectAndReturn(115, &TestNum, (1));

    do { if ((AdcConductor_YetAnotherTest()))
{ } else { UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((117))); } } while (0);
}