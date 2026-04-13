// CEEDLING NOTICE: This generated file only to be consumed by CMock

#ifndef _TASKSCHEDULER_H_ // Ceedling-generated include guard
#define _TASKSCHEDULER_H_

#include "Types.h"

void TaskScheduler_Init(void);
void TaskScheduler_Update(uint32 time);
bool TaskScheduler_DoUsart(void);
bool TaskScheduler_DoAdc(void);

#endif // _TASKSCHEDULER_H_
