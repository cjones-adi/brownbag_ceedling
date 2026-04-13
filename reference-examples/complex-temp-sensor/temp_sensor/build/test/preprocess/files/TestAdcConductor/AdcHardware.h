// CEEDLING NOTICE: This generated file only to be consumed by CMock

#ifndef _ADCHARDWARE_H_ // Ceedling-generated include guard
#define _ADCHARDWARE_H_

#include "Types.h"

void AdcHardware_Init(void);
void AdcHardware_StartConversion(void);
bool AdcHardware_GetSampleComplete(void);
uint16 AdcHardware_GetSample(void);

#endif // _ADCHARDWARE_H_
