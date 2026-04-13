// CEEDLING NOTICE: This generated file only to be consumed by CMock

#ifndef _ADCMODEL_H_ // Ceedling-generated include guard
#define _ADCMODEL_H_

#include "Types.h"

bool AdcModel_DoGetSample(void);
void AdcModel_ProcessInput(uint16 millivolts);

bool AdcModel_DoNothingExceptTestASpecialType(EXAMPLE_STRUCT_T ExampleStruct);
bool AdModel_DoNothingExceptTestPointers(uint32* pExample);
EXAMPLE_STRUCT_T AdcModel_DoNothingExceptReturnASpecialType(void);

#endif // _ADCMODEL_H_
