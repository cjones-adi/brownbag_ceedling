// CEEDLING NOTICE: This generated file only to be consumed for test runner creation

#include "build/vendor/unity/src/unity.h"
#include "src/Types.h"
#include "src/TaskScheduler.h"

void setUp(void)
{
  TaskScheduler_Init();
}

void tearDown(void)
{
}

void testShouldScheduleUsartTaskAfter1000ms(void)
{
  do { if (!(TaskScheduler_DoUsart()))
{ } else { UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((23))); } } while (0);

  TaskScheduler_Update(999);
  do { if (!(TaskScheduler_DoUsart()))
{ } else { UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((26))); } } while (0);

  TaskScheduler_Update(1000);
  do { if ((TaskScheduler_DoUsart()))
{ } else { UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((29))); } } while (0);
}

void testShouldClearUsartDoFlagAfterReported(void)
{
  do { if (!(TaskScheduler_DoUsart()))
{ } else { UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((34))); } } while (0);
  TaskScheduler_Update(1000);
  do { if ((TaskScheduler_DoUsart()))
{ } else { UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((36))); } } while (0);
  do { if (!(TaskScheduler_DoUsart()))
{ } else { UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((37))); } } while (0);
}

void testShouldScheduleUsartTaskEvery1000ms(void)
{
  do { if (!(TaskScheduler_DoUsart()))
{ } else { UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((42))); } } while (0);

  TaskScheduler_Update(1300);
  do { if ((TaskScheduler_DoUsart()))
{ } else { UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((45))); } } while (0);

  TaskScheduler_Update(2000);
  do { if ((TaskScheduler_DoUsart()))
{ } else { UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((48))); } } while (0);

  TaskScheduler_Update(3100);
  do { if ((TaskScheduler_DoUsart()))
{ } else { UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((51))); } } while (0);
}

void testShouldScheduleUsartTaskOnlyOncePerPeriod(void)
{
  do { if (!(TaskScheduler_DoUsart()))
{ } else { UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((56))); } } while (0);
  TaskScheduler_Update(1000);
  do { if ((TaskScheduler_DoUsart()))
{ } else { UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((58))); } } while (0);
  TaskScheduler_Update(1001);
  do { if (!(TaskScheduler_DoUsart()))
{ } else { UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((60))); } } while (0);
  TaskScheduler_Update(1999);
  do { if (!(TaskScheduler_DoUsart()))
{ } else { UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((62))); } } while (0);
  TaskScheduler_Update(2000);
  do { if ((TaskScheduler_DoUsart()))
{ } else { UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((64))); } } while (0);
}

void testShouldScheduleAdcTaskAfter100ms(void)
{
  do { if (!(TaskScheduler_DoAdc()))
{ } else { UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((69))); } } while (0);

  TaskScheduler_Update(99);
  do { if (!(TaskScheduler_DoAdc()))
{ } else { UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((72))); } } while (0);

  TaskScheduler_Update(100);
  do { if ((TaskScheduler_DoAdc()))
{ } else { UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((75))); } } while (0);
}

void testShouldClearAdcDoFlagAfterReported(void)
{
  do { if (!(TaskScheduler_DoAdc()))
{ } else { UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((80))); } } while (0);
  TaskScheduler_Update(100);
  do { if ((TaskScheduler_DoAdc()))
{ } else { UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((82))); } } while (0);
  do { if (!(TaskScheduler_DoAdc()))
{ } else { UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((83))); } } while (0);
}

void testShouldScheduleAdcTaskEvery100ms(void)
{
  do { if (!(TaskScheduler_DoAdc()))
{ } else { UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((88))); } } while (0);

  TaskScheduler_Update(121);
  do { if ((TaskScheduler_DoAdc()))
{ } else { UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((91))); } } while (0);

  TaskScheduler_Update(200);
  do { if ((TaskScheduler_DoAdc()))
{ } else { UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((94))); } } while (0);

  TaskScheduler_Update(356);
  do { if ((TaskScheduler_DoAdc()))
{ } else { UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((97))); } } while (0);
}

void testShouldScheduleAdcTaskOnlyOncePerPeriod(void)
{
  do { if (!(TaskScheduler_DoAdc()))
{ } else { UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((102))); } } while (0);
  TaskScheduler_Update(100);
  do { if ((TaskScheduler_DoAdc()))
{ } else { UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((104))); } } while (0);
  TaskScheduler_Update(101);
  do { if (!(TaskScheduler_DoAdc()))
{ } else { UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((106))); } } while (0);
  TaskScheduler_Update(199);
  do { if (!(TaskScheduler_DoAdc()))
{ } else { UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((108))); } } while (0);
  TaskScheduler_Update(200);
  do { if ((TaskScheduler_DoAdc()))
{ } else { UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((110))); } } while (0);
}