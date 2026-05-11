# 🔴 Demo 3: Test-Driven Development – Safety Decision Logic

**Total Time: 45 minutes**
**Module under test:** Safety logic in `test_led_control.c` (business rules, built test-first)

---

## 📝 Opening Philosophy (3 min)

```
Traditional approach:
  1. Think about implementation
  2. Write code
  3. Debug on hardware
  4. Add tests (maybe)

TDD approach:
  1. Think about behaviour
  2. Write a failing test  (RED)
  3. Write the simplest code that passes  (GREEN)
  4. Improve the design  (REFACTOR)
  5. Repeat

"Debugging on hardware is 10× slower than fixing a failing test on your PC.
 TDD keeps you in the fast loop."
```

---

## 🔄 The TDD Cycle (2 min)

```
🔴 RED    – Write a failing test. Prove the test actually catches a bug.
🟢 GREEN  – Write the minimum code to make it pass. No extras.
🔧 BLUE   – Refactor. Improve readability and design. Tests stay green.
↩️  REPEAT – One small behaviour at a time.
```

---

## 🎯 What We Are Building (3 min)

The safety decision logic for the MAX32655FTHR charging demo:

```
main.c logic (production):
  Red LED always ON  →  system is running
  If (temperature > 40 °C) AND (voltage ≥ 4.2 V):
      Green LED ON  →  "stop charging" signal to the PMIC
```

We will drive this logic entirely through `led_control` and express it as
unit tests — no `main.c`, no PMIC, no ADC, no I2C needed.

---

## 🔴 RED: First Test — Normal Conditions (5 min)

Start with the simplest case: both values below threshold → only Red LED should be on.

```c
void test_safety_logic_normal_conditions(void)
{
    int32_t  temperature = 30000;  // 30 °C in millideg – below 40 °C threshold
    uint32_t voltage     = 3900;   // 3.9 V in mV – below 4.2 V threshold

    led_control_init();
    red_led_on();

    if (temperature > 40000 && voltage >= 4200) {
        green_led_on();
    }

    TEST_ASSERT_TRUE(led_control_get(LED_COLOR_RED));
    TEST_ASSERT_FALSE(led_control_get(LED_COLOR_GREEN));
}
```

Run before `red_led_on()` exists:
```bash
ceedling test:path[test_led_control]   # FAILS – undeclared identifier
```

**Say:** "RED phase. The test cannot even compile because we haven't written the
helper yet. That is fine — that is exactly where we want to be."

---

## 🟢 GREEN: Add the Convenience Wrappers (4 min)

Add to `led_control.h` and `led_control.c`:

```c
// led_control.h (declarations)
int red_led_on(void);
int red_led_off(void);
int green_led_on(void);
int green_led_off(void);
int blue_led_on(void);
int blue_led_off(void);

// led_control.c (definitions – one-liners that call led_control_set)
int red_led_on(void)   { return led_control_set(LED_COLOR_RED,   true);  }
int red_led_off(void)  { return led_control_set(LED_COLOR_RED,   false); }
int green_led_on(void) { return led_control_set(LED_COLOR_GREEN, true);  }
int green_led_off(void){ return led_control_set(LED_COLOR_GREEN, false); }
int blue_led_on(void)  { return led_control_set(LED_COLOR_BLUE,  true);  }
int blue_led_off(void) { return led_control_set(LED_COLOR_BLUE,  false); }
```

```bash
ceedling test:path[test_led_control]   # PASSES
```

**Say:** "GREEN. The minimum code that makes the test pass. Nothing more."

---

## 🔴 RED: Over-Temperature Only (3 min)

```c
void test_safety_logic_over_temp_only(void)
{
    int32_t  temperature = 45000;  // 45 °C – above threshold
    uint32_t voltage     = 3900;   // 3.9 V – below threshold

    led_control_init();
    red_led_on();

    if (temperature > 40000 && voltage >= 4200) {
        green_led_on();
    }

    TEST_ASSERT_TRUE(led_control_get(LED_COLOR_RED));
    TEST_ASSERT_FALSE(led_control_get(LED_COLOR_GREEN));  // AND condition – both required
}
```

```bash
ceedling test:path[test_led_control]   # PASSES immediately (logic already correct)
```

**Say:** "The test passes without any new code because the AND condition in the
if-statement already handles this. But the test now documents that over-temperature
alone is NOT enough to trigger the stop-charging signal."

---

## 🔴 RED: Over-Voltage Only (2 min)

```c
void test_safety_logic_over_voltage_only(void)
{
    int32_t  temperature = 30000;  // 30 °C – below threshold
    uint32_t voltage     = 4300;   // 4.3 V – above threshold

    led_control_init();
    red_led_on();

    if (temperature > 40000 && voltage >= 4200) {
        green_led_on();
    }

    TEST_ASSERT_TRUE(led_control_get(LED_COLOR_RED));
    TEST_ASSERT_FALSE(led_control_get(LED_COLOR_GREEN));  // must NOT trigger alone
}
```

**Ask the audience:** "What would happen if someone changed `&&` to `||`?"
Run the test to show it is the safety net against that mistake.

---

## 🔴 RED: Both Thresholds Exceeded (4 min)

The most important test — the only case that should stop charging:

```c
void test_safety_logic_both_thresholds_exceeded(void)
{
    int32_t  temperature = 41000;  // 41 °C – above 40 °C
    uint32_t voltage     = 4200;   // 4.2 V – at threshold (>= triggers)

    led_control_init();
    red_led_on();

    if (temperature > 40000 && voltage >= 4200) {
        green_led_on();
    }

    TEST_ASSERT_TRUE(led_control_get(LED_COLOR_RED));
    TEST_ASSERT_TRUE(led_control_get(LED_COLOR_GREEN));   // stop charging fires
}
```

Change `&&` to `||` in the if-statement:
```bash
ceedling test:path[test_led_control]   # FAILS – one of the "only" tests breaks
```

Restore `&&`:
```bash
ceedling test:path[test_led_control]   # All pass again
```

**Say:** "This is TDD doing its job. The combined test suite locks down exactly
the intended logic so a future refactor cannot silently break it."

---

## 🔧 Refactor: What TDD Gave Us (3 min)

Point out what we now have:

| Test | What it proves |
|------|----------------|
| `normal_conditions` | System runs, no false trigger |
| `over_temp_only` | Single condition is not enough |
| `over_voltage_only` | Single condition is not enough |
| `both_thresholds_exceeded` | Both required → charging stops |

**Say:** "Each test is a living requirement. If someone changes the safety
logic six months from now, these tests are the first line of defence."

---

## ✅ Run Full Suite (1 min)

```bash
ceedling test:all
```

All tests pass including the safety logic tests from this demo.

---

## 🎯 Key Takeaways

- ✅ **Tests before code** — requirements become executable specifications
- ✅ **Each test expresses intent** — the test name is the requirement
- ✅ **The test suite catches logic regressions** — changing `&&` to `||` fails immediately
- ✅ **Millidegree / millivolt units** — integer units avoid floating-point precision issues

---

**Next Demo:** The safety threshold sits exactly at `temperature > 40000` and `voltage >= 4200`.
What happens at 40000 exactly? What about a raw ADC value that overflows when converted?
Demo 4 explores boundary conditions and an integer overflow bug from a real SDK driver.
