# ⚠️ Demo 4: Boundary Conditions & Integer Overflow

**Total Time: 45 minutes**
**Modules under test:** Safety threshold boundaries + `temp_sensor_convert_raw` overflow bug

---

## 📝 Opening (3 min)

```
"From Demo 3, our safety condition is:
   temperature > 40000  AND  voltage >= 4200

Question: What happens at temperature = 40000 exactly?
          What about 39999? 40001?

And: The MAX31889 ADC raw value gets converted using a formula
     from the Linux kernel lt8460 driver.  That formula has a bug.
     The bug lets a 202 °C battery read as 5 °C.
     A unit test catches it.  The field doesn't get a chance to."
```

---

## 🎯 Two Topics Today (2 min)

**Topic A — Threshold boundary tests**
Test the exact edges of the `temperature > 40000` and `voltage >= 4200` conditions.

**Topic B — Integer overflow bug (convert_raw)**
Reproduce a real-world truncation bug from `lt8460_get_temp_channel_temp()` in the Linux IIO subsystem, embedded inside `temp_sensor_convert_raw()`.

---

## 📐 Topic A: Boundary Value Analysis (8 min)

### Theory — The Three Values to Test at Every Boundary

```
For condition  x > N:
  Test  N - 1  → should NOT trigger  (just below)
  Test  N      → should NOT trigger  (exactly at – '>' not '>=')
  Test  N + 1  → SHOULD trigger      (just above)

For condition  x >= N:
  Test  N - 1  → should NOT trigger
  Test  N      → SHOULD trigger      (exactly at – '>=' includes N)
  Test  N + 1  → SHOULD trigger
```

### Temperature threshold  (> 40000 millideg)

Open `test/test_led_control.c` and walk through the safety tests from Demo 3.
Then add the boundary precision tests:

```c
// Exactly AT threshold – must NOT trigger (> not >=)
void test_safety_logic_temp_exactly_at_threshold_does_not_trigger(void)
{
    int32_t  temperature = 40000;  // exactly 40 °C
    uint32_t voltage     = 4200;   // voltage also at threshold

    led_control_init();
    red_led_on();

    if (temperature > 40000 && voltage >= 4200) {
        green_led_on();
    }

    TEST_ASSERT_FALSE(led_control_get(LED_COLOR_GREEN));  // > not >=
}

// One millidegree above – MUST trigger
void test_safety_logic_temp_one_unit_above_threshold_triggers(void)
{
    int32_t  temperature = 40001;  // 40.001 °C
    uint32_t voltage     = 4200;

    led_control_init();
    red_led_on();

    if (temperature > 40000 && voltage >= 4200) {
        green_led_on();
    }

    TEST_ASSERT_TRUE(led_control_get(LED_COLOR_GREEN));
}
```

### Voltage threshold  (>= 4200 mV)

```c
// Exactly AT threshold – MUST trigger (>= includes 4200)
void test_safety_logic_voltage_exactly_at_threshold_triggers(void)
{
    int32_t  temperature = 40001;
    uint32_t voltage     = 4200;   // exactly 4.2 V

    led_control_init();
    red_led_on();

    if (temperature > 40000 && voltage >= 4200) {
        green_led_on();
    }

    TEST_ASSERT_TRUE(led_control_get(LED_COLOR_GREEN));
}

// One millivolt below – must NOT trigger
void test_safety_logic_voltage_one_unit_below_threshold_does_not_trigger(void)
{
    int32_t  temperature = 40001;
    uint32_t voltage     = 4199;   // 4.199 V

    led_control_init();
    red_led_on();

    if (temperature > 40000 && voltage >= 4200) {
        green_led_on();
    }

    TEST_ASSERT_FALSE(led_control_get(LED_COLOR_GREEN));
}
```

Run:
```bash
ceedling test:path[test_led_control]
```

**Say:** "These tests define the exact operator semantics. If someone changes `>` to `>=`
for temperature, `test_safety_logic_temp_exactly_at_threshold_does_not_trigger` fails
immediately — before the code ever reaches hardware."

---

## 🐛 Topic B: Integer Overflow Bug in convert_raw (15 min)

### The Bug's Origin

The formula in `temp_sensor_convert_raw()` replicates `lt8460_get_temp_channel_temp()`
from the Linux kernel IIO subsystem. The original bug:

```c
// ORIGINAL BUG (Linux lt8460):
int32_t temp = ((int32_t)raw * 1200000 / 255 - 554250) * 1000 / 1910;
*value = (uint16_t)temp;   // ← silently truncates bits [31:16]
```

