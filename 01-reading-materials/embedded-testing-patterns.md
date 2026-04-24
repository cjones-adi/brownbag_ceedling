# 🏭 Embedded Testing Patterns

Common patterns you will encounter when writing unit tests for embedded firmware. Each pattern includes the problem it solves, the CMock/Unity mechanics, and a concrete code example.

---

## Pattern 1 — Volatile Register / Flag Polling (Call Sequencing)

### The Problem
A function reads the same hardware register or status flag **more than once** in a single call — for example, polling a DMA completion flag inside a loop. The compiler (and your mock) sees two separate calls; each needs to return a different value to exercise the full path.

### How CMock Handles It
CMock queues `_ExpectAndReturn` calls **in order**. Each call to the mock consumes the next entry in the queue, so you can model each read independently.

### Real-World Example — Waiting for DMA Completion
```c
// src/dma_transfer.c
#include "dma_driver.h"

void dma_wait_for_complete(void)
{
    while (!DMA_IsComplete())
    {
        /* spin — production code may add a timeout here */
    }
}
```

```c
// test/test_dma_transfer.c
#include "unity.h"
#include "mock_dma_driver.h"
#include "dma_transfer.h"

void setUp(void)    {}
void tearDown(void) {}

void test_dma_wait_polls_until_complete(void)
{
    // First read  → DMA not done yet  (loop iterates)
    DMA_IsComplete_ExpectAndReturn(false);

    // Second read → DMA now complete  (loop exits)
    DMA_IsComplete_ExpectAndReturn(true);

    dma_wait_for_complete();  // CMock verifies both calls were made, in order
}
```

> **Key point:** If the production code calls `DMA_IsComplete()` a third time unexpectedly, CMock will **fail the test** — the queue is exhausted.

### Variant — ADC Busy-Wait
```c
// Queue: NOT_READY → NOT_READY → READY
ADC_GetStatus_ExpectAndReturn(ADC_NOT_READY);
ADC_GetStatus_ExpectAndReturn(ADC_NOT_READY);
ADC_GetStatus_ExpectAndReturn(ADC_READY);
adc_wait_for_conversion();
```

---

## Pattern 2 — Hardware Abstraction Layer (HAL) Isolation

### The Problem
Business logic is tangled with hardware calls, making it untestable without real silicon.

### Solution
Separate logic from hardware behind a HAL interface. Test the logic by mocking the HAL.

```c
// src/temperature_monitor.c
#include "adc_hal.h"
#include "temperature_monitor.h"

bool temperature_is_critical(void)
{
    uint16_t raw = ADC_ReadChannel(TEMP_CHANNEL);
    return raw > CRITICAL_THRESHOLD;
}
```

```c
void test_temperature_is_critical_when_above_threshold(void)
{
    ADC_ReadChannel_ExpectAndReturn(TEMP_CHANNEL, CRITICAL_THRESHOLD + 1);
    TEST_ASSERT_TRUE(temperature_is_critical());
}

void test_temperature_is_not_critical_when_below_threshold(void)
{
    ADC_ReadChannel_ExpectAndReturn(TEMP_CHANNEL, CRITICAL_THRESHOLD - 1);
    TEST_ASSERT_FALSE(temperature_is_critical());
}
```

---

## Pattern 3 — State Machine Transition Testing

### The Problem
State machines have many valid and invalid transitions. Manual testing on hardware only covers the happy path.

### Solution
Drive the state machine through each transition in isolation, verifying outputs at each step.

```c
// Test: IDLE → RUNNING on START_CMD, output pin driven HIGH
void test_state_machine_transitions_to_running_on_start(void)
{
    sm_init();                                   // put SM in IDLE
    GPIO_SetPin_Expect(MOTOR_ENABLE_PIN, HIGH);  // expect output side-effect

    sm_process_event(START_CMD);

    TEST_ASSERT_EQUAL(STATE_RUNNING, sm_get_state());
}

// Test: invalid transition is ignored
void test_state_machine_ignores_start_when_already_running(void)
{
    sm_init();
    sm_process_event(START_CMD);         // get to RUNNING
    // no GPIO_SetPin_Expect — no output should occur

    sm_process_event(START_CMD);         // duplicate START — should be a no-op

    TEST_ASSERT_EQUAL(STATE_RUNNING, sm_get_state());
}
```

---

## Pattern 4 — Error / Fault Path Injection

### The Problem
Hardware functions can fail (I2C NAK, SPI timeout, CRC error). These error paths are nearly impossible to trigger on real hardware reliably, yet they contain some of the most critical code.

