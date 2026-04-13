# 🔗 Exercise 5: Integration Testing (4-6 hours)

**Master Testing Complex System Interactions**

## 🎯 Learning Objectives

By the end of this exercise, you will:
- ✅ Design and implement effective integration tests
- ✅ Balance unit tests with integration tests appropriately
- ✅ Test complex multi-module interactions
- ✅ Handle integration testing challenges in embedded systems
- ✅ Apply integration testing patterns to real-world scenarios

## 📋 Prerequisites

- Completed Exercises 1-4
- Understanding of software architecture patterns
- Experience with mocking and dependency injection
- Familiarity with embedded system design

## 🎮 Exercise Overview

You'll build and test a **Smart Thermostat System** that integrates multiple subsystems. This exercise focuses on testing how these modules work together, not just individually.

## 🏗️ System Architecture

```
┌─────────────────────────────────────────────────────────┐
│                 Smart Thermostat System                 │
├─────────────────┬─────────────────┬─────────────────────┤
│  Sensor Layer   │  Control Layer  │  Interface Layer    │
├─────────────────┼─────────────────┼─────────────────────┤
│ • Temperature   │ • PID Controller│ • Display Driver    │
│ • Humidity      │ • Schedule Mgr  │ • Button Handler    │
│ • Occupancy     │ • Safety Monitor│ • WiFi Interface    │
│ • Ambient Light │ • Mode Manager  │ • Mobile App API    │
└─────────────────┴─────────────────┴─────────────────────┘
                          │
                    ┌─────────────┐
                    │ HVAC System │
                    │ • Heating   │
                    │ • Cooling   │
                    │ • Fan       │
                    └─────────────┘
```

## 🧪 Integration Testing Levels

### Level 1: Module Integration (Component Testing)
Test how closely related modules work together:
- Sensor + Filter integration
- Controller + Safety Monitor integration
- Display + Button Handler integration

### Level 2: Subsystem Integration (Service Testing)
Test how subsystems collaborate:
- Sensor Layer → Control Layer data flow
- Control Layer → Interface Layer updates
- Interface Layer → Control Layer commands

### Level 3: System Integration (End-to-End Testing)
Test complete user scenarios:
- Temperature setpoint change → HVAC response
- Schedule activation → mode transitions
- Emergency scenarios → safety responses

## 🏗️ Core System Components

### Sensor Management Subsystem
```c
typedef struct {
    float temperature_c;
    float humidity_percent;
    bool occupancy_detected;
    uint16_t light_level;
    uint32_t timestamp;
    bool sensors_healthy;
} sensor_data_t;

typedef struct {
    sensor_data_t current_reading;
    sensor_data_t filtered_reading;
    bool calibration_valid;
    uint32_t last_update_time;
} sensor_manager_t;

// Functions to implement and test
void sensor_manager_init(sensor_manager_t* mgr);
bool sensor_manager_update(sensor_manager_t* mgr);
sensor_data_t sensor_manager_get_data(const sensor_manager_t* mgr);
bool sensor_manager_is_healthy(const sensor_manager_t* mgr);
```

### Climate Control Subsystem
```c
typedef enum {
    HVAC_OFF,
    HVAC_HEATING,
    HVAC_COOLING,
    HVAC_FAN_ONLY,
    HVAC_AUTO
} hvac_mode_t;

typedef struct {
    float setpoint_temperature;
    float current_temperature;
    hvac_mode_t current_mode;
    float heating_output_percent;
    float cooling_output_percent;
    bool fan_running;
} climate_control_t;

// Functions to implement and test
void climate_control_init(climate_control_t* ctrl);
void climate_control_set_target(climate_control_t* ctrl, float temp);
void climate_control_update(climate_control_t* ctrl, sensor_data_t sensor_data);
hvac_mode_t climate_control_get_mode(const climate_control_t* ctrl);
```

### User Interface Subsystem
```c
typedef enum {
    UI_EVENT_TEMP_UP,
    UI_EVENT_TEMP_DOWN,
    UI_EVENT_MODE_CHANGE,
    UI_EVENT_SCHEDULE_TOGGLE,
    UI_EVENT_WIFI_CONNECT
} ui_event_t;

typedef struct {
    float display_temperature;
    float display_setpoint;
    hvac_mode_t display_mode;
    bool schedule_active;
    bool wifi_connected;
    uint32_t last_interaction_time;
} user_interface_t;

// Functions to implement and test
void user_interface_init(user_interface_t* ui);
void user_interface_update_display(user_interface_t* ui, climate_control_t* ctrl);
ui_event_t user_interface_process_input(user_interface_t* ui);
bool user_interface_is_idle(const user_interface_t* ui, uint32_t current_time);
```

## 🧪 Integration Testing Patterns

### Pattern 1: Data Flow Testing
Test that data flows correctly between modules:

