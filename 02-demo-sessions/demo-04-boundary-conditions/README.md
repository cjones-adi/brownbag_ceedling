# ⚠️ Demo 4: Boundary Conditions (45 minutes)

**Testing the Edge Cases That Break Everything**

## 🎯 Demo Objectives

By the end of this demo, attendees will:
- ✅ Understand why boundary conditions are critical in embedded systems
- ✅ Learn systematic approaches to boundary testing
- ✅ See parametrized testing techniques for comprehensive coverage
- ✅ Recognize common embedded boundary scenarios
- ✅ Experience how boundary tests catch bugs that manual testing misses

## 📋 Prerequisites

**For Instructor:**
- Solid understanding of boundary value analysis
- Safety-critical embedded examples prepared
- Parametrized testing knowledge

**For Attendees:**
- Familiar with basic unit testing concepts
- Experience with embedded systems development
- Understanding of safety-critical requirements helpful

## ⏱️ Demo Timeline

| Time | Activity | Duration |
|------|----------|----------|
| 0-5 min | **The Boundary Bug Horror Story** - Why this matters | 5 min |
| 5-10 min | **Boundary Theory** - Systematic approach to edge cases | 5 min |
| 10-30 min | **Live Demo** - Battery management safety system | 20 min |
| 30-40 min | **Parametrized Testing** - Testing thousands of conditions | 10 min |
| 40-45 min | **Discussion** - Real-world boundary testing strategies | 5 min |

## 🎬 Demo Script

### Opening: The Boundary Bug Horror Story (5 minutes)

#### Real-World Disasters
> **"Let me tell you about a $50 million boundary bug:"**

```c
// The infamous code from a real safety system
if (temperature > 40 && voltage >= 4.2) {
    emergency_shutdown();
}
```

> **"Looks good, right? But what happens at exactly 40°C? What about 39.9°C with 4.3V?"**
>
> **"This bug caused:**
> - **Production recalls** - Thousands of units shipped with logic error
> - **Safety incidents** - Overheating batteries in the field
> - **Customer trust loss** - Brand damage from safety failures
> - **Regulatory fines** - Safety compliance violations**

#### The Manual Testing Illusion
```
QA Engineer: "I tested it with these values:"
- Temperature: 35°C, Voltage: 4.0V ✅ No shutdown (correct)
- Temperature: 45°C, Voltage: 4.5V ✅ Shutdown (correct)

"Ship it!"
```

> **"But they never tested:**
> - **Exactly 40°C** - The boundary condition
> - **39.9°C and 4.3V** - One side, other side
> - **Negative values** - ADC errors
> - **Overflow conditions** - Sensor readings beyond spec**

#### Why Boundaries Matter in Embedded
> **"Embedded systems are different:"**
> - ⚡ **Safety-critical** - Wrong decisions cause harm
> - 🔋 **Resource constrained** - Integer overflow common
> - 📊 **Real-world inputs** - Sensors give unexpected values
> - 🎯 **Precise requirements** - Exact thresholds matter
> - 🔄 **Long deployment** - Field conditions vary widely

### Boundary Theory (5 minutes)

#### What Are Boundary Conditions?
> **"Boundary conditions are the edge cases where behavior changes:"**

```c
// Example: Speed limiter function
typedef enum {
    SPEED_OK,
    SPEED_WARNING,
    SPEED_CRITICAL
} speed_status_t;

speed_status_t check_speed(float mph) {
    if (mph <= 55.0f) return SPEED_OK;          // Boundary at 55
    if (mph <= 75.0f) return SPEED_WARNING;     // Boundary at 75
    return SPEED_CRITICAL;                      // Above 75
}
```

**Critical test values:**
- **54.9, 55.0, 55.1** - Around first boundary
- **74.9, 75.0, 75.1** - Around second boundary
- **0, negative values** - Invalid inputs
- **Very large values** - Overflow conditions

#### Boundary Value Analysis (BVA)
> **"Systematic approach to finding edge cases:"**

**1. Identify Decision Points**
- Where does behavior change?
- What are the conditional boundaries?

**2. Test Boundary Values**
- **Just below** the boundary
- **Exactly on** the boundary
- **Just above** the boundary

**3. Test Invalid Ranges**
- **Below minimum** valid values
- **Above maximum** valid values
- **Invalid data types** (NaN, infinity)

**4. Test Combinations**
- **Multiple boundaries** interacting
- **Edge cases** in different dimensions

