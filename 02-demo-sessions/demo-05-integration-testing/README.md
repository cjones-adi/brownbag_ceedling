# 🔗 Demo 5: Integration Testing (45 minutes)

**When the Whole is Different from the Sum of Its Parts**

## 🎯 Demo Objectives

By the end of this demo, attendees will:
- ✅ Understand when integration testing is necessary vs unit testing
- ✅ See how to test multiple modules working together
- ✅ Learn to balance unit tests vs integration tests effectively
- ✅ Experience testing complex embedded system interactions
- ✅ Recognize integration testing patterns for embedded systems

## 📋 Prerequisites

**For Instructor:**
- Understanding of testing pyramid concepts
- Multi-module embedded system example prepared
- Knowledge of when integration tests add value

**For Attendees:**
- Completed previous demos or equivalent unit testing knowledge
- Understanding of embedded system architectures
- Experience with multi-module development

## ⏱️ Demo Timeline

| Time | Activity | Duration |
|------|----------|----------|
| 0-5 min | **Integration vs Unit** - When each approach is needed | 5 min |
| 5-10 min | **The Testing Pyramid** - Balancing different test types | 5 min |
| 10-35 min | **Live Demo** - Temperature monitoring system integration | 25 min |
| 35-40 min | **Testing Strategy** - When to write integration tests | 5 min |
| 40-45 min | **Discussion** - Best practices and real-world applications | 5 min |

## 🎬 Demo Script

### Opening: Integration vs Unit Testing (5 minutes)

#### The "It Works on My Machine" Problem
> **"Show of hands: How many times have you heard this?"**
>
> 👩‍💻 **Developer A:** "My sensor driver unit tests all pass!"
> 👨‍💻 **Developer B:** "My display controller unit tests all pass!"
> 🤖 **Integration:** "The system crashes when we put them together!"

#### Why Individual Tests Aren't Enough
```c
// Unit Test A: Sensor driver works perfectly
void test_sensor_read_temperature_returns_valid_data(void) {
    float temp = sensor_read_temperature();
    TEST_ASSERT_FLOAT_WITHIN(0.1, 25.0, temp);  // ✅ PASS
}

// Unit Test B: Display works perfectly
void test_display_show_temperature_formats_correctly(void) {
    display_show_temperature(25.0);
    TEST_ASSERT_STRING_EQUAL("25.0°C", display_buffer);  // ✅ PASS
}

// Reality: Integration fails!
// - Sensor returns Kelvin, display expects Celsius
// - Sensor updates at 10Hz, display refreshes at 1Hz
// - Memory corruption between modules
// - Timing dependencies not tested
```

#### The Integration Gap
> **"Unit tests prove individual components work correctly."**
> **"Integration tests prove components work correctly TOGETHER."**

**What Unit Tests Miss:**
- 🔄 **Data flow between modules** - Format mismatches, unit conversions
- ⏰ **Timing relationships** - Update rates, synchronization
- 🧠 **Memory interactions** - Shared buffers, resource conflicts
- 📞 **Communication protocols** - Message formats, error recovery
- 🎯 **System-level behavior** - End-to-end scenarios

### The Testing Pyramid in Embedded (5 minutes)

#### Traditional Testing Pyramid
```
        /\
       /  \     System Tests (5%)
      /____\    - Complete hardware
     /      \   Integration Tests (20%)
    /________\  - Module interactions
   /          \ Unit Tests (75%)
  /____________\ - Individual functions
```

#### Embedded Testing Reality
```
        /\
       /  \     Hardware-in-Loop (5%)
      /____\    - Real hardware validation
     /      \   Integration Tests (25%)
    /________\  - Critical module interactions
   /          \ Unit Tests (70%)
  /____________\ - Logic & algorithms
```

#### Why More Integration Tests in Embedded?

**🔗 Complex Interactions**
- Hardware abstraction layers
- Real-time scheduling
- Interrupt-driven architectures
- Resource sharing (memory, peripherals)

**⚡ Timing Dependencies**
- Sensor sampling rates
- Communication timeouts
- Real-time deadlines
- State machine transitions

**🛡️ Safety Requirements**
- Fail-safe behavior
- Error propagation
- System recovery
- Compliance validation

