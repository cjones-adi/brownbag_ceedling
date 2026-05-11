# 🎬 Demo Series Overview

## 🎯 Learning Progression

```
Demo 1: Simple Start          (30 min)  – led_control, first tests, Red-Green-Refactor
    ↓
Demo 2: Mocking Hardware      (45 min)  – i2c_hal + temp_sensor, CMock call sequencing
    ↓
Demo 3: Test-Driven Development (45 min) – safety logic built test-first
    ↓
Demo 4: Boundary Conditions   (45 min)  – exact thresholds + integer overflow bug
    ↓
Demo 5: Integration Testing   (45 min)  – full safety charging pipeline end-to-end
```

**Total Series Time: 3 hours 30 minutes**

---

## 🧰 Reference Project

All demos use a single project: `reference-examples/my_project`

This is a **safety charging demo** for the **MAX32655FTHR** (Analog Devices / Maxim).
It combines:

| Module | File(s) | Role |
|--------|---------|------|
| LED control | `led_control.c/.h` | RGB LED state tracker (no hardware deps) |
| I2C HAL | `i2c_hal.c/.h` | Thin, mockable wrapper over the MXC I2C driver |
| Temperature sensor | `temp_sensor.c/.h` | MAX31889 driver (real SDK in fw, HAL in test) |
| ADC monitor | `adc_monitor.c/.h` | MAX32655 ADC → millivolt conversion |
| Main | `main.c` | Safety loop: Temp > 40 °C AND Voltage ≥ 4.2 V → stop charging |

**The production safety logic:**
```c
// Red LED always on (system running)
// If temperature > 40000 millideg AND voltage >= 4200 mV:
//     Green LED on  →  "stop charging" signal to MAX20303 PMIC via I2C
```

The demos build this project incrementally — attendees see how each testing concept
applies directly to real firmware they would write.

---

## 📋 Demo Session Details

### 🚀 Demo 1: Simple Start
**Duration:** 30 minutes
**Module:** `led_control`
**Key concept:** Red-Green-Refactor, host-native testing with no hardware deps

**What attendees see:**
- `led_control.c` has zero hardware includes — compiles and runs on the host
- `setUp()` gives every test a clean slate via `led_control_init()`
- The AAA pattern (Arrange → Act → Assert) inside a test function
- `TEST_ASSERT_BITS` for bitmask assertions
- First failing test → passing test cycle

**Takeaway:** "Tests run in milliseconds on the PC. No board, no cable, no flash cycle."

---

### 🎭 Demo 2: Mocking Hardware
**Duration:** 45 minutes
**Modules:** `i2c_hal` (mocked) + `temp_sensor` (read path)
**Key concept:** CMock replaces real hardware; any I2C response can be injected

**What attendees see:**
- CMock auto-generates `mock_i2c_hal.h` from `i2c_hal.h` — no manual stubs
- `StubWithCallback` injects a 25 °C byte pattern `{0x13, 0x88}`
- MAX31889 encoding math walked through live (raw × 5 = millideg)
- I2C timeout test — sensor disconnect without physically unplugging anything
- I2C bus-hung test — `i2c_hal_reset_ExpectAndReturn()` verifies recovery fires exactly once
- `tearDown()` → `Verify()` as the silent gatekeeper

**Takeaway:** "Fault injection without touching hardware. CMock verifies call counts automatically."

---

### 🔴 Demo 3: Test-Driven Development
**Duration:** 45 minutes
**Module:** Safety logic (`test_led_control.c` safety test group)
**Key concept:** Write the test first; the test is the requirement

**What attendees see:**
- Test written before `red_led_on()` exists → compile failure (RED)
- Convenience wrappers added → tests pass (GREEN)
- Four safety logic tests built one at a time:
  - Normal (both below) → green LED off
  - Over-temp only → green LED off  (AND not OR)
  - Over-voltage only → green LED off
  - Both exceeded → green LED on
- `&&` changed to `||` live to show the test suite catching the regression

**Takeaway:** "The test suite locks down the logic. A future refactor cannot silently break it."

---

### ⚠️ Demo 4: Boundary Conditions
**Duration:** 45 minutes
**Modules:** Safety threshold boundaries + `temp_sensor_convert_raw`
**Key concept:** Exact threshold semantics and silent integer overflow

**What attendees see:**
- `temperature > 40000` — at exactly 40000 must NOT trigger (`>` not `>=`)
- `voltage >= 4200` — at exactly 4200 MUST trigger
- One-unit-above / one-unit-below tests for each boundary
- `temp_sensor_convert_raw` overflow bug from the Linux lt8460 IIO driver:
  - `(uint16_t)` cast silently truncates a 32-bit result
  - raw=200 → 202 °C stored as 6 °C → safety threshold never crossed
  - Toggle `TEMP_CONVERT_BUG_ENABLED` to switch between bug and fix
  - Unit tests catch the overflow; hardware testing cannot

**Takeaway:** "Test exactly at the boundary. Integer overflow is silent — the test is not."

---

### 🔗 Demo 5: Integration Testing
**Duration:** 45 minutes
**Modules:** All three — `led_control` + `temp_sensor` + `adc_monitor`
**Key concept:** End-to-end pipeline validation; cross-module data agreements

**What attendees see:**
- Full safety pipeline driven through unit tests:
  I2C bytes → `temp_sensor_read()` → millideg → safety condition → LED state
- Data unit agreement confirmed (millideg, millivolts — same units everywhere)
- I2C error propagation: timeout must not cause a false alarm
- The same logic that runs in `main.c` — proved before the board is touched
- `ceedling gcov:all` — coverage report across all modules

**Takeaway:** "Integration tests close the gap between passing unit tests and a working system."

---

## ⏱️ Session Timing Guide

| Segment | Content | Time |
|---------|---------|------|
| Pre-demo | Reading materials, setup check | 15 min |
| Demo 1 | Simple start | 30 min |
| Break | | 10 min |
| Demo 2 | Mocking hardware | 45 min |
| Demo 3 | TDD | 45 min |
| Break | | 10 min |
| Demo 4 | Boundary conditions | 45 min |
| Demo 5 | Integration testing | 45 min |
| Q&A + hands-on intro | | 15 min |
| **Total** | | **~4 hours** |

---

**Next:** Each `demo-0x-*/demo-script.md` contains the full presenter script with
code snippets, timing cues, audience questions, and expected terminal output.
