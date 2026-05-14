# 🎬 Demo 1: Simple Start – LED Control

**Total Time: 30 minutes**
**Module under test:** `led_control` (pure C logic, zero hardware dependencies)

---

## 📝 Opening Hook (2 min)

```
"Today we write tests for embedded code that run on your laptop in milliseconds –
no board, no cable, no flash cycle."
```

---

## 🎯 What We Are Building (3 min)

A pure-logic `led_control` module — no GPIO, no I2C, no SDK.

The module tracks the on/off state of up to 8 LED channels in a software
state array. There are **no hardware calls** whatsoever; the implementation
only uses `<stdbool.h>` and `<stdint.h>`. This is intentional: Demo 1 is
about proving that logic correctness can be verified completely in software.
Hardware interaction (GPIO HAL, mocking) is introduced in Demo 2.

```
Requirements:
1. Initialize all LEDs to OFF
2. Turn an individual LED on or off by number (0–7)
3. Query whether a specific LED is on
4. Handle out-of-range LED numbers gracefully (no crash, return false)
5. Report the number of supported LEDs
```

---

## ⚡ Live Setup (5 min)

### Create and inspect the project
```bash
cd 02-demo-sessions/demo-01-simple-start
ceedling new simple-led-control   # or ceedling new demo_led_control
cat project.yml        # Show :paths: and Unity plugin — no :mock_headers: yet
ceedling test:all      # Baseline: "No tests to run" — Ceedling is working
```

### Create the led module
```bash
ceedling module:create[led_control]
```
### Show the files we will be working with
```
src/led_control.h   ← interface we are about to define
src/led_control.c   ← implementation (pure state logic, no hardware calls)
test/test_led_control.c
```

Point out: `led_control.c` will have **no hardware includes** — just `<stdint.h>`
and `<stdbool.h>`. That is what makes it runnable on the host without any SDK or board.

---

## 🔍 Walk Through the Header (3 min)

```c
// src/led_control.h

#ifndef LED_CONTROL_H
#define LED_CONTROL_H

#include <stdbool.h>
#include <stdint.h>

// Initialize LED controller — sets all channels to OFF
void led_control_init(void);

// Turn LED on/off by index (0-based)
void led_control_turn_on(uint8_t led_num);
void led_control_turn_off(uint8_t led_num);

// Query LED state; returns false for out-of-range index
bool led_control_is_on(uint8_t led_num);

// Returns the total number of supported LED channels
uint8_t led_control_get_max_leds(void);

#endif
```

Key points:
- **No GPIO types, no board headers** — the interface is fully host-portable.
- `led_control_is_on()` returns `false` (not a crash) for an invalid index —
  boundary behaviour we will explicitly test.
- Everything is plain C: no HAL, no PMIC, no I2C. Hardware wiring comes in Demo 2.

---

## 🧪 Walk Through the First Tests (5 min)

Open `test/test_led_control.c` and walk through the structure:

```c
#include "unity.h"
#include "led_control.h"

/* reset module state before each test */
void setUp(void)
{
    led_control_init();   // fresh state before every test
}

/* release resources after each test  */
void tearDown(void)
{
    // nothing to clean up — no mocks, no hardware
}

// A good test name answers three questions: what function, under what condition, what is the expected result.
// test_<function>_<condition>_<expected_result>

// Initialisation test
void test_led_control_should_initialize_all_leds_to_off(void) {
    // Arrange - setUp() already called led_control_init()
    // Initially
    led_control_init();

    // Act & Assert - Check that all LEDs start in OFF state
    for (uint8_t led = 0; led < led_control_get_max_leds(); led++) {
        TEST_ASSERT_FALSE_MESSAGE(led_control_is_on(led),
                                  "LED should be OFF after initialization");
    }
}

// State set / clear
void test_led_control_turn_on_sets_led_to_on(void)            { ... }
void test_led_control_turn_off_clears_led(void)               { ... }

// Independence: toggling one LED must not affect another
void test_led_control_leds_are_independent(void)              { ... }

// Boundary: out-of-range index must not crash
void test_led_control_out_of_range_returns_false(void)        { ... }
```

Show the **AAA pattern** inside one test:
```c
void test_led_control_turn_on_sets_led_to_on(void)
{
    // Arrange – setUp() already called led_control_init()
    uint8_t led_num = 3;

    // Act
    led_control_turn_on(led_num);

    // Assert
    TEST_ASSERT_TRUE(led_control_is_on(led_num));
}
```

Point out: no mock setup, no `ExpectAndReturn` — just state in, state out.
This is the simplest possible unit test: call a function, check the result.

---

## 🔴 Red Phase (1 min)

Create the test file but leave `led_control.c` empty (only the header declared):

```bash
ceedling test:path[test_led_control]   # FAILS – linker errors, functions undefined
```

**Say:** "RED phase — the test cannot link because the implementation doesn't exist yet.
That is exactly what we want: the test is already telling us what to build."

---

## 🟢 Green Phase (4 min)

Create the minimal `led_control.c`:

```c
#include "led_control.h"

#define MAX_LEDS 8
static bool led_states[MAX_LEDS];

void led_control_init(void)
{
    for (uint8_t i = 0; i < MAX_LEDS; i++) {
        led_states[i] = false;
    }
}

void led_control_turn_on(uint8_t led_num)
{
    if (led_num < MAX_LEDS) {
        led_states[led_num] = true;
    }
}

void led_control_turn_off(uint8_t led_num)
{
    if (led_num < MAX_LEDS) {
        led_states[led_num] = false;
    }
}

bool led_control_is_on(uint8_t led_num)
{
    if (led_num >= MAX_LEDS) return false;
    return led_states[led_num];
}

uint8_t led_control_get_max_leds(void)
{
    return MAX_LEDS;
}
```

```bash
ceedling test:path[test_led_control]   # PASSES
```

**Say:** "GREEN — minimal code, no hardware at all. The array is the entire 'hardware'.
Notice `led_control_is_on()` guards the index and returns `false` — our boundary test
already verified that."

---

## 🔧 Refactor Phase (2 min)

Point out the independence test — it proves that turning one LED off does not affect others:

```c
void test_led_control_leds_are_independent(void)
{
    // Arrange
    led_control_turn_on(0);
    led_control_turn_on(1);
    led_control_turn_on(2);

    // Act
    led_control_turn_off(1);

    // Assert
    TEST_ASSERT_TRUE(led_control_is_on(0));    // unaffected
    TEST_ASSERT_FALSE(led_control_is_on(1));   // off
    TEST_ASSERT_TRUE(led_control_is_on(2));    // unaffected
}
```

Mention `TEST_ASSERT_FALSE_MESSAGE` as a way to emit a helpful failure message when
looping over all channels in the init test — Unity has many assertion variants.

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
- ✅ **Red → Green → Refactor** — write the test first, then the minimum code
- ✅ **Tests run in milliseconds** — faster feedback than any flash cycle
- ✅ **Boundary conditions tested** — out-of-range index handled safely

---

**Next Demo:** We introduce a real hardware dependency (`gpio_hal`) inside
`led_control_set()` and use **CMock** to replace it with a mock — so we can
verify that the correct GPIO pin is driven without any board on the bench.
