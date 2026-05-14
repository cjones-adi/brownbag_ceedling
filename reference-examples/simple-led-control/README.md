# 💡 Simple LED Control — Reference Example

**Demo 1 "Simple Start" reference project.**

A beginner-friendly Ceedling project that demonstrates basic unit testing concepts
using a module with **zero hardware dependencies**.  No GPIO, no HAL, no mocks — just
pure C logic tested with Unity on the host.

Hardware interaction (gpio_hal, CMock) is introduced in the **Demo 2** reference project
(`reference-examples/simple_mock`).

---

## 📁 Project Structure

```
simple-led-control/
├── project.yml              # Ceedling config (:use_mocks: FALSE)
├── src/
│   ├── led_control.h        # LED control interface (no hardware types)
│   └── led_control.c        # Pure state-array implementation
└── test/
    └── test_led_control.c   # Unity tests — no mock includes
```

---

## 🔍 What This Example Demonstrates

| Concept | How it appears here |
|---|---|
| Arrange-Act-Assert (AAA) | Every test follows the three-section pattern |
| `setUp()` / `tearDown()` | `setUp()` calls `led_control_init()` for a clean slate |
| Basic Unity assertions | `TEST_ASSERT_TRUE/FALSE`, `TEST_ASSERT_FALSE_MESSAGE`, `TEST_ASSERT_GREATER_THAN` |
| Boundary-condition testing | Out-of-range index returns `false`, does not crash |
| LED independence | Turning one LED off must not affect others |

---

## 🚀 Quick Start

```bash
cd reference-examples/simple-led-control/
ceedling test:all
```

Expected output:
```
[test_led_control.c]
  All tests passed.

TESTED:  7
PASSED:  7
FAILED:  0
IGNORED: 0
```

---

## 📋 Test Scenarios Covered

### Group A — Initialisation
- All LEDs are OFF after `led_control_init()`

### Group B — Turn on / turn off
- `led_control_turn_on()` sets the LED to ON
- `led_control_turn_off()` clears the LED
- Turning one LED off does not affect neighbouring LEDs

### Group C — Boundary conditions
- `led_control_is_on()` returns `false` for an out-of-range index
- `led_control_turn_on(255)` does not crash or corrupt valid state

### Group D — Utility
- `led_control_get_max_leds()` returns a non-zero value

---

## 🔧 Key Design Points

### No hardware in `led_control.c`
```c
// The only includes:
#include "led_control.h"   // our own header
// stdint.h / stdbool.h pulled in via the header
```
No GPIO registers, no HAL, no SDK.  Runs anywhere a C compiler exists.

### `setUp()` as the test reset button
```c
void setUp(void)
{
    led_control_init();   // fresh state — no test depends on another
}
```

### AAA in action
```c
void test_led_control_turn_on_sets_led_to_on(void)
{
    // Arrange — setUp() already called init
    uint8_t led = 3u;

    // Act
    led_control_turn_on(led);

    // Assert
    TEST_ASSERT_TRUE(led_control_is_on(led));
}
```

---

## 🔗 Related Resources

| Resource | Purpose |
|---|---|
| `02-demo-sessions/demo-01-simple-start/` | Presenter script for this example |
| `reference-examples/simple_mock/` | Demo 2 — adds gpio_hal + CMock mocking |

---

**Perfect for:** First-time embedded testing, learning AAA structure, introducing the Red-Green-Refactor cycle without the complexity of mocks.
