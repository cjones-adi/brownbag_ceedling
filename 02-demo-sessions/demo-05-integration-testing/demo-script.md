# 🔗 Demo 5: Quick Reference Script

**Total Time: 45 minutes**

## 📝 Opening Problem (5 min)
```
"Show of hands: How many times heard this?"

👩‍💻 Dev A: "My sensor driver unit tests all pass!"
👨‍💻 Dev B: "My display controller unit tests all pass!"
🤖 Integration: "System crashes when we put them together!"

What Unit Tests Miss:
🔄 Data flow between modules (format mismatches)
⏰ Timing relationships (update rates, sync)
🧠 Memory interactions (shared buffers, conflicts)
📞 Communication protocols (message formats, errors)
🎯 System-level behavior (end-to-end scenarios)
```

## 🏗️ Testing Pyramid (5 min)

### Traditional vs Embedded Reality
```
Traditional:                 Embedded:
    /\                          /\
   /  \   System (5%)          /  \   Hardware-in-Loop (5%)
  /____\                      /____\
 /      \ Integration (20%)   /      \ Integration (25%)
/________\                   /________\
/        \ Unit (75%)       /        \ Unit (70%)
```

### Why More Integration in Embedded?
- **Complex interactions** - HAL, real-time, interrupts
- **Timing dependencies** - Sampling rates, timeouts, deadlines
- **Safety requirements** - Fail-safe, recovery, compliance

## 🌡️ System Architecture (3 min)
```
┌─────────────┐    ┌─────────────┐    ┌─────────────┐    ┌─────────────┐
│   Sensor    │───▶│  Processor  │───▶│   Logger    │───▶│   Display   │
│   Driver    │    │    Filter   │    │   Buffer    │    │ Controller  │
└─────────────┘    └─────────────┘    └─────────────┘    └─────────────┘

BME280 @ 1Hz  →  Moving Average  →  Ring Buffer   →  LCD @ 1Hz
                 3-sample filter    100 readings

Integration Points (Where Bugs Hide):
- Data format between sensor and filter
- Update rate synchronization
- Buffer overflow handling
- Error propagation through pipeline
```

## ⚡ Setup (2 min)
```bash
mkdir demo_temp_monitor_integration
cd demo_temp_monitor_integration
ceedling new demo_temp_monitor_integration
cd demo_temp_monitor_integration
```

## 🔧 Module Interfaces (5 min)
```c
// sensor_driver.h
typedef enum { SENSOR_OK, SENSOR_COMM_ERROR, SENSOR_NOT_READY } sensor_status_t;
sensor_status_t sensor_read_temperature(float* temperature_celsius);

// temp_filter.h
typedef struct { float samples[3]; int current_index; int sample_count; } temp_filter_t;
float temp_filter_update(temp_filter_t* filter, float new_sample);

// temp_logger.h
typedef struct { float readings[100]; uint32_t timestamps[100]; int write_index; } temp_logger_t;
void temp_logger_add_reading(temp_logger_t* logger, float temp, uint32_t timestamp);

// temp_display.h
typedef enum { TEMP_TREND_STABLE, TEMP_TREND_RISING, TEMP_TREND_FALLING } temp_trend_t;
void temp_display_update(float current, float average, temp_trend_t trend);
```

## 🧪 Unit Tests First (5 min)
```c
// test/test_temp_filter.c - Establish baseline functionality
void test_temp_filter_should_initialize_to_empty_state(void) {
    temp_filter_t filter;
    temp_filter_init(&filter);
    TEST_ASSERT_EQUAL(0, filter.sample_count);
}

void test_temp_filter_should_compute_moving_average_when_full(void) {
    temp_filter_t filter;
    temp_filter_init(&filter);

    temp_filter_update(&filter, 20.0f);
    temp_filter_update(&filter, 25.0f);
    float result = temp_filter_update(&filter, 30.0f);

    TEST_ASSERT_FLOAT_WITHIN(0.01f, 25.0f, result);  // Average of 20,25,30
}
```

**Say:** "Unit tests prove individual components work. Now let's test integration..."

## 🔗 Integration Tests (15 min)