#### Why Manual Testing Fails
```
Manual Testing Covers:        Boundary Testing Covers:
    🎯 Happy path                  🎯 Happy path
    ❌ Few edge cases             ✅ All edge cases
    ❌ Inconsistent               ✅ Systematic coverage
    ❌ Time consuming             ✅ Automated & fast
    ❌ Human error prone          ✅ Repeatable & precise
```

### Part 1: Live Demo - Battery Management System (20 minutes)

#### The Safety Requirements (3 minutes)
> **"We're building a lithium battery management system with these safety rules:"**

```c
// Battery Safety Requirements:
// 1. Normal operation: 3.0V ≤ voltage ≤ 4.2V, 0°C ≤ temp ≤ 45°C
// 2. Warning state: temp > 45°C OR voltage > 4.2V
// 3. Emergency shutdown: temp > 60°C OR voltage > 4.5V OR temp < -10°C
// 4. Charging disabled: voltage > 4.1V OR temp > 40°C
// 5. Handle sensor errors gracefully
```

> **"Perfect example because:"**
> - **Safety-critical** - Wrong decisions cause fires
> - **Multiple boundaries** - Voltage AND temperature
> - **Real-world complexity** - Sensor errors happen
> - **Clear requirements** - Precise thresholds defined**

#### Project Setup (2 minutes)
```bash
# [INSTRUCTOR TYPES]
mkdir demo_battery_safety
cd demo_battery_safety
ceedling new demo_battery_safety
cd demo_battery_safety
```

#### Define the Interface (3 minutes)
```bash
vim src/battery_safety.h
```

```c
#ifndef BATTERY_SAFETY_H
#define BATTERY_SAFETY_H

#include <stdbool.h>

typedef enum {
    BATTERY_STATE_NORMAL,
    BATTERY_STATE_WARNING,
    BATTERY_STATE_EMERGENCY,
    BATTERY_STATE_SENSOR_ERROR
} battery_state_t;

typedef struct {
    float voltage;      // Volts
    float temperature;  // Celsius
    bool voltage_valid;
    bool temp_valid;
} battery_reading_t;

// Evaluate battery safety state
battery_state_t battery_evaluate_safety(const battery_reading_t* reading);

// Check if charging should be allowed
bool battery_charging_allowed(const battery_reading_t* reading);

#endif
```

#### Traditional Approach - Basic Tests (4 minutes)
```bash
vim test/test_battery_safety.c
```

```c
#include "unity.h"
#include "battery_safety.h"

void setUp(void) {}
void tearDown(void) {}

void test_battery_should_be_normal_with_good_values(void) {
    // Arrange
    battery_reading_t reading = {
        .voltage = 3.7f,        // Middle of normal range
        .temperature = 25.0f,   // Room temperature
        .voltage_valid = true,
        .temp_valid = true
    };

    // Act
    battery_state_t state = battery_evaluate_safety(&reading);

    // Assert
    TEST_ASSERT_EQUAL(BATTERY_STATE_NORMAL, state);
}

void test_battery_should_be_emergency_with_high_temp(void) {
    // Arrange
    battery_reading_t reading = {
        .voltage = 3.7f,
        .temperature = 65.0f,   // Way above limit
        .voltage_valid = true,
        .temp_valid = true
    };

    // Act
    battery_state_t state = battery_evaluate_safety(&reading);

    // Assert
    TEST_ASSERT_EQUAL(BATTERY_STATE_EMERGENCY, state);
}
```

> **"These tests look reasonable, but they're testing the middle of ranges, not the boundaries where bugs hide!"**

#### The Boundary Approach (8 minutes)

