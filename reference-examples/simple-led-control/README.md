# 💡 Simple LED Control - Reference Example

A beginner-friendly Ceedling project that demonstrates basic unit testing concepts for embedded systems.

## 🎯 Project Overview

This example implements a simple LED control module with comprehensive unit tests. It's designed to be your first introduction to embedded unit testing patterns.

## 📁 Project Structure

```
simple-led-control/
├── project.yml         # Ceedling configuration
├── src/
│   ├── led_control.h   # LED control interface
│   └── led_control.c   # LED control implementation
├── test/
│   └── test_led_control.c  # Unit tests
├── build/              # Generated build artifacts
└── mixin/              # Additional build configurations
```

## 🔍 What This Example Demonstrates

### Core Testing Concepts
- ✅ **Basic Unity Assertions** - `TEST_ASSERT_EQUAL`, `TEST_ASSERT_TRUE`
- ✅ **Test Organization** - Setup, test cases, teardown
- ✅ **Simple Mocking** - Hardware abstraction for testability
- ✅ **Test Coverage** - Ensuring all code paths are tested

### Embedded-Specific Patterns
- 🎯 **Hardware Abstraction** - Separating logic from hardware
- 🎯 **State Management** - Testing LED state tracking
- 🎯 **Error Handling** - Validating invalid input handling

## 🚀 Quick Start

### Run All Tests
```bash
cd reference-examples/simple-led-control/
ceedling test:all
```

### Run Specific Test
```bash
ceedling test:test_led_control
```

### Generate Coverage Report
```bash
ceedling gcov:all
```

### Clean Build Artifacts
```bash
ceedling clean
```

## 📋 Test Scenarios Covered

### ✅ Basic Functionality
- LED initialization
- LED turn on/off operations
- LED state querying

### ✅ Edge Cases
- Invalid LED numbers
- Repeated operations
- Boundary conditions

### ✅ Error Conditions
- Out-of-range LED indices
- Invalid state parameters

## 🔧 Key Learning Points

### 1. Test Structure
```c
void setUp(void) {
    // Initialize before each test
    led_control_init();
}

void tearDown(void) {
    // Cleanup after each test
}

void test_led_turn_on_should_set_led_state_high(void) {
    // Arrange
    uint8_t led_number = 1;

    // Act
    led_control_turn_on(led_number);

    // Assert
    TEST_ASSERT_TRUE(led_control_is_on(led_number));
}
```

### 2. Hardware Abstraction
The LED control module doesn't directly manipulate GPIO registers. Instead, it uses an abstraction layer that can be mocked for testing:

```c
// In production: calls actual GPIO functions
// In tests: calls mocked functions
void gpio_set_pin_high(uint8_t pin);
void gpio_set_pin_low(uint8_t pin);
```

### 3. Testable Design
- **Separation of Concerns** - Logic separated from hardware
- **Pure Functions** - Predictable inputs and outputs
- **State Management** - Clear state transitions

## 📊 Coverage Analysis

After running `ceedling gcov:all`, check:
- **Line Coverage** - Which lines of code were executed
- **Branch Coverage** - Which decision branches were taken
- **Function Coverage** - Which functions were called

Target: **100% line coverage** for this simple example.

## 🎓 Learning Exercises

### Beginner
1. **Add a new test** for LED blinking functionality
2. **Modify a test** to see it fail, then fix it
3. **Add boundary testing** for maximum LED numbers

### Intermediate
1. **Implement LED PWM** control with corresponding tests
2. **Add error reporting** functionality with tests
3. **Create integration tests** with multiple LEDs

## 🔗 Related Concepts

This example introduces concepts used in:
- Exercise 1: First Test (similar structure)
- Demo 1: Simple Start (same project)
- Exercise 3: Sensor Driver (hardware abstraction)

## ❗ Common Pitfalls to Avoid

### ❌ Don't Do This
- Testing implementation details instead of behavior
- Writing tests that depend on execution order
- Mocking everything (over-mocking)

### ✅ Do This Instead
- Test the public interface behavior
- Make each test independent
- Mock only external dependencies

## 🔄 Next Steps

After understanding this example:
1. **Try the exercises** in `03-hands-on-exercises/`
2. **Study the complex example** in `complex-temp-sensor/`
3. **Start your own project** using this as a template

---

**Perfect for:** First-time embedded testing, learning basic patterns, teaching others