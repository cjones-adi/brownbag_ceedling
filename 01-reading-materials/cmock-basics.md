# 🎭 CMock – Hardware Interface Mocking Basics

## What Is It?
CMock is a **automatic mock generator** for C, built by Throw The Switch. It generates mock versions of your hardware interfaces so you can test code in isolation — no real hardware required.

## The Problem It Solves
Embedded code often calls hardware drivers directly (SPI, I2C, GPIO, ADC, etc.). CMock **replaces those calls** with controllable stubs during unit testing.

## How It Works
1. Point CMock at a **header file** (e.g., `uart_driver.h`)
2. CMock auto-generates `mock_uart_driver.c` / `.h`
3. Your test uses the mock to **set expectations** and **inject return values**
4. Ceedling handles generation automatically — no manual step needed

## Core Mock Patterns

| Pattern | Example |
|---|---|
| **Expect a call** | `UART_Send_Expect(0x41)` |
| **Return a value** | `ADC_Read_ExpectAndReturn(512)` |
| **Ignore a call** | `LED_Toggle_Ignore()` |
| **Expect N times** | `Timer_Start_ExpectNTimes(3)` |
| **Verify not called** | Enforced automatically if `_Expect` was never set |

## Example
```c
#include "mock_uart_driver.h"

void test_send_byte_calls_uart(void) {
    UART_Send_Expect('A');   // expect exactly one call with 'A'
    send_byte('A');          // function under test
}                            // CMock verifies at teardown
```

## Key Takeaways
- ✅ No physical hardware needed to run tests
- ✅ Strict call verification — unexpected calls **fail the test**
- ✅ Generated mocks stay in sync when headers change (just re-run Ceedling)
- ✅ Enables true unit isolation of embedded firmware modules
