// CEEDLING NOTICE: This generated file only to be consumed by CMock

#ifndef _USARTCONFIGURATOR_H_ // Ceedling-generated include guard
#define _USARTCONFIGURATOR_H_

#include "Types.h"

void Usart_ConfigureUsartIO(void);
void Usart_EnablePeripheralClock(void);
void Usart_Reset(void);
void Usart_ConfigureMode(void);
void Usart_SetBaudRateRegister(uint8 baudRateRegisterSetting);
void Usart_Enable(void);

#endif // _USARTCONFIGURATOR_H_