```c
void test_sensor_data_should_flow_to_climate_control(void)
{
    // Arrange
    sensor_manager_t sensor_mgr;
    climate_control_t climate_ctrl;

    sensor_manager_init(&sensor_mgr);
    climate_control_init(&climate_ctrl);

    // Simulate sensor reading
    mock_temperature_sensor_set_reading(22.5);

    // Act - Update both systems
    sensor_manager_update(&sensor_mgr);
    sensor_data_t sensor_data = sensor_manager_get_data(&sensor_mgr);
    climate_control_update(&climate_ctrl, sensor_data);

    // Assert - Verify data propagated
    TEST_ASSERT_EQUAL_FLOAT(22.5, climate_ctrl.current_temperature);
}
```

### Pattern 2: Event Chain Testing
Test that events cascade correctly through the system:

```c
void test_user_temp_change_should_trigger_hvac_response(void)
{
    // Arrange - Full system setup
    setup_complete_thermostat_system(&system);
    set_current_temperature(20.0);

    // Act - User increases setpoint
    user_interface_process_event(&system.ui, UI_EVENT_TEMP_UP);
    system_process_cycle(&system);

    // Assert - Verify chain reaction
    TEST_ASSERT_EQUAL_FLOAT(21.0, system.climate.setpoint_temperature);
    TEST_ASSERT_EQUAL(HVAC_HEATING, system.climate.current_mode);
    TEST_ASSERT_TRUE(system.climate.fan_running);
}
```

### Pattern 3: State Synchronization Testing
Test that multiple modules maintain consistent state:

```c
void test_system_state_should_remain_consistent_across_modules(void)
{
    // Arrange
    setup_complete_system(&system);

    // Act - Perform various operations
    simulate_temperature_change(&system, 18.0);  // Cold
    simulate_user_setpoint_change(&system, 22.0);  // Increase setpoint
    simulate_time_passage(&system, 5 * 60);  // 5 minutes

    // Assert - Check state consistency
    TEST_ASSERT_EQUAL_FLOAT(22.0, system.ui.display_setpoint);
    TEST_ASSERT_EQUAL_FLOAT(22.0, system.climate.setpoint_temperature);
    TEST_ASSERT_EQUAL(HVAC_HEATING, system.climate.current_mode);
    TEST_ASSERT_EQUAL(HVAC_HEATING, system.ui.display_mode);
}
```

## 📊 Testing Strategy by Integration Level

### Level 1: Module Integration Tests
Focus on immediate neighbors:

```c
// Test sensor manager + data filter integration
void test_sensor_filter_integration(void)
{
    // Test that filtered data is properly calculated
    // Test that outlier rejection works
    // Test that filter adapts to sensor noise
}

// Test climate controller + safety monitor integration
void test_climate_safety_integration(void)
{
    // Test safety limits override controller output
    // Test emergency shutoff scenarios
    // Test safety alarm triggering
}
```

### Level 2: Subsystem Integration Tests
Focus on cross-layer communication:

```c
// Test sensor subsystem → control subsystem
void test_sensor_to_control_integration(void)
{
    // Test sensor data drives control decisions
    // Test sensor failures trigger safe modes
    // Test sensor calibration affects control accuracy
}

// Test control subsystem → interface subsystem
void test_control_to_interface_integration(void)
{
    // Test control state updates display
    // Test control alarms trigger UI alerts
    // Test control mode changes update UI
}
```

### Level 3: System Integration Tests
Focus on end-to-end scenarios:

```c
// Test complete user interaction scenarios
void test_complete_heating_scenario(void)
{
    // Cold house (15°C), user sets 22°C
    // System should: detect temp, start heating, monitor progress, reach target
    // Test includes: sensors, controller, HVAC, UI feedback
}

void test_complete_emergency_scenario(void)
{
    // Overheating scenario (30°C), safety limits (25°C)
    // System should: detect overheat, emergency stop, alert user, require reset
    // Test includes: all subsystems coordinating for safety
}
```

## 🎯 Exercise Phases

### Phase 1: Component Integration (90 minutes)
**Focus:** Test closely coupled modules
- Sensor + Filter integration
- Controller + Safety Monitor integration
- UI + Network interface integration
- Mock external dependencies only

### Phase 2: Data Flow Integration (75 minutes)
**Focus:** Test data propagation through layers
- Sensor data → Control algorithms
- Control decisions → HVAC hardware
- UI events → System responses
- Error conditions → User feedback

### Phase 3: State Consistency Integration (60 minutes)
**Focus:** Test system-wide state management
- Mode changes propagate correctly
- Setpoint changes sync across modules
- Error states are consistent
- Recovery scenarios work properly

### Phase 4: Timing Integration (75 minutes)
**Focus:** Test time-dependent interactions
- Update cycle coordination
- Timeout handling between modules
- Schedule-driven mode changes
- Sensor sampling synchronization

### Phase 5: Failure Integration (60 minutes)
**Focus:** Test system behavior under failures
- Sensor failure → graceful degradation
- Communication loss → safe defaults
- Hardware failure → emergency procedures
- Recovery after fault resolution

## 🛠️ Integration Testing Tools

