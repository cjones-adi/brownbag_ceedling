# 🔗 Demo 5: Integration Testing – Full Safety Charging Pipeline

**Total Time: 45 minutes**
**Modules:** `led_control` + `temp_sensor` + `adc_monitor` — end-to-end

---

## 📝 Opening (3 min)

```
"Show of hands: How many times have you seen this?"

  Dev A: "My LED module unit tests all pass!"
  Dev B: "My temperature sensor unit tests all pass!"
  Dev C: "My ADC monitor unit tests all pass!"
  Integration: "When we put them all together, the safety logic doesn't fire."

What unit tests individually miss:
  - Data format agreements between modules (millideg? raw counts? degrees?)
  - Error propagation — does an I2C failure prevent the LED from updating?
  - The combined condition — both temperature AND voltage must be read correctly
  - The sequence — what if adc_monitor_init() is never called?
```

---

## 🏗️ The Full System Architecture (3 min)

```
┌─────────────────┐    ┌─────────────────┐    ┌───────────────────┐
│  temp_sensor    │    │  adc_monitor    │    │   led_control     │
│                 │    │                 │    │                   │
│ temp_sensor_    │    │ adc_monitor_    │    │ red_led_on()      │
│   read(mdeg)    │    │   read_voltage  │    │ green_led_on()    │
│                 │    │   (mV)          │    │ led_control_get() │
└────────┬────────┘    └────────┬────────┘    └────────┬──────────┘
         │                      │                      │
         └──────────────────────┴──────────────────────┘
                                │
                         Safety Logic
                    (temperature > 40000
                     AND voltage >= 4200)
                                │
                         Green LED = ON
                      ("Stop Charging" signal)
```

In `main.c`, this loop runs continuously.
In integration tests, we drive the same logic without a board.

---

## 🎯 What Integration Tests Add (3 min)

| Question | Unit Tests | Integration Tests |
|---|---|---|
| Does `led_control` set/clear correctly? | ✅ | ✅ |
| Does `temp_sensor` decode bytes correctly? | ✅ | ✅ |
| Does an I2C error prevent the LED from turning on? | ❌ | ✅ |
| Does the safety logic fire with real module outputs? | ❌ | ✅ |
| Does the system stay safe if `adc_monitor_init()` fails? | ❌ | ✅ |
| Is the data unit agreement correct (millideg, mV)? | ❌ | ✅ |

---

## 🔍 Review the module interfaces before writing tests (5 min)

```c
// temp_sensor.h
int temp_sensor_init(void);
int temp_sensor_read(int32_t *temp_out);   // millidegrees C

// adc_monitor.h
int adc_monitor_init(void);
int adc_monitor_read_voltage(uint32_t *voltage_out);  // millivolts

// led_control.h
int  led_control_init(void);
int  red_led_on(void);
int  green_led_on(void);
bool led_control_get(led_color_t color);
```

Point out the unit agreement:
- Temperature: `int32_t` millidegrees — threshold is `40000`
- Voltage: `uint32_t` millivolts — threshold is `4200`
- Same units used everywhere: in the sensor API, in the tests, in `main.c`

---

## 🧪 Integration Test: Happy Path — Below Threshold (5 min)

Create `test/test_safety_pipeline.c`:

```c
#include "unity.h"
#include "led_control.h"
#include "mock_i2c_hal.h"    // CMock: controls temp_sensor's I2C calls
#include "temp_sensor.h"
#include "adc_monitor.h"     // Note: adc_monitor uses real MXC ADC in fw build;
                              //       in TEST build we supply a stub or mock

/* MAX31889 encoding for 25 °C: raw=5000=0x1388, millideg=25000 */
static int fake_temp_25C(uint8_t dev_addr,
                          const uint8_t *tx, uint8_t tx_len,
                          uint8_t *rx, uint8_t rx_len, int n)
{
    rx[0] = 0x13u; rx[1] = 0x88u;
    return I2C_SUCCESS;
}

void setUp(void)
{
    mock_i2c_hal_Init();
    led_control_init();
    temp_sensor_init();
}

void tearDown(void)
{
    mock_i2c_hal_Verify();
    mock_i2c_hal_Destroy();
}

void test_pipeline_no_alarm_when_both_below_threshold(void)
{
    // Arrange
    i2c_hal_write_read_StubWithCallback(fake_temp_25C);  // 25 °C

    // Act – replicate the main.c loop
    int32_t  temp_mdeg = 0;
    uint32_t volt_mv   = 3900;   // 3.9 V – below threshold

    red_led_on();
    temp_sensor_read(&temp_mdeg);

    if (temp_mdeg > 40000 && volt_mv >= 4200) {
        green_led_on();
    }

    // Assert
    TEST_ASSERT_TRUE(led_control_get(LED_COLOR_RED));
    TEST_ASSERT_FALSE(led_control_get(LED_COLOR_GREEN));
    TEST_ASSERT_EQUAL_INT32(25000, temp_mdeg);
}
```