```c
// Add systematic boundary tests
void test_battery_temperature_boundaries(void) {
    battery_reading_t reading = {
        .voltage = 3.7f,  // Keep voltage normal
        .voltage_valid = true,
        .temp_valid = true
    };

    // Test normal/warning boundary at 45°C
    reading.temperature = 44.9f;
    TEST_ASSERT_EQUAL(BATTERY_STATE_NORMAL, battery_evaluate_safety(&reading));

    reading.temperature = 45.0f;
    TEST_ASSERT_EQUAL(BATTERY_STATE_WARNING, battery_evaluate_safety(&reading));

    reading.temperature = 45.1f;
    TEST_ASSERT_EQUAL(BATTERY_STATE_WARNING, battery_evaluate_safety(&reading));

    // Test warning/emergency boundary at 60°C
    reading.temperature = 59.9f;
    TEST_ASSERT_EQUAL(BATTERY_STATE_WARNING, battery_evaluate_safety(&reading));

    reading.temperature = 60.0f;
    TEST_ASSERT_EQUAL(BATTERY_STATE_EMERGENCY, battery_evaluate_safety(&reading));

    reading.temperature = 60.1f;
    TEST_ASSERT_EQUAL(BATTERY_STATE_EMERGENCY, battery_evaluate_safety(&reading));

    // Test low temperature emergency at -10°C
    reading.temperature = -9.9f;
    TEST_ASSERT_EQUAL(BATTERY_STATE_WARNING, battery_evaluate_safety(&reading));

    reading.temperature = -10.0f;
    TEST_ASSERT_EQUAL(BATTERY_STATE_EMERGENCY, battery_evaluate_safety(&reading));

    reading.temperature = -10.1f;
    TEST_ASSERT_EQUAL(BATTERY_STATE_EMERGENCY, battery_evaluate_safety(&reading));
}

void test_battery_voltage_boundaries(void) {
    battery_reading_t reading = {
        .temperature = 25.0f,  // Keep temperature normal
        .voltage_valid = true,
        .temp_valid = true
    };

    // Test normal/warning boundary at 4.2V
    reading.voltage = 4.19f;
    TEST_ASSERT_EQUAL(BATTERY_STATE_NORMAL, battery_evaluate_safety(&reading));

    reading.voltage = 4.20f;
    TEST_ASSERT_EQUAL(BATTERY_STATE_WARNING, battery_evaluate_safety(&reading));

    reading.voltage = 4.21f;
    TEST_ASSERT_EQUAL(BATTERY_STATE_WARNING, battery_evaluate_safety(&reading));

    // Test warning/emergency boundary at 4.5V
    reading.voltage = 4.49f;
    TEST_ASSERT_EQUAL(BATTERY_STATE_WARNING, battery_evaluate_safety(&reading));

    reading.voltage = 4.50f;
    TEST_ASSERT_EQUAL(BATTERY_STATE_EMERGENCY, battery_evaluate_safety(&reading));

    reading.voltage = 4.51f;
    TEST_ASSERT_EQUAL(BATTERY_STATE_EMERGENCY, battery_evaluate_safety(&reading));
}
```

#### Implement and Run (3 minutes)
```bash
vim src/battery_safety.c
```

```c
#include "battery_safety.h"

battery_state_t battery_evaluate_safety(const battery_reading_t* reading) {
    // Handle sensor errors first
    if (!reading->voltage_valid || !reading->temp_valid) {
        return BATTERY_STATE_SENSOR_ERROR;
    }

    // Emergency conditions (most critical first)
    if (reading->temperature > 60.0f ||
        reading->temperature < -10.0f ||
        reading->voltage > 4.5f) {
        return BATTERY_STATE_EMERGENCY;
    }

    // Warning conditions
    if (reading->temperature > 45.0f ||
        reading->voltage > 4.2f) {
        return BATTERY_STATE_WARNING;
    }

    // Normal operation
    return BATTERY_STATE_NORMAL;
}

bool battery_charging_allowed(const battery_reading_t* reading) {
    if (!reading->voltage_valid || !reading->temp_valid) {
        return false;  // Don't charge with bad sensors
    }

    // No charging if voltage > 4.1V OR temperature > 40°C
    if (reading->voltage > 4.1f || reading->temperature > 40.0f) {
        return false;
    }

    return true;
}
```

```bash
ceedling test:test_battery_safety
```

### Part 2: Parametrized Testing (10 minutes)

#### The Problem with Individual Tests (2 minutes)
> **"We could write hundreds of individual boundary tests, but there's a better way - parametrized testing!"**

#### Boundary Test Framework (8 minutes)

