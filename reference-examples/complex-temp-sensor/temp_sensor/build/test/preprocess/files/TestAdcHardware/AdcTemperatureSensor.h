// CEEDLING NOTICE: This generated file only to be consumed by CMock

#ifndef _ADCTEMPERATURESENSOR_H_ // Ceedling-generated include guard
#define _ADCTEMPERATURESENSOR_H_

#include "Types.h"

void Adc_StartTemperatureSensorConversion(void);
bool Adc_TemperatureSensorSampleReady(void);
uint16 Adc_ReadTemperatureSensor(void);

#endif // _ADCTEMPERATURESENSOR_H_
