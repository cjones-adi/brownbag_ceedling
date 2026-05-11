# 🎭 Demo 2: Mocking Hardware – I2C Temperature Sensor

**Total Time: 45 minutes**
**Modules under test:** `temp_sensor` (read path) via a mocked `i2c_hal`

---

## 📝 Opening Hook (3 min)

```
"Raise your hand if this has happened to you:"
  - PCB delayed 6 weeks, software deadline is next week
  - Sensor reads fine on the bench, fails randomly in the field
  - Need to test I2C timeout handling but you can't unplug the cable
    fast enough while the code is running

"Today: we write a complete sensor driver test suite –
 including fault injection – without touching a MAX31889 or any I2C bus."
```

---

## 🎯 What We Are Building (3 min)

The `temp_sensor` read path from the safety charging demo.

In production, `temp_sensor_read()` talks to the MAX31889 temperature sensor
over I2C using the MaximSDK driver. In the test build (when `TEST` is defined),
it calls `i2c_hal_write_read()` instead — a thin HAL that CMock can replace
with a fully controllable fake.

```
Production build:        Test build:
  temp_sensor_read()       temp_sensor_read()
       │                        │
  MAX31889 SDK driver      i2c_hal_write_read()   ← CMock replaces this
       │                        │
  Real I2C hardware         fake callback we write
```

---

## 🧠 Mocking Concepts (5 min)

### What is a Mock?
```
A mock is a generated fake of a hardware interface that:
  ✅ Replaces real hardware calls in the test build
  ✅ Lets the test inject any return value or byte payload
  ✅ Verifies that the production code called the right functions
  ✅ Fails the test automatically if an unexpected call is made
```

### The HAL we will mock — `i2c_hal.h`
```c
// Return codes
#define I2C_SUCCESS    0
#define I2C_TIMEOUT   -1
#define I2C_BUS_HUNG  -2

// Combined write-then-read (mirrors what MAX31889 read needs)
int i2c_hal_write_read(uint8_t dev_addr,
                       const uint8_t *tx, uint8_t tx_len,
                       uint8_t *rx,       uint8_t rx_len);

// Bus recovery after BUS_HUNG
int i2c_hal_reset(void);
```

CMock reads this header and auto-generates `mock_i2c_hal.h / mock_i2c_hal.c`.
No manual work — Ceedling triggers generation when it sees `#include "mock_i2c_hal.h"`
in the test file.

---

## 🔍 Walk Through the Test File (10 min)

Open `test/test_temp_sensor.c`. Show the three sections:

```
SCENARIO 1 – overflow bug demo  (covered in Demo 4)
SCENARIO 2 – simulated hardware, no board required  ← today
SCENARIO 3 – simulated I2C bus-hung + recovery      ← today
```

### setUp / tearDown
```c
void setUp(void)
{
    mock_i2c_hal_Init();      // reset CMock tracking state
}

void tearDown(void)
{
    mock_i2c_hal_Verify();    // assert all expectations were met
    mock_i2c_hal_Destroy();   // free CMock internals
}
```

### The fake callback for a 25 °C reading
```c
// MAX31889 encoding: temp_millideg = raw * 5
// 25 °C  →  raw = 5000 = 0x1388  →  rx = {0x13, 0x88}
static int fake_i2c_read_25C(uint8_t dev_addr,
                              const uint8_t *tx,  uint8_t tx_len,
                              uint8_t       *rx,  uint8_t rx_len,
                              int            num_calls)
{
    rx[0] = 0x13u;   // MSB
    rx[1] = 0x88u;   // LSB
    return I2C_SUCCESS;
}
```

**Ask:** "Where does this 0x1388 come from?"

Walk through the sensor encoding math:
```
25 °C × (1 / 0.005) = 5000 raw counts
5000 decimal = 0x1388
Big-endian 2-byte: rx[0]=0x13, rx[1]=0x88
Verify: (0x13 << 8) | 0x88 = 5000
        5000 * 5 = 25000 millideg = 25 °C  ✓
```

---

## 🧪 Scenario 2a — Positive Temperature Parse (5 min)

```c
void test_temp_sensor_read_parses_positive_temperature_correctly(void)
{
    // Arrange – install the fake callback
    i2c_hal_write_read_StubWithCallback(fake_i2c_read_25C);

    // Act
    int32_t temp = 0;
    int ret = temp_sensor_read(&temp);

    // Assert
    TEST_ASSERT_EQUAL_INT(0, ret);
    TEST_ASSERT_EQUAL_INT32(25000, temp);   // 25 °C in millidegrees
}
```

Run it:
```bash
ceedling test:path[test_temp_sensor]
```

**Say:** "No hardware. No cable. No board. We just proved the driver decodes the
MAX31889 byte format correctly."

---

## 🧪 Scenario 2b — I2C Timeout (5 min)

```c
static int fake_i2c_timeout(...) { return I2C_TIMEOUT; }

void test_temp_sensor_read_handles_i2c_timeout_safely(void)
{
    i2c_hal_write_read_StubWithCallback(fake_i2c_timeout);
    // No i2c_hal_reset_Expect() -- an accidental reset() call FAILS the test

    int32_t temp = 99999;   // sentinel – must NOT be modified
    int ret = temp_sensor_read(&temp);

    TEST_ASSERT_EQUAL_INT(I2C_TIMEOUT, ret);
    TEST_ASSERT_EQUAL_INT32(99999, temp);   // output untouched
}
```

**Say:** "We just tested the disconnect scenario — unplugged sensor, broken cable —
without physically touching the hardware. And we verified that `i2c_hal_reset()`
was NOT called, which is correct behaviour for a simple timeout."

---

## 🧪 Scenario 3 — I2C Bus Hung + Recovery (5 min)

```c
static int fake_i2c_bus_hung(...) { return I2C_BUS_HUNG; }

void test_temp_sensor_read_calls_reset_on_bus_hung(void)
{
    i2c_hal_write_read_StubWithCallback(fake_i2c_bus_hung);

    // Strict expectation: reset() called exactly once
    i2c_hal_reset_ExpectAndReturn(I2C_SUCCESS);

    int32_t temp = 99999;   // sentinel
    int ret = temp_sensor_read(&temp);

    TEST_ASSERT_EQUAL_INT(I2C_BUS_HUNG, ret);
    TEST_ASSERT_EQUAL_INT32(99999, temp);
}
```

**Say:** "We just tested the 'SDA stuck low' scenario — normally you would have to
physically short-circuit the I2C bus on the board to hit this path. With CMock we
inject it in one line. And `mock_i2c_hal_Verify()` in tearDown confirms reset()
was called exactly once — not zero times, not twice."

---

## ✅ Run Full Suite (1 min)

```bash
ceedling test:all
```

All scenarios should pass.

---

## 🎯 Key Takeaways

- ✅ **CMock generates the mock from the header** — no manual stub writing
- ✅ **StubWithCallback** injects any byte payload or return code you need
- ✅ **ExpectAndReturn** enforces strict call counts — extra or missing calls fail the test
- ✅ **Sentinel pattern** (`temp = 99999`) proves output is not modified on error
- ✅ **tearDown Verify()** is the silent gatekeeper — catches unsatisfied expectations

---

**Next Demo:** We use TDD to build the safety decision logic (Temp > 40 °C AND Voltage ≥ 4.2 V)
test-first, producing the `test_safety_logic_*` tests from scratch.