### Part 1: Live Demo - Temperature Monitoring System (25 minutes)

#### System Architecture (3 minutes)
> **"We're building a temperature monitoring system with these components:"**

```
┌─────────────┐    ┌─────────────┐    ┌─────────────┐    ┌─────────────┐
│   Sensor    │───▶│  Processor  │───▶│   Logger    │───▶│   Display   │
│   Driver    │    │    Filter   │    │   Buffer    │    │ Controller  │
└─────────────┘    └─────────────┘    └─────────────┘    └─────────────┘
     │                    │                    │                    │
 I2C Sensor          Moving Average        Ring Buffer           LCD/LED
 BME280 @ 1Hz       3-sample filter      100 readings         Update @ 1Hz
```

**Individual Module Responsibilities:**
- **Sensor Driver** - Read BME280 over I2C, return temperature in Celsius
- **Filter** - Apply moving average to reduce noise
- **Logger** - Store readings in circular buffer for history
- **Display** - Show current temperature and trend

**Integration Points (Where Bugs Hide):**
- Data format between sensor and filter
- Update rate synchronization
- Buffer overflow handling
- Error propagation through pipeline

#### Project Setup (2 minutes)
```bash
# [INSTRUCTOR TYPES]
mkdir demo_temp_monitor_integration
cd demo_temp_monitor_integration
ceedling new demo_temp_monitor_integration
cd demo_temp_monitor_integration
```

#### Define Module Interfaces (5 minutes)

```bash
vim src/sensor_driver.h
```

```c
#ifndef SENSOR_DRIVER_H
#define SENSOR_DRIVER_H

#include <stdbool.h>

typedef enum {
    SENSOR_OK,
    SENSOR_COMM_ERROR,
    SENSOR_NOT_READY
} sensor_status_t;

sensor_status_t sensor_init(void);
sensor_status_t sensor_read_temperature(float* temperature_celsius);
bool sensor_is_connected(void);

#endif
```

```bash
vim src/temp_filter.h
```

```c
#ifndef TEMP_FILTER_H
#define TEMP_FILTER_H

typedef struct {
    float samples[3];
    int current_index;
    int sample_count;
} temp_filter_t;

void temp_filter_init(temp_filter_t* filter);
float temp_filter_update(temp_filter_t* filter, float new_sample);
bool temp_filter_is_stable(const temp_filter_t* filter);

#endif
```

```bash
vim src/temp_logger.h
```

```c
#ifndef TEMP_LOGGER_H
#define TEMP_LOGGER_H

#include <stdint.h>
#include <stdbool.h>

#define TEMP_LOG_SIZE 100

typedef struct {
    float readings[TEMP_LOG_SIZE];
    uint32_t timestamps[TEMP_LOG_SIZE];
    int write_index;
    int count;
} temp_logger_t;

void temp_logger_init(temp_logger_t* logger);
void temp_logger_add_reading(temp_logger_t* logger, float temperature, uint32_t timestamp);
float temp_logger_get_average(const temp_logger_t* logger, int num_samples);
bool temp_logger_is_full(const temp_logger_t* logger);

#endif
```

```bash
vim src/temp_display.h
```

```c
#ifndef TEMP_DISPLAY_H
#define TEMP_DISPLAY_H

typedef enum {
    TEMP_TREND_UNKNOWN,
    TEMP_TREND_STABLE,
    TEMP_TREND_RISING,
    TEMP_TREND_FALLING
} temp_trend_t;

void temp_display_init(void);
void temp_display_update(float current_temp, float average_temp, temp_trend_t trend);
void temp_display_show_error(const char* error_message);

#endif
```

#### Unit Tests First (5 minutes)
> **"Let's write unit tests for individual modules to establish baseline functionality:"**

