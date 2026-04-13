# ⚠️ Demo 4: Quick Reference Script

**Total Time: 45 minutes**

## 📝 Opening Horror Story (5 min)
```
"$50 million boundary bug story:"

// The infamous safety system code
if (temperature > 40 && voltage >= 4.2) {
    emergency_shutdown();
}

"What happens at exactly 40°C? 39.9°C with 4.3V?"

This bug caused:
- Production recalls (thousands of units)
- Safety incidents (overheating batteries)
- Brand damage (customer trust loss)
- Regulatory fines (safety violations)

Manual testing checked:
- 35°C, 4.0V ✅ (middle ranges)
- 45°C, 4.5V ✅ (middle ranges)

Never tested exact boundaries where bugs hide!
```

## 🎯 Why Boundaries Matter (2 min)
```
Embedded systems are different:
⚡ Safety-critical - Wrong decisions cause harm
🔋 Resource constrained - Integer overflow common
📊 Real-world inputs - Sensors give unexpected values
🎯 Precise requirements - Exact thresholds matter
🔄 Long deployment - Field conditions vary widely
```

## 🧠 Boundary Theory (3 min)

### What Are Boundaries?
```
Edge cases where behavior changes:

speed_status_t check_speed(float mph) {
    if (mph <= 55.0f) return SPEED_OK;      // Boundary at 55
    if (mph <= 75.0f) return SPEED_WARNING; // Boundary at 75
    return SPEED_CRITICAL;                   // Above 75
}

Critical test values:
- 54.9, 55.0, 55.1 (around first boundary)
- 74.9, 75.0, 75.1 (around second boundary)
- 0, negatives (invalid inputs)
- Very large values (overflow)
```

### Boundary Value Analysis (BVA)
```
1. Identify Decision Points - Where does behavior change?
2. Test Boundary Values - Just below, exactly on, just above
3. Test Invalid Ranges - Below min, above max, invalid types
4. Test Combinations - Multiple boundaries interacting
```

## 🔋 Live Demo Setup (2 min)
```bash
mkdir demo_battery_safety
cd demo_battery_safety
ceedling new demo_battery_safety
cd demo_battery_safety
```

## ⚡ Battery Safety Requirements (3 min)
```c
// Battery Safety Requirements:
// 1. Normal: 3.0V ≤ voltage ≤ 4.2V, 0°C ≤ temp ≤ 45°C
// 2. Warning: temp > 45°C OR voltage > 4.2V
// 3. Emergency: temp > 60°C OR voltage > 4.5V OR temp < -10°C
// 4. Charging disabled: voltage > 4.1V OR temp > 40°C
// 5. Handle sensor errors

typedef enum {
    BATTERY_STATE_NORMAL,
    BATTERY_STATE_WARNING,
    BATTERY_STATE_EMERGENCY,
    BATTERY_STATE_SENSOR_ERROR
} battery_state_t;

battery_state_t battery_evaluate_safety(const battery_reading_t* reading);
```

## 🧪 Traditional vs Boundary Testing (8 min)

### Traditional Approach (3 min)
```c
void test_battery_should_be_normal_with_good_values(void) {
    battery_reading_t reading = {3.7f, 25.0f, true, true};
    TEST_ASSERT_EQUAL(BATTERY_STATE_NORMAL, battery_evaluate_safety(&reading));
}

void test_battery_should_be_emergency_with_high_temp(void) {
    battery_reading_t reading = {3.7f, 65.0f, true, true};
    TEST_ASSERT_EQUAL(BATTERY_STATE_EMERGENCY, battery_evaluate_safety(&reading));
}
```
**Say:** "Tests middle of ranges, not boundaries where bugs hide!"

### Boundary Approach (5 min)
```c
void test_battery_temperature_boundaries(void) {
    battery_reading_t reading = {3.7f, 0, true, true};

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
}
```

## 🔧 Implementation (3 min)
```c
battery_state_t battery_evaluate_safety(const battery_reading_t* reading) {
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
    if (reading->temperature > 45.0f || reading->voltage > 4.2f) {
        return BATTERY_STATE_WARNING;
    }

    return BATTERY_STATE_NORMAL;
}
```

## 📊 Parametrized Testing (10 min)