### Complete Pipeline Test (8 min)
```c
// test/test_temp_monitor_integration.c
#include "mock_i2c_hal.h"

void test_complete_temperature_pipeline_should_process_correctly(void) {
    // Arrange - Mock sensor return
    uint8_t temp_data[] = {0x80, 0x00, 0x00};  // 25.0°C in BME280 format
    i2c_read_ExpectAndReturn(BME280_ADDR, BME280_TEMP_REG, NULL, 3, I2C_OK);
    i2c_read_ReturnMemThruPtr_data(temp_data, 3);

    // Act - Run complete pipeline
    float raw_temp;
    sensor_read_temperature(&raw_temp);
    float filtered = temp_filter_update(&filter, raw_temp);
    temp_logger_add_reading(&logger, filtered, timestamp);
    float average = temp_logger_get_average(&logger, 1);
    temp_display_update(filtered, average, TEMP_TREND_STABLE);

    // Assert - End-to-end verification
    TEST_ASSERT_FLOAT_WITHIN(0.5f, 25.0f, filtered);
    TEST_ASSERT_EQUAL(1, logger.count);
}
```

### Error Propagation Test (4 min)
```c
void test_system_should_handle_sensor_errors_gracefully(void) {
    // Arrange - Force communication failure
    i2c_read_ExpectAndReturn(BME280_ADDR, BME280_TEMP_REG, NULL, 3, I2C_TIMEOUT);

    // Act
    sensor_status_t result = sensor_read_temperature(&temperature);

    // Assert - Error handled gracefully
    TEST_ASSERT_EQUAL(SENSOR_COMM_ERROR, result);
    // Verify error propagates to display
    temp_display_show_error("Sensor Communication Error");
}
```

### High Frequency Test (3 min)
```c
void test_system_maintains_integrity_under_rapid_updates(void) {
    // Arrange - Multiple rapid readings
    uint8_t readings[][3] = { {0x80,0,0}, {0x81,0,0}, {0x82,0,0} };

    // Act - Process multiple readings rapidly
    for (int i = 0; i < 3; i++) {
        i2c_read_ExpectAndReturn(...);
        // Process reading through pipeline
    }

    // Assert - Data integrity maintained
    TEST_ASSERT_EQUAL(3, logger.count);
    TEST_ASSERT_TRUE(temp_filter_is_stable(&filter));
}
```

**Say:** "Integration testing reveals data flow, error propagation, timing behavior!"

## 📊 What Integration Tests Revealed (5 min)
```
✅ Integration Tests Found:
- Data format issues (units, precision, endianness)
- Error propagation paths (how failures cascade)
- Resource management (buffer overflow handling)
- System resilience (recovery from failures)

❌ These Were Missed by Unit Tests:
- Module interaction bugs
- Timing dependency issues
- Resource sharing conflicts
- System-level error scenarios
```

## 🎯 Testing Strategy (5 min)

### When to Write Integration Tests
```
✅ Write Integration Tests When:
🔄 Data flows between modules
⏰ Timing dependencies exist
🧠 Shared resources are used
🛡️ System-level behavior matters

❌ Avoid Integration Tests When:
- Pure algorithm testing (use unit tests)
- Simple data transformations
- Independent utility functions
- Complex setup outweighs value
```

### Optimal Balance
```
⚖️ Testing Strategy:
70% Unit Tests     - Fast feedback, easy debugging
25% Integration    - Critical interactions
5% System Tests    - End-to-end validation

Too Few Integration → Modules work alone, fail together
Too Many Integration → Slow feedback, hard to debug
```

### Integration Patterns
```
📊 Pipeline Testing:     Input → A → B → C → Output
🔄 State Machine:        Event → State → Action → Hardware
📞 Protocol Stack:       App → Transport → Network → Physical
⚡ Real-Time:           Interrupt → Handler → Task → Response
```

## 💬 Discussion (5 min)

### Key Takeaways
- **Integration ≠ replacement** for unit tests - both needed
- **Test interactions**, not implementations
- **Balance is crucial** - optimize for feedback speed
- **Embedded needs more** integration testing than typical software

### Common Questions
**"Integration tests too slow?"** → "Critical interactions justify cost"
**"Unit tests catch everything?"** → "Unit tests miss interaction bugs"
**"Replace unit tests?"** → "Both needed for comprehensive coverage"

### Real-World Challenges
- Hardware simulation complexity
- Real-time timing constraints
- Resource limitation testing
- Power management interactions

---

## 🎯 Instructor Notes

### Key Messages
- **Unit tests** prove components work individually
- **Integration tests** prove they work together
- **Both essential** for reliable embedded systems
- **Balance** prevents slow feedback cycles

### Demo Tips
- **Start with unit baseline** - show what they establish
- **Show integration failures** that unit tests miss
- **Keep examples realistic** to embedded development
- **Emphasize balance** - not replacement strategy

### Handle Questions
**"Too complex to set up?"** → "Start with critical interfaces"
**"How much integration?"** → "Focus on highest-risk interactions"
**"When do I need this?"** → "When modules must work together"

---

**End Goal**: Attendees understand integration testing complements, not replaces, unit testing!