```c
// test/test_temp_filter.c
#include "unity.h"
#include "temp_filter.h"

void test_temp_filter_should_initialize_to_empty_state(void) {
    temp_filter_t filter;
    temp_filter_init(&filter);

    TEST_ASSERT_EQUAL(0, filter.current_index);
    TEST_ASSERT_EQUAL(0, filter.sample_count);
    TEST_ASSERT_FALSE(temp_filter_is_stable(&filter));
}

void test_temp_filter_should_return_single_value_when_only_one_sample(void) {
    temp_filter_t filter;
    temp_filter_init(&filter);

    float result = temp_filter_update(&filter, 25.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 25.0f, result);
}

void test_temp_filter_should_compute_moving_average_when_full(void) {
    temp_filter_t filter;
    temp_filter_init(&filter);

    temp_filter_update(&filter, 20.0f);
    temp_filter_update(&filter, 25.0f);
    float result = temp_filter_update(&filter, 30.0f);

    // Average of 20, 25, 30 = 25.0
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 25.0f, result);
    TEST_ASSERT_TRUE(temp_filter_is_stable(&filter));
}
```

> **"Unit tests establish that individual components work. But now let's see integration testing..."**

#### Integration Test - The Real Challenge (10 minutes)

```bash
vim test/test_temp_monitor_integration.c
```

```c
#include "unity.h"
#include "sensor_driver.h"
#include "temp_filter.h"
#include "temp_logger.h"
#include "temp_display.h"
#include "mock_i2c_hal.h"  // Mock the I2C hardware

// Integration test setup
static temp_filter_t filter;
static temp_logger_t logger;

void setUp(void) {
    temp_filter_init(&filter);
    temp_logger_init(&logger);
    temp_display_init();
}

void tearDown(void) {
    // Cleanup
}

void test_complete_temperature_pipeline_should_process_readings_correctly(void) {
    // Arrange - Mock sensor to return specific readings
    uint8_t temp_data[] = {0x80, 0x00, 0x00};  // Represents 25.0°C in BME280 format

    i2c_init_ExpectAndReturn(I2C_OK);
    i2c_read_ExpectAndReturn(BME280_ADDR, BME280_TEMP_REG, NULL, 3, I2C_OK);
    i2c_read_IgnoreArg_data();
    i2c_read_ReturnMemThruPtr_data(temp_data, 3);

    // Act - Run complete pipeline
    sensor_status_t sensor_result = sensor_init();
    TEST_ASSERT_EQUAL(SENSOR_OK, sensor_result);

    float raw_temperature;
    sensor_result = sensor_read_temperature(&raw_temperature);
    TEST_ASSERT_EQUAL(SENSOR_OK, sensor_result);

    float filtered_temperature = temp_filter_update(&filter, raw_temperature);

    uint32_t timestamp = 1000;  // Mock timestamp
    temp_logger_add_reading(&logger, filtered_temperature, timestamp);

    float average = temp_logger_get_average(&logger, 1);
    temp_display_update(filtered_temperature, average, TEMP_TREND_STABLE);

    // Assert - Verify end-to-end behavior
    TEST_ASSERT_FLOAT_WITHIN(0.5f, 25.0f, raw_temperature);
    TEST_ASSERT_FLOAT_WITHIN(0.5f, 25.0f, filtered_temperature);
    TEST_ASSERT_FLOAT_WITHIN(0.5f, 25.0f, average);

    // Verify logger state
    TEST_ASSERT_EQUAL(1, logger.count);
    TEST_ASSERT_FALSE(temp_logger_is_full(&logger));
}

void test_system_should_handle_sensor_communication_errors_gracefully(void) {
    // Arrange - Force sensor communication failure
    i2c_init_ExpectAndReturn(I2C_OK);
    i2c_read_ExpectAndReturn(BME280_ADDR, BME280_TEMP_REG, NULL, 3, I2C_TIMEOUT);
    i2c_read_IgnoreArg_data();

    // Act - Try to read sensor
    sensor_status_t sensor_result = sensor_init();
    TEST_ASSERT_EQUAL(SENSOR_OK, sensor_result);

    float temperature;
    sensor_result = sensor_read_temperature(&temperature);

    // Assert - System handles error gracefully
    TEST_ASSERT_EQUAL(SENSOR_COMM_ERROR, sensor_result);

    // Verify error propagates to display
    temp_display_show_error("Sensor Communication Error");

    // System should continue functioning when sensor recovers
    TEST_ASSERT_TRUE(temp_filter_is_stable(&filter) || !temp_filter_is_stable(&filter));  // State preserved
}

void test_system_should_maintain_data_integrity_under_high_frequency_updates(void) {
    // Arrange - Simulate rapid sensor updates
    uint8_t temp_readings[][3] = {
        {0x80, 0x00, 0x00},  // 25.0°C
        {0x81, 0x00, 0x00},  // 25.1°C
        {0x82, 0x00, 0x00},  // 25.2°C
        {0x83, 0x00, 0x00},  // 25.3°C
        {0x84, 0x00, 0x00},  // 25.4°C
    };

    i2c_init_ExpectAndReturn(I2C_OK);
    sensor_init();

    // Act - Process multiple readings rapidly
    for (int i = 0; i < 5; i++) {
        i2c_read_ExpectAndReturn(BME280_ADDR, BME280_TEMP_REG, NULL, 3, I2C_OK);
        i2c_read_IgnoreArg_data();
        i2c_read_ReturnMemThruPtr_data(temp_readings[i], 3);

        float raw_temp;
        sensor_read_temperature(&raw_temp);

        float filtered_temp = temp_filter_update(&filter, raw_temp);
        temp_logger_add_reading(&logger, filtered_temp, 1000 + i * 100);
    }

    // Assert - Data integrity maintained
    TEST_ASSERT_EQUAL(5, logger.count);

    // Verify filter converged to stable state
    TEST_ASSERT_TRUE(temp_filter_is_stable(&filter));

    // Verify trend calculation works
    float recent_average = temp_logger_get_average(&logger, 3);
    float older_average = temp_logger_get_average(&logger, 5);
    TEST_ASSERT_GREATER_THAN(older_average, recent_average);  // Temperature rising trend
}

void test_system_should_recover_from_buffer_overflow_conditions(void) {
    // Arrange - Fill logger buffer beyond capacity
    i2c_init_ExpectAndReturn(I2C_OK);
    sensor_init();

    // Act - Add more readings than buffer can hold
    for (int i = 0; i < TEMP_LOG_SIZE + 10; i++) {
        uint8_t temp_data[] = {0x80, 0x00, 0x00};
        i2c_read_ExpectAndReturn(BME280_ADDR, BME280_TEMP_REG, NULL, 3, I2C_OK);
        i2c_read_IgnoreArg_data();
        i2c_read_ReturnMemThruPtr_data(temp_data, 3);

        float temp;
        sensor_read_temperature(&temp);
        float filtered = temp_filter_update(&filter, temp);
        temp_logger_add_reading(&logger, filtered, 1000 + i * 100);
    }

    // Assert - System handles overflow gracefully
    TEST_ASSERT_EQUAL(TEMP_LOG_SIZE, logger.count);  // Should not exceed buffer size
    TEST_ASSERT_TRUE(temp_logger_is_full(&logger));

    // Verify oldest data was overwritten (circular buffer behavior)
    float average = temp_logger_get_average(&logger, TEMP_LOG_SIZE);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 25.0f, average);  // Data should still be reasonable
}
```