A `uint16_t` cast on an `int32_t` keeps only the lower 16 bits.
For values outside –32 768 to +32 767 millideg the result is wrong — silently.

### The Bug Reproduced in Our Code

Open `temp_sensor.h` and show the toggle:
```c
// Toggle with:  #define TEMP_CONVERT_BUG_ENABLED   (uncomment = bug, comment = fix)
```

Open `temp_sensor.c` and show both versions:
```c
// BUG version
int temp_sensor_convert_raw(uint8_t raw, int16_t *value_millideg)
{
    int32_t temp = ((int32_t)raw * 1200000 / 255 - 554250) * MILLI / 1910;
    *value_millideg = (int16_t)(uint16_t)temp;   // ← BUG line
    return 0;
}

// FIX version
int temp_sensor_convert_raw(uint8_t raw, int32_t *value_millideg)
{
    *value_millideg = ((int32_t)raw * 1200000 / 255 - 554250) * MILLI / 1910;
    return 0;
}
```

### The Overflow Lookup Table

| raw | int32_t result | BUG int16_t stored | Error |
|-----|---------------:|-------------------:|-------|
| 0   | −290 183 mdeg (−290 °C) | −28 040 (−28 °C) | +262 °C wrong |
| 128 | 25 184 mdeg (25 °C) | 25 184 (25 °C) | no overflow |
| 200 | 202 579 mdeg (202 °C) | 5 971 (6 °C) | −196 °C wrong |

**The safety consequence:** raw=200 represents a 202 °C battery. The bug stores 6 °C.
The `> 40 °C` threshold is never crossed. Charging is never stopped. The battery overheats.

### Running the Bug Tests

Enable the bug (`#define TEMP_CONVERT_BUG_ENABLED` uncommented):
```bash
ceedling test:path[test_temp_sensor]
```

The bug tests pass — they document the wrong output:
```c
void test_convert_raw_demonstrates_overflow_value(void)
{
    int16_t result = 0;
    temp_sensor_convert_raw(200, &result);
    TEST_ASSERT_EQUAL_INT16(5971, result);   // overflow value — expected with bug
}

void test_convert_raw_overflow_safety_check_fails_to_trigger(void)
{
    int16_t stored_millideg = 0;
    temp_sensor_convert_raw(200, &stored_millideg);
    int deg_C = (int32_t)stored_millideg / 1000;
    TEST_ASSERT_EQUAL_INT(5, deg_C);           // BUG: 202 °C appears as 5 °C
    TEST_ASSERT_LESS_THAN_INT(40, deg_C);      // safety threshold does NOT fire
}
```

Now comment out `#define TEMP_CONVERT_BUG_ENABLED` (fix active):
```bash
ceedling test:path[test_temp_sensor]
```

The fix tests pass — full range preserved:
```c
void test_convert_raw_full_range_preserved_by_fix(void)
{
    int32_t result = 0;
    temp_sensor_convert_raw(200, &result);
    TEST_ASSERT_EQUAL_INT32(202579, result);   // correct — no truncation
}

void test_convert_raw_fix_correctly_triggers_safety_threshold(void)
{
    int32_t stored_millideg = 0;
    temp_sensor_convert_raw(200, &stored_millideg);
    int deg_C = stored_millideg / 1000;
    TEST_ASSERT_EQUAL_INT(202, deg_C);
    TEST_ASSERT_GREATER_OR_EQUAL_INT(40, deg_C);  // safety threshold fires correctly
}
```

**Say:** "This is what the unit test catches and hardware testing misses. You will
never accidentally set raw=200 on the bench. The test vector hits this in milliseconds."

---

## ✅ Run Full Suite (1 min)

```bash
ceedling test:all
```

All boundary and overflow tests pass (with fix active).

---

## 🎯 Key Takeaways

- ✅ **Test `>` vs `>=` explicitly** — exact threshold semantics matter for safety
- ✅ **Boundary triples**: just below, exactly at, just above — cover every decision point
- ✅ **Integer overflow is silent** — a `(uint16_t)` cast never warns you; a test does
- ✅ **Test vectors reach states hardware testing cannot** — raw=200 → 202 °C is not easy to reproduce physically
- ✅ **Overflow bugs in third-party code are testable** — copy the formula, wrap it in a function, test it

---

**Next Demo:** All three modules (`led_control`, `temp_sensor`, `adc_monitor`) are combined
into a single integration test that validates the full safety charging pipeline end-to-end.
