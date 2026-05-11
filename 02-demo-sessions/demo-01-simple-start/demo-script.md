# 🎬 Demo 1: Simple Start – LED Control

**Total Time: 30 minutes**
**Module under test:** `led_control` (pure C, no hardware dependencies)

---

## 📝 Opening Hook (2 min)

```
"Show of hands: How many have said 'My code works, I tested it on hardware'?"

"Today we write tests for embedded code that run on your laptop in milliseconds –
no board, no cable, no flash cycle."
```

---

## 🎯 What We Are Building (3 min)

The `led_control` module from the MAX32655FTHR safety charging demo.

It tracks the on/off state of three RGB LED channels (Red, Green, Blue) in software.
In the real firmware, `main.c` reads `led_control_get_state()` and writes the bitmask
to the MAX20303 PMIC via I2C. For unit testing, there is no PMIC — just pure C logic.

```
Requirements:
1. Initialize all LEDs to OFF
2. Set / clear individual LED channels by color
3. Query whether a specific LED is on
4. Toggle an LED's state
5. Return the raw combined state bitmask
6. Return -1 (error) if the module is not initialized
```

---

## ⚡ Live Setup (5 min)

### Create and inspect the project
```bash
cd reference-examples/my_project
cat project.yml        # Show :paths:, :plugins:, and :defines: :test: [TEST]
ceedling test:all      # Baseline: all tests should pass before we start
```

### Show the files we will be working with
```
src/led_control.h   ← interface we are testing
src/led_control.c   ← implementation (no #include of any hardware driver)
test/test_led_control.c
```

Point out: `led_control.c` has **no hardware includes** — just `<stdint.h>` and `<stdbool.h>`.
That is what makes it testable on the host without any SDK or board.

---

## 🔍 Walk Through the Header (3 min)

```c
// src/led_control.h  (show the real file)

typedef enum {
    LED_COLOR_BLUE  = 0x1,
    LED_COLOR_RED   = 0x2,
    LED_COLOR_GREEN = 0x4
} led_color_t;

int  led_control_init(void);
int  led_control_set(led_color_t color, bool state);
bool led_control_get(led_color_t color);
int  led_control_toggle(led_color_t color);
uint8_t led_control_get_state(void);
```

Key point: the bitmask values match the MAX20303 PMIC register bits — the same
bitmask that will be written to hardware in `main.c`.

---

## 🧪 Walk Through the First Tests (5 min)

Open `test/test_led_control.c` and walk through the structure:

```c
#include "unity.h"
#include "led_control.h"

void setUp(void)    { led_control_init(); }  // fresh state before every test
void tearDown(void) { }

// Initialisation tests
void test_led_control_init_should_succeed(void)         { ... }
void test_led_control_init_clears_state(void)           { ... }

// Per-color set / clear
void test_red_led_on_sets_red(void)                     { ... }
void test_red_led_off_clears_red(void)                  { ... }
void test_red_led_on_does_not_affect_green(void)        { ... }
```

Show the **AAA pattern** inside one test:
```c
void test_red_led_on_sets_red(void)
{
    // Arrange – setUp() already called led_control_init()

    // Act
    int result = red_led_on();

    // Assert
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_TRUE(led_control_get(LED_COLOR_RED));
}
```

---

## 🔴 Red Phase (1 min)

Comment out `red_led_on()` in `led_control.h` (or pretend it doesn't exist yet):

```bash
ceedling test:path[test_led_control]   # FAILS – function undeclared
```

**Say:** "RED phase — the test fails because the function doesn't exist yet. That is the goal."

---

## 🟢 Green Phase (3 min)

Restore / add `red_led_on()` in `led_control.c`:

```c
int red_led_on(void)  { return led_control_set(LED_COLOR_RED,  true);  }
int red_led_off(void) { return led_control_set(LED_COLOR_RED,  false); }
```

```bash
ceedling test:path[test_led_control]   # PASSES
```

**Say:** "GREEN — the simplest code that makes the test pass. No premature optimisation."

---

## 🔧 Refactor Phase (2 min)

Point out the existing `led_control_get_state()` bitmask and the `test_get_state_reflects_active_leds` test:

```c
void test_get_state_reflects_active_leds(void)
{
    red_led_on();
    green_led_on();
    uint8_t state = led_control_get_state();
    TEST_ASSERT_BITS(LED_COLOR_RED,   LED_COLOR_RED,   state);
    TEST_ASSERT_BITS(LED_COLOR_GREEN, LED_COLOR_GREEN, state);
    TEST_ASSERT_BITS(LED_COLOR_BLUE,  0,               state);   // Blue should be OFF
}
```

Show `TEST_ASSERT_BITS` — a Unity macro that checks only the specified bit positions.
This is how the bitmask test stays readable and precise.

---

## ✅ Run Full Suite (1 min)

```bash
ceedling test:all
```

Expected output:
```
[test_led_control.c]
  All tests passed.

TESTED:  <n>
PASSED:  <n>
FAILED:  0
IGNORED: 0
```

---

## 🎯 Key Takeaways

- ✅ **No hardware needed** — `led_control.c` has zero hardware includes
- ✅ **setUp() gives every test a clean slate** — no test depends on another
- ✅ **Red → Green → Refactor** — write the test first, then the code
- ✅ **Tests run in milliseconds** — faster feedback than any flash cycle

---

**Next Demo:** We add a real hardware dependency (`i2c_hal`) and replace it with a mock so we can test the temperature sensor driver without a MAX31889 on the bench.
