// CEEDLING NOTICE: This generated file only to be consumed for test runner creation

#include "build/vendor/unity/src/unity.h"
#include "src/Types.h"
#include "src/UsartHardware.h"
#include "MockUsartConfigurator.h"
#include "MockUsartPutChar.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void testInitShouldConfigureUsartPeripheralByCallingConfiguratorAppropriately(void)
{
  Usart_ConfigureUsartIO_CMockExpect(24);
  Usart_EnablePeripheralClock_CMockExpect(25);
  Usart_Reset_CMockExpect(26);
  Usart_ConfigureMode_CMockExpect(27);
  Usart_SetBaudRateRegister_CMockExpect(28, 73);
  Usart_Enable_CMockExpect(29);

  UsartHardware_Init(73);
}

void testTransmitStringShouldSendDesiredStringOutUsingUsart(void)
{
  Usart_PutChar_CMockExpect(36, 'h');
  Usart_PutChar_CMockExpect(37, 'e');
  Usart_PutChar_CMockExpect(38, 'l');
  Usart_PutChar_CMockExpect(39, 'l');
  Usart_PutChar_CMockExpect(40, 'o');

  UsartHardware_TransmitString("hello");
}