```bash
ceedling test:path[test_safety_pipeline]
```

---

## 🧪 Integration Test: Safety Fires — Both Thresholds (5 min)

```c
/* MAX31889 encoding for 41 °C: raw=8200=0x2008, millideg=41000 */
static int fake_temp_41C(uint8_t dev_addr,
                          const uint8_t *tx, uint8_t tx_len,
                          uint8_t *rx, uint8_t rx_len, int n)
{
    rx[0] = 0x20u; rx[1] = 0x08u;
    return I2C_SUCCESS;
}

void test_pipeline_alarm_fires_when_both_thresholds_exceeded(void)
{
    // Arrange
    i2c_hal_write_read_StubWithCallback(fake_temp_41C);  // 41 °C

    // Act
    int32_t  temp_mdeg = 0;
    uint32_t volt_mv   = 4200;   // exactly at voltage threshold (>= triggers)

    red_led_on();
    temp_sensor_read(&temp_mdeg);

    if (temp_mdeg > 40000 && volt_mv >= 4200) {
        green_led_on();
    }

    // Assert
    TEST_ASSERT_TRUE(led_control_get(LED_COLOR_RED));
    TEST_ASSERT_TRUE(led_control_get(LED_COLOR_GREEN));   // stop charging
    TEST_ASSERT_EQUAL_INT32(41000, temp_mdeg);
}
```

**Say:** "We just drove the entire pipeline: I2C bytes → millideg → safety condition → LED state.
No hardware. This is what `main.c` does on the board — we proved it works before flashing."

---

## 🧪 Integration Test: I2C Error Does Not Set Green LED (5 min)

```c
void test_pipeline_i2c_error_leaves_green_led_off(void)
{
    // Arrange – sensor unreachable
    i2c_hal_write_read_StubWithCallback(/* fake_i2c_timeout from demo 2 */ NULL);
    // Use ExpectAndReturn for clarity:
    i2c_hal_write_read_ExpectAnyArgsAndReturn(I2C_TIMEOUT);

    // Act
    int32_t  temp_mdeg = 99999;   // sentinel
    uint32_t volt_mv   = 4500;    // voltage is above threshold

    red_led_on();
    int ret = temp_sensor_read(&temp_mdeg);

    // Only apply safety logic if read succeeded
    if (ret == 0 && temp_mdeg > 40000 && volt_mv >= 4200) {
        green_led_on();
    }

    // Assert – I2C error must not cause a false alarm
    TEST_ASSERT_EQUAL_INT(I2C_TIMEOUT, ret);
    TEST_ASSERT_TRUE(led_control_get(LED_COLOR_RED));
    TEST_ASSERT_FALSE(led_control_get(LED_COLOR_GREEN));  // no false alarm
    TEST_ASSERT_EQUAL_INT32(99999, temp_mdeg);            // output untouched
}
```

**Say:** "Unit tests proved the sensor handles timeout correctly. Integration tests prove
the *system* does the right thing with that timeout — it does not misfire the alarm."

---

## ✅ Run Full Suite (1 min)

```bash
ceedling test:all
```

All unit + integration tests pass.

Optionally show coverage:
```bash
ceedling gcov:all
# Open build/artifacts/gcov/index.html
```

---

## 🏁 Demo Series Wrap-Up (3 min)

| Demo | What We Learned |
|------|----------------|
| **01 Simple Start** | Tests run on the host in milliseconds with no hardware |
| **02 Mocking Hardware** | CMock replaces real I2C so any response can be injected |
| **03 TDD** | Tests written first become living requirements |
| **04 Boundary Conditions** | Exact threshold semantics and silent integer overflow caught by tests |
| **05 Integration** | End-to-end pipeline validation before the board is touched |

**Closing:**
```
"The full test suite for this safety charging demo runs in under a second.
 On the board, a single flash + observe cycle takes 30–60 seconds.
 For 50 test cases that is a 25–50× speed difference.

 More importantly: the I2C bus-hung scenario, the 202 °C overflow,
 and the 40000 millideg exact boundary cannot be reliably triggered
 on hardware at all.  Unit and integration tests are the only way to
 prove those paths work."
```

---

## �� Key Takeaways

- ✅ **Integration tests validate cross-module data agreements** — units, formats, return codes
- ✅ **Error propagation belongs in integration tests** — does the system respond correctly when one module fails?
- ✅ **The full Ceedling stack** — Unity assertions + CMock mocks + gcov coverage — runs on any CI server
- ✅ **Proceed to the hands-on exercises** in `03-hands-on-exercises/` to apply these patterns yourself