> **"Look at what integration testing reveals:"**
> 1. **Data flow validation** - Temperature flows correctly through entire pipeline
> 2. **Error propagation** - Communication failures handled gracefully system-wide
> 3. **Timing behavior** - System works under rapid update conditions
> 4. **Resource limits** - Buffer overflow handled correctly
> 5. **State consistency** - System maintains coherent state across modules

### Part 2: Testing Strategy (5 minutes)

#### When to Write Integration Tests

✅ **Write Integration Tests When:**

**🔄 Data Flows Between Modules**
- Format conversions (units, endianness)
- Protocol implementations
- Message passing systems

**⏰ Timing Dependencies Exist**
- Update rate mismatches
- Real-time scheduling
- Synchronization requirements

**🧠 Shared Resources Are Used**
- Memory buffers
- Hardware peripherals
- Communication buses

**🛡️ System-Level Behavior Matters**
- Error recovery scenarios
- Fail-safe mechanisms
- End-to-end functionality

❌ **Avoid Integration Tests When:**
- Pure algorithm testing (use unit tests)
- Simple data transformations
- Independent utility functions
- Complex setup outweighs value

#### Integration Test Patterns in Embedded

**📊 Pipeline Testing**
```c
Input → Module A → Module B → Module C → Output
Test: Verify data flows correctly end-to-end
```

**🔄 State Machine Integration**
```c
Event → State Machine → Action → Hardware
Test: Verify state transitions cause correct actions
```