### System Test Harness
```c
typedef struct {
    sensor_manager_t sensor_mgr;
    climate_control_t climate_ctrl;
    user_interface_t ui;
    schedule_manager_t scheduler;
    safety_monitor_t safety;

    // Mock hardware interfaces
    mock_temperature_sensor_t temp_sensor;
    mock_hvac_hardware_t hvac_hw;
    mock_display_t display;

    // Test control
    uint32_t simulated_time;
    bool real_time_mode;
} thermostat_test_system_t;

// Test harness functions
void test_system_init(thermostat_test_system_t* system);
void test_system_advance_time(thermostat_test_system_t* system, uint32_t ms);
void test_system_inject_sensor_data(thermostat_test_system_t* system, sensor_data_t data);
void test_system_verify_state(thermostat_test_system_t* system);
```

### Mock Coordination
```c
// Coordinate multiple mocks for realistic scenarios
void setup_cold_house_scenario(thermostat_test_system_t* system)
{
    mock_temperature_sensor_set_reading(15.0);
    mock_humidity_sensor_set_reading(45.0);
    mock_occupancy_sensor_set_detected(true);
    mock_hvac_hardware_set_capabilities(HVAC_CAN_HEAT | HVAC_CAN_COOL);
}

void setup_summer_cooling_scenario(thermostat_test_system_t* system)
{
    mock_temperature_sensor_set_reading(28.0);
    mock_humidity_sensor_set_reading(65.0);
    mock_outdoor_temp_set(35.0);
    mock_hvac_hardware_set_efficiency(0.8);  // Reduced efficiency in heat
}
```

### Integration Test Data
```c
// Realistic test data sets
typedef struct {
    const char* scenario_name;
    sensor_data_t initial_conditions;
    float target_temperature;
    uint32_t test_duration_ms;
    hvac_mode_t expected_final_mode;
} integration_test_scenario_t;

static const integration_test_scenario_t scenarios[] = {
    {
        "Cold Morning Warmup",
        {.temperature_c = 16.0, .humidity_percent = 40.0, .occupancy_detected = true},
        21.0,
        15 * 60 * 1000,  // 15 minutes
        HVAC_HEATING
    },
    {
        "Hot Afternoon Cooldown",
        {.temperature_c = 26.0, .humidity_percent = 60.0, .occupancy_detected = true},
        22.0,
        20 * 60 * 1000,  // 20 minutes
        HVAC_COOLING
    }
    // More scenarios...
};
```

## 🎯 Success Criteria

### Integration Coverage
- [ ] All major data flows tested
- [ ] All subsystem interactions validated
- [ ] All user scenarios covered
- [ ] All failure modes tested

### System Quality
- [ ] End-to-end functionality verified
- [ ] Performance requirements met
- [ ] Safety requirements validated
- [ ] User experience tested

### Test Architecture
- [ ] Integration tests are maintainable
- [ ] Test data is realistic and comprehensive
- [ ] Test execution is reliable and repeatable
- [ ] Integration issues are caught effectively

## 🚨 Integration Testing Pitfalls

### Over-Mocking
```c
❌ Bad - Mock everything (becomes unit test)
mock_sensor_manager_get_data_ExpectAndReturn(&sensor_data);
mock_climate_control_update_Expect(&ctrl, sensor_data);
mock_ui_update_display_Expect(&ui, &ctrl);

✅ Good - Mock only external dependencies
// Use real sensor_manager, climate_control, ui
// Mock only hardware interfaces (I2C, display, HVAC)
```

### Under-Integration
```c
❌ Bad - Test modules in isolation
void test_sensor_manager_alone(void)
void test_climate_control_alone(void)

✅ Good - Test realistic interactions
void test_sensor_to_climate_integration(void)
void test_complete_temperature_regulation_loop(void)
```

### Ignoring Timing
```c
❌ Bad - Ignore real-world timing constraints
// Call functions in sequence without timing consideration

✅ Good - Test realistic timing scenarios
// Advance simulated time between operations
// Test behavior under timing pressure
// Verify timeout handling
```

## 🏆 Advanced Integration Challenges

### Multi-Threading Integration
- Test thread communication
- Validate shared resource access
- Test synchronization mechanisms
- Handle race conditions

### Network Integration
- Test WiFi connectivity scenarios
- Validate cloud service integration
- Handle network failures gracefully
- Test over-the-air updates

### Power Management Integration
- Test low-power mode transitions
- Validate wake-up scenarios
- Test battery backup behavior
- Handle power loss/recovery

## 📈 Integration Testing Metrics

### Coverage Metrics
- **Interface Coverage** - All module interfaces exercised
- **Scenario Coverage** - All user scenarios tested
- **Failure Coverage** - All failure modes tested
- **Timing Coverage** - All timing scenarios tested

### Quality Metrics
- **Integration Defect Rate** - Bugs found in integration vs unit testing
- **Test Execution Time** - Time to run full integration suite
- **Test Reliability** - Percentage of stable test results
- **Requirements Traceability** - Integration tests to requirements mapping

---

**Ready to master the art of testing system interactions? Let's build bulletproof integration tests!** 🔗