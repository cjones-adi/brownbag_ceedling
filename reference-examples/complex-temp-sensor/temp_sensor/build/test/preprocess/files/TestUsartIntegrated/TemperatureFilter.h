// CEEDLING NOTICE: This generated file only to be consumed by CMock

#ifndef _TEMPERATUREFILTER_H_ // Ceedling-generated include guard
#define _TEMPERATUREFILTER_H_

#include "Types.h"

void TemperatureFilter_Init(void);
float TemperatureFilter_GetTemperatureInCelcius(void);
void TemperatureFilter_ProcessInput(float temperature);

#endif // _TEMPERATUREFILTER_H_