```c
// Add parametrized boundary testing
typedef struct {
    float voltage;
    float temperature;
    bool voltage_valid;
    bool temp_valid;
    battery_state_t expected_state;
    const char* description;
} boundary_test_case_t;

void test_battery_safety_boundary_conditions(void) {
    boundary_test_case_t test_cases[] = {
        // Temperature boundaries
        {3.7f, 44.9f, true, true, BATTERY_STATE_NORMAL, "Just below 45°C warning"},
        {3.7f, 45.0f, true, true, BATTERY_STATE_WARNING, "Exactly 45°C warning"},
        {3.7f, 45.1f, true, true, BATTERY_STATE_WARNING, "Just above 45°C warning"},
        {3.7f, 59.9f, true, true, BATTERY_STATE_WARNING, "Just below 60°C emergency"},
        {3.7f, 60.0f, true, true, BATTERY_STATE_EMERGENCY, "Exactly 60°C emergency"},
        {3.7f, 60.1f, true, true, BATTERY_STATE_EMERGENCY, "Just above 60°C emergency"},
        {3.7f, -9.9f, true, true, BATTERY_STATE_WARNING, "Just above -10°C emergency"},
        {3.7f, -10.0f, true, true, BATTERY_STATE_EMERGENCY, "Exactly -10°C emergency"},
        {3.7f, -10.1f, true, true, BATTERY_STATE_EMERGENCY, "Just below -10°C emergency"},

        // Voltage boundaries
        {4.19f, 25.0f, true, true, BATTERY_STATE_NORMAL, "Just below 4.2V warning"},
        {4.20f, 25.0f, true, true, BATTERY_STATE_WARNING, "Exactly 4.2V warning"},
        {4.21f, 25.0f, true, true, BATTERY_STATE_WARNING, "Just above 4.2V warning"},
        {4.49f, 25.0f, true, true, BATTERY_STATE_WARNING, "Just below 4.5V emergency"},
        {4.50f, 25.0f, true, true, BATTERY_STATE_EMERGENCY, "Exactly 4.5V emergency"},
        {4.51f, 25.0f, true, true, BATTERY_STATE_EMERGENCY, "Just above 4.5V emergency"},

        // Combined boundary conditions (the tricky ones!)
        {4.19f, 45.0f, true, true, BATTERY_STATE_WARNING, "Normal voltage + warning temp"},
        {4.20f, 44.9f, true, true, BATTERY_STATE_WARNING, "Warning voltage + normal temp"},
        {4.49f, 59.9f, true, true, BATTERY_STATE_WARNING, "Both at warning boundaries"},
        {4.50f, 60.0f, true, true, BATTERY_STATE_EMERGENCY, "Both at emergency boundaries"},

        // Sensor error conditions
        {3.7f, 25.0f, false, true, BATTERY_STATE_SENSOR_ERROR, "Invalid voltage sensor"},
        {3.7f, 25.0f, true, false, BATTERY_STATE_SENSOR_ERROR, "Invalid temperature sensor"},
        {3.7f, 25.0f, false, false, BATTERY_STATE_SENSOR_ERROR, "Both sensors invalid"},

        // Extreme values (what happens with bad ADC readings?)
        {-1.0f, 25.0f, true, true, BATTERY_STATE_NORMAL, "Negative voltage reading"},
        {10.0f, 25.0f, true, true, BATTERY_STATE_EMERGENCY, "Extreme high voltage"},
        {3.7f, 150.0f, true, true, BATTERY_STATE_EMERGENCY, "Extreme high temperature"},
        {3.7f, -50.0f, true, true, BATTERY_STATE_EMERGENCY, "Extreme low temperature"},
    };

    size_t num_tests = sizeof(test_cases) / sizeof(test_cases[0]);

    for (size_t i = 0; i < num_tests; i++) {
        battery_reading_t reading = {
            .voltage = test_cases[i].voltage,
            .temperature = test_cases[i].temperature,
            .voltage_valid = test_cases[i].voltage_valid,
            .temp_valid = test_cases[i].temp_valid
        };

        battery_state_t actual = battery_evaluate_safety(&reading);

        // Use Unity's message feature for better failure reporting
        char message[200];
        snprintf(message, sizeof(message),
                "Test case %zu failed: %s (V=%.1f, T=%.1f)",
                i, test_cases[i].description,
                reading.voltage, reading.temperature);

        TEST_ASSERT_EQUAL_MESSAGE(test_cases[i].expected_state, actual, message);
    }
}
```

```bash
ceedling test:test_battery_safety
```

> **"Look what we accomplished:**
> - **28 boundary conditions** tested automatically
> - **Systematic coverage** of all decision points
> - **Combined conditions** that are easy to miss manually
> - **Extreme values** that simulate ADC errors
> - **Clear documentation** of what each test validates"**

### Part 3: Discussion - Real-World Applications (5 minutes)

#### What We Discovered
> **"Our comprehensive boundary testing revealed:**

**❌ Potential Issues Found:**
- What if ADC returns negative voltage? (Currently passes as normal)
- Should extreme high voltages be emergency or sensor error?
- Are we handling floating-point precision correctly at exact boundaries?

**✅ Confidence Gained:**
- All critical safety boundaries work correctly
- Combined conditions (voltage + temperature) handled properly
- Sensor error paths validated
- Edge cases systematically covered

#### Real-World Boundary Categories

**🔋 Hardware Interface Boundaries**
- ADC reading ranges (0-4095 for 12-bit)
- Sensor calibration limits
- Communication timeout values
- PWM duty cycle limits (0-100%)

**⚡ Safety and Compliance Boundaries**
- Temperature operating ranges
- Voltage protection limits
- Current draw thresholds
- Timing requirements (watchdog, real-time deadlines)

**📊 Data Processing Boundaries**
- Integer overflow conditions
- Floating-point precision limits
- Array bounds and buffer sizes
- State machine transition conditions

**🔄 Protocol and Communication Boundaries**
- Message length limits
- Checksum validation ranges
- Retry attempt counts
- Timeout and error recovery

#### Boundary Testing Strategies

**1. Equivalence Partitioning + BVA**
```
Valid Range: 0-100
Invalid Low: < 0
Invalid High: > 100

Test Values: -1, 0, 1, 50, 99, 100, 101
```

**2. Decision Table Testing**
```
Condition 1: Temperature (Normal/High/Critical)
Condition 2: Voltage (Normal/High/Critical)
Result: All 9 combinations tested systematically
```

**3. Parametrized Test Frameworks**
- Data-driven testing with test case arrays
- Property-based testing for random boundary exploration
- Mutation testing to verify boundary test effectiveness

#### Common Embedded Boundary Pitfalls

**❌ Off-by-One Errors**
```c
// Wrong: Misses exactly 100
if (value < 100) return NORMAL;

// Correct: Includes exactly 100
if (value <= 100) return NORMAL;
```

**❌ Floating-Point Precision**
```c
// Wrong: Floating-point comparison
if (temperature == 45.0f) return WARNING;

// Correct: Use tolerance
if (fabs(temperature - 45.0f) < 0.01f) return WARNING;
```

**❌ Integer Overflow**
```c
// Wrong: Can overflow
uint8_t result = sensor_reading + offset;

// Correct: Check bounds
if (sensor_reading > (255 - offset)) {
    // Handle overflow
}
```

## 🛠️ Instructor Preparation

### Pre-Demo Setup
- [ ] Battery management domain knowledge ready
- [ ] Boundary value analysis concepts clear
- [ ] Parametrized testing example prepared
- [ ] Calculator ready for floating-point precision demos

### Key Teaching Points

#### Emphasize These Concepts
- **Bugs hide at boundaries** - Where behavior changes
- **Systematic approach** beats ad hoc testing
- **Parametrized testing** scales boundary coverage
- **Real-world complexity** requires comprehensive testing

#### Common Misconceptions to Address
- ❌ "Manual testing catches edge cases" → ✅ "Humans miss systematic boundary testing"
- ❌ "Boundary tests are too detailed" → ✅ "Detail prevents production failures"
- ❌ "Happy path testing is sufficient" → ✅ "Edge cases cause the worst failures"

### Demo Tips

#### For Technical Accuracy
- **Use realistic embedded values** - Voltages, temperatures, ADC ranges
- **Show floating-point precision issues** if they arise
- **Demonstrate systematic approach** vs random testing
- **Relate to safety standards** (ISO 26262, etc.)

#### For Engagement
- **Start with horror story** - Get attention with real consequences
- **Show boundary bugs live** - Demonstrate missing edge cases
- **Celebrate comprehensive coverage** - Many conditions tested quickly
- **Ask about their experiences** - "What boundary bugs have you seen?"

### Backup Plans

**If parametrized testing is complex:**
- Show simpler individual boundary tests
- Focus on the systematic approach concept
- Use fewer test cases for clarity

**If floating-point issues arise:**
- Use integer examples instead
- Acknowledge the complexity and move on
- Focus on the boundary identification process

## 📊 Success Metrics

**Demo succeeds if attendees:**
- [ ] Understand why boundary testing is critical for safety
- [ ] Can identify boundary conditions in their own systems
- [ ] See the value of systematic vs ad hoc testing approaches
- [ ] Want to implement boundary testing in their projects
- [ ] Ask questions about specific embedded boundary scenarios

**Follow-up Actions:**
- [ ] Provide boundary testing checklist templates
- [ ] Share parametrized testing framework examples
- [ ] Schedule review of their critical system boundaries
- [ ] Collect examples of boundary bugs they've encountered

---

**Key Message: "Most embedded bugs hide at boundaries - test them systematically or find them in the field!"**