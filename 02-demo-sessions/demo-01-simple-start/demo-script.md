# 🎬 Demo 1: Quick Reference Script

**Total Time: 30 minutes**

## 📝 Opening Hook (1 min)
```
"Show of hands: How many have said 'My code works, I tested it on hardware'?"
"Today: Test embedded code BEFORE it touches a microcontroller"
```

## 🎯 Problem Setup (4 min)
```
LED Control Requirements:
1. Turn LED on/off by number (0-7)
2. Query LED state
3. Handle invalid LED numbers
4. Initialize all LEDs to OFF
```

## ⚡ Live Setup (10 min)

### Create Project (3 min)
```bash
mkdir demo_led_control
cd demo_led_control
ceedling new demo_led_control
cd demo_led_control
tree
```

### Verify Setup (2 min)
```bash
cat project.yml    # Show key settings
ceedling test:all  # Should show "No tests to run"
```

### Create Header (5 min)
```c
// src/led_control.h
#ifndef LED_CONTROL_H
#define LED_CONTROL_H
#include <stdbool.h>
#include <stdint.h>

void led_control_init(void);
void led_control_turn_on(uint8_t led_num);
void led_control_turn_off(uint8_t led_num);
bool led_control_is_on(uint8_t led_num);
uint8_t led_control_get_max_leds(void);

#endif
```

## 🧪 First Test (5 min)

```c
// test/test_led_control.c
#include "unity.h"
#include "led_control.h"

void setUp(void) {
    led_control_init();
}

void tearDown(void) {}

void test_led_control_should_initialize_all_leds_to_off_state(void) {
    for (uint8_t led = 0; led < led_control_get_max_leds(); led++) {
        TEST_ASSERT_FALSE(led_control_is_on(led));
    }
}
```

## 🔴 Red Phase (1 min)
```bash
ceedling test:test_led_control  # FAILS - functions don't exist
```
**Say:** "RED phase - test fails because no implementation yet!"

## 🟢 Green Phase (4 min)

```c
// src/led_control.c
#include "led_control.h"

#define MAX_LEDS 8
static bool led_states[MAX_LEDS];

void led_control_init(void) {
    for (uint8_t i = 0; i < MAX_LEDS; i++) {
        led_states[i] = false;
    }
}

void led_control_turn_on(uint8_t led_num) {
    if (led_num < MAX_LEDS) led_states[led_num] = true;
}

void led_control_turn_off(uint8_t led_num) {
    if (led_num < MAX_LEDS) led_states[led_num] = false;
}

bool led_control_is_on(uint8_t led_num) {
    if (led_num < MAX_LEDS) return led_states[led_num];
    return false;
}

uint8_t led_control_get_max_leds(void) {
    return MAX_LEDS;
}
```

```bash
ceedling test:test_led_control  # PASSES!
```
**Say:** "GREEN! Test passes - we have working, tested code!"

## 💬 Discussion (5 min)

### Key Points
- ✅ Complete testing environment in 10 minutes
- ✅ Tests run instantly - no hardware needed
- ✅ Red-Green-Refactor cycle demonstrated
- ✅ Confidence in code correctness

### Handle Common Questions
**"This doesn't test GPIO!"** → "Correct! Unit tests = logic. Demo 2 = mocking hardware."
**"Slower than hardware?"** → "Milliseconds vs minutes. Tests edge cases you'd miss."
**"Real-time constraints?"** → "Unit tests = algorithms. Performance testing = separate."

### Next Demo Teasers
- Demo 2: Mock I2C hardware
- Demo 3: Test-driven development
- Demo 4: Boundary conditions

---

## 🎯 Instructor Notes

### Keep Energy High
- Move quickly through setup
- Celebrate when tests pass
- Use "show of hands" questions
- Share personal testing stories

### Handle Issues
- **Compilation error**: Check syntax carefully
- **Ceedling not found**: Verify PATH
- **Test failure**: Use as learning opportunity

### Time Management
- **5 min warning** if behind schedule
- **Skip bonus test** if running late
- **Extend discussion** if ahead of schedule

---

**End Goal**: Attendees leave excited to try unit testing themselves!