### Solution
Use `_ExpectAndReturn` to inject failure return codes directly.

```c
void test_sensor_read_returns_error_on_i2c_failure(void)
{
    I2C_Transmit_ExpectAndReturn(SENSOR_ADDR, CMD_READ, I2C_ERROR_NACK);

    SensorResult_t result = sensor_read_temperature();

    TEST_ASSERT_EQUAL(SENSOR_ERR_COMM, result.status);
}
```

---

## Pattern 5 — Callback / Function Pointer Testing

### The Problem
Interrupt service routines and completion callbacks use function pointers that are hard to trigger in a test environment.

### Solution
Capture the registered callback and invoke it directly in the test.

```c
// Production code registers a callback with a driver
static transfer_complete_cb_t registered_cb = NULL;

void DMA_RegisterCallback(transfer_complete_cb_t cb)  // real function under test
{
    registered_cb = cb;
}
```

```c
// CMock captures the argument passed to the mock
static transfer_complete_cb_t captured_cb = NULL;

void DMA_RegisterCallback_StubWithCallback(transfer_complete_cb_t cb,
                                           int num_calls)
{
    captured_cb = cb;   // save it
}

void test_dma_callback_triggers_state_update(void)
{
    DMA_RegisterCallback_StubWithCallback(DMA_RegisterCallback_StubWithCallback, 1);

    dma_init();                  // registers its callback
    captured_cb(DMA_SUCCESS);    // fire it manually, as if IRQ fired

    TEST_ASSERT_EQUAL(STATE_TRANSFER_DONE, dma_get_state());
}
```

---

## Pattern 6 — Boundary / Limit Testing

### The Problem
Off-by-one errors and limit violations are the most common bugs in embedded arithmetic and buffer handling.

### Solution
Always test: **below minimum**, **at minimum**, **at maximum**, **above maximum**.

```c
void test_clamp_returns_min_when_input_is_below_range(void)
{
    TEST_ASSERT_EQUAL_INT(0,   clamp(-1,  0, 100));
}

void test_clamp_returns_min_when_input_equals_min(void)
{
    TEST_ASSERT_EQUAL_INT(0,   clamp(0,   0, 100));
}

void test_clamp_returns_value_when_input_is_in_range(void)
{
    TEST_ASSERT_EQUAL_INT(50,  clamp(50,  0, 100));
}

void test_clamp_returns_max_when_input_equals_max(void)
{
    TEST_ASSERT_EQUAL_INT(100, clamp(100, 0, 100));
}

void test_clamp_returns_max_when_input_is_above_range(void)
{
    TEST_ASSERT_EQUAL_INT(100, clamp(101, 0, 100));
}
```

---

## Pattern 7 — Timeout / Retry Loop Testing

### The Problem
Loops that give up after N retries or a deadline need both the "success before timeout" and "fail after timeout" paths tested.

### Solution
Queue enough mock returns to exhaust the retry count, then verify the timeout error is returned.

```c
#define MAX_RETRIES 3

// Test: succeeds on the second attempt
void test_connect_succeeds_on_second_attempt(void)
{
    UART_Connect_ExpectAndReturn(UART_BUSY);   // attempt 1 — busy
    UART_Connect_ExpectAndReturn(UART_OK);     // attempt 2 — success

    TEST_ASSERT_EQUAL(STATUS_OK, connect_with_retry());
}

// Test: all retries exhausted → timeout error
void test_connect_returns_timeout_after_max_retries(void)
{
    UART_Connect_ExpectAndReturn(UART_BUSY);   // attempt 1
    UART_Connect_ExpectAndReturn(UART_BUSY);   // attempt 2
    UART_Connect_ExpectAndReturn(UART_BUSY);   // attempt 3 — MAX_RETRIES hit

    TEST_ASSERT_EQUAL(STATUS_TIMEOUT, connect_with_retry());
}
```

---

## Quick Reference — Choosing the Right CMock Pattern

| Scenario | CMock Mechanic |
|----------|---------------|
| Single call, fixed return value | `Func_ExpectAndReturn(val)` |
| Same function called N times, same return | `Func_ExpectAndReturn` × N (or `_IgnoreAndReturn`) |
| Same function, **different return per call** | Queue multiple `_ExpectAndReturn` — consumed in order |
| Call expected but return value irrelevant | `Func_Expect(args)` |
| Call should never happen | Don't set expectation — unexpected calls auto-fail |
| Call may or may not happen | `Func_IgnoreAndReturn(val)` |
| Need to inspect arguments passed in | `Func_StubWithCallback(my_stub)` |