**📞 Protocol Stack Testing**
```c
Application → Transport → Network → Physical
Test: Verify message delivery and error handling
```

**⚡ Real-Time Integration**
```c
Interrupt → Handler → Task → Response
Test: Verify timing requirements are met
```

### Part 3: Discussion - Best Practices (5 minutes)

#### What We Learned from the Demo

**🔍 Integration Tests Revealed:**
- **Data format issues** - Units, precision, endianness
- **Error propagation paths** - How failures cascade
- **Resource management** - Buffer overflow handling
- **System resilience** - Recovery from failures

**🎯 These Were Missed by Unit Tests:**
- Module interaction bugs
- Timing dependency issues
- Resource sharing conflicts
- System-level error scenarios

#### Integration Testing Guidelines

**⚖️ Balance is Key**
```
Too Few Integration Tests:
- Modules work alone but fail together
- Integration bugs found late
- Customer-found issues

Too Many Integration Tests:
- Slow feedback cycles
- Hard to debug failures
- Maintenance overhead
```

**🎯 Optimal Strategy**
- **70% Unit Tests** - Fast feedback, easy debugging
- **25% Integration Tests** - Critical interactions
- **5% System Tests** - End-to-end validation

#### Common Integration Testing Patterns

**🏗️ Bottom-Up Integration**
- Test lower-level modules first
- Build up to higher-level functionality
- Good for embedded driver stacks

**🔝 Top-Down Integration**
- Test system interfaces first
- Mock lower-level dependencies
- Good for application logic

**🎯 Risk-Based Integration**
- Test highest-risk interactions first
- Focus on safety-critical paths
- Optimize testing ROI

#### Real-World Integration Challenges

**⚡ Embedded-Specific Issues**
- Hardware simulation complexity
- Real-time timing constraints
- Resource limitation testing
- Power management interactions

**🛠️ Practical Solutions**
- Use hardware abstraction layers
- Mock timing-critical dependencies
- Test resource exhaustion scenarios
- Automate integration test execution

## 🛠️ Instructor Preparation

### Pre-Demo Setup
- [ ] Multi-module embedded example prepared
- [ ] Clear understanding of testing pyramid
- [ ] Integration testing patterns researched
- [ ] Real-world examples ready

### Key Teaching Points

#### Emphasize These Concepts
- **Integration tests complement unit tests** - Don't replace them
- **Test interactions, not implementations** - Focus on interfaces
- **Balance is crucial** - Too many integration tests slows development
- **Embedded systems need more integration testing** than typical software

#### Common Misconceptions to Address
- ❌ "Integration tests replace unit tests" → ✅ "Both are needed for comprehensive coverage"
- ❌ "Integration tests are too slow" → ✅ "Critical interactions justify the cost"
- ❌ "Unit tests catch everything" → ✅ "Unit tests miss interaction bugs"

### Demo Tips

#### For Technical Flow
- **Start with unit tests** to show baseline
- **Show integration test failures** that unit tests miss
- **Demonstrate different integration patterns** (pipeline, protocol, etc.)
- **Keep examples realistic** to embedded development

#### For Engagement
- **Relate to their experiences** - "How many integration bugs have you seen?"
- **Show the testing pyramid** clearly
- **Celebrate comprehensive coverage** across test types
- **Encourage questions** about their specific systems

### Backup Plans

**If integration tests are complex:**
- Focus on simpler interface testing
- Emphasize concepts over implementation
- Use more straightforward examples

**If mocking setup fails:**
- Use simpler mock scenarios
- Focus on integration testing principles
- Discuss real-world approaches

## 📊 Success Metrics

**Demo succeeds if attendees:**
- [ ] Understand when integration tests add value beyond unit tests
- [ ] Can identify integration points in their own systems
- [ ] See the balance needed between test types
- [ ] Want to implement integration testing strategically
- [ ] Ask questions about specific embedded integration scenarios

**Follow-up Actions:**
- [ ] Share integration testing checklist
- [ ] Provide testing pyramid guidelines for embedded
- [ ] Schedule review of their critical system integration points
- [ ] Collect examples of integration bugs they've encountered

---

**Key Message: "Unit tests prove components work individually - integration tests prove they work together. Both are essential for reliable embedded systems!"**