### The Problem (2 min)
```
"We could write hundreds of individual boundary tests,
but there's a better way - parametrized testing!"
```

### Test Framework (8 min)
```c
typedef struct {
    float voltage, temperature;
    bool voltage_valid, temp_valid;
    battery_state_t expected_state;
    const char* description;
} boundary_test_case_t;

void test_battery_safety_boundary_conditions(void) {
    boundary_test_case_t test_cases[] = {
        // Temperature boundaries
        {3.7f, 44.9f, true, true, BATTERY_STATE_NORMAL, "Just below 45°C"},
        {3.7f, 45.0f, true, true, BATTERY_STATE_WARNING, "Exactly 45°C"},
        {3.7f, 45.1f, true, true, BATTERY_STATE_WARNING, "Just above 45°C"},
        {3.7f, 59.9f, true, true, BATTERY_STATE_WARNING, "Just below 60°C"},
        {3.7f, 60.0f, true, true, BATTERY_STATE_EMERGENCY, "Exactly 60°C"},
        {3.7f, 60.1f, true, true, BATTERY_STATE_EMERGENCY, "Just above 60°C"},

        // Voltage boundaries
        {4.19f, 25.0f, true, true, BATTERY_STATE_NORMAL, "Just below 4.2V"},
        {4.20f, 25.0f, true, true, BATTERY_STATE_WARNING, "Exactly 4.2V"},
        {4.21f, 25.0f, true, true, BATTERY_STATE_WARNING, "Just above 4.2V"},

        // Combined conditions (the tricky ones!)
        {4.19f, 45.0f, true, true, BATTERY_STATE_WARNING, "Normal V + warning T"},
        {4.50f, 60.0f, true, true, BATTERY_STATE_EMERGENCY, "Both at emergency"},

        // Sensor errors
        {3.7f, 25.0f, false, true, BATTERY_STATE_SENSOR_ERROR, "Invalid voltage"},

        // Extreme values (bad ADC readings)
        {-1.0f, 25.0f, true, true, BATTERY_STATE_NORMAL, "Negative voltage"},
        {10.0f, 25.0f, true, true, BATTERY_STATE_EMERGENCY, "Extreme voltage"},
    };

    for (size_t i = 0; i < num_tests; i++) {
        // Run test case with descriptive failure message
        // TEST_ASSERT_EQUAL_MESSAGE with description
    }
}
```

**Say:** "28 boundary conditions tested systematically!"

## 💬 Discussion (5 min)

### What We Accomplished
- ✅ **28 boundary conditions** tested automatically
- ✅ **Systematic coverage** of all decision points
- ✅ **Combined conditions** that are easy to miss
- ✅ **Extreme values** simulating ADC errors
- ✅ **Clear documentation** of each test

### Real-World Categories

#### Hardware Interface Boundaries
- ADC ranges (0-4095 for 12-bit)
- Sensor calibration limits
- Communication timeouts
- PWM duty cycles (0-100%)

#### Safety/Compliance Boundaries
- Temperature operating ranges
- Voltage protection limits
- Current thresholds
- Timing requirements

#### Data Processing Boundaries
- Integer overflow conditions
- Floating-point precision
- Array bounds, buffer sizes
- State machine transitions

### Common Pitfalls
```c
❌ Off-by-One: if (value < 100)    ✅ Correct: if (value <= 100)
❌ Float Equality: == 45.0f        ✅ Tolerance: fabs(temp - 45.0f) < 0.01f
❌ Overflow: result = reading + offset  ✅ Check: if (reading > 255 - offset)
```

---

## 🎯 Instructor Notes

### Key Messages
- **Bugs hide at boundaries** where behavior changes
- **Systematic approach** beats random testing
- **Parametrized testing** scales boundary coverage
- **Real-world complexity** needs comprehensive testing

### Demo Tips
- **Start with impact** - Horror story gets attention
- **Use realistic values** - Embedded voltages/temperatures
- **Show systematic approach** vs ad hoc testing
- **Celebrate coverage** - Many conditions tested quickly

### Handle Questions
**"Too detailed?"** → "Detail prevents production failures"
**"Manual testing sufficient?"** → "Humans miss systematic boundaries"
**"Takes too long?"** → "Automated tests run in milliseconds"

---

**End Goal**: Attendees excited about systematic boundary testing for safety!