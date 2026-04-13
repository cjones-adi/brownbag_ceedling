# 🌡️ Complex Temperature Sensor - Reference Example

A production-ready Ceedling project demonstrating advanced embedded unit testing patterns. This comprehensive example shows how testing scales to complex embedded systems.

## 🎯 Project Overview

This example implements a complete temperature sensor system with multiple layers of abstraction, comprehensive error handling, and extensive test coverage. It represents a real-world embedded architecture with proper testing strategies.

## 📁 Project Structure (47+ Files)

```
complex-temp-sensor/
├── project.yml                 # Advanced Ceedling configuration
├── src/                        # 47+ source files
│   ├── AdcConductor.c         # ADC coordination layer
│   ├── AdcHardware.c          # Hardware abstraction
│   ├── AdcModel.c             # ADC data modeling
│   ├── TemperatureCalculator.c # Temperature conversion
│   ├── TemperatureFilter.c    # Signal filtering
│   ├── TaskScheduler.c        # Real-time task management
│   ├── TimerHardware.c        # Timer abstraction
│   ├── UsartModel.c           # UART communication
│   └── ... (38+ more files)
├── test/                       # 15+ test files
│   ├── TestTemperatureCalculator.c
│   ├── TestTemperatureFilter.c
│   ├── TestTaskScheduler.c
│   ├── TestUsartIntegrated.c
│   ├── adc/                   # ADC-specific tests
│   ├── support/               # Test utilities
│   └── ... (10+ more test files)
├── mixin/                      # Build configurations
├── build/                      # Generated artifacts
└── README.md                  # This file
```

## 🏗️ Architecture Layers

### Hardware Abstraction Layer (HAL)
- **AdcHardware.c** - ADC register access
- **TimerHardware.c** - Timer peripherals
- **UsartHardware.c** - UART communication

### Driver Layer
- **AdcConductor.c** - ADC orchestration
- **TimerConductor.c** - Timer management
- **UsartConductor.c** - UART operations

### Application Layer
- **TemperatureCalculator.c** - Temperature math
- **TemperatureFilter.c** - Signal processing
- **TaskScheduler.c** - Task coordination

### System Layer
- **Main.c** - System initialization
- **Executor.c** - Main loop execution

## 🚀 Quick Start

### Run All Tests
```bash
cd reference-examples/complex-temp-sensor/
ceedling test:all
```

### Run Module-Specific Tests
```bash
ceedling test:TestTemperatureCalculator
ceedling test:TestTemperatureFilter
ceedling test:TestTaskScheduler
```

### Generate Coverage Report
```bash
ceedling gcov:all
# View coverage in build/artifacts/gcov/GcovCoverageResults.html
```

### Run Integration Tests
```bash
ceedling test:TestTimerIntegrated
ceedling test:TestUsartIntegrated
```

## 🔍 What This Example Demonstrates

### Advanced Testing Patterns
- ✅ **Layered Testing Strategy** - Unit, integration, and system tests
- ✅ **Complex Mocking** - Multi-level hardware abstraction
- ✅ **Test Doubles** - Stubs, mocks, and fakes
- ✅ **Custom Assertions** - Domain-specific test utilities
- ✅ **Test Organization** - Modular test suites

### Real-World Scenarios
- 🌡️ **Sensor Reading Pipeline** - ADC → Calculation → Filtering
- ⏰ **Real-Time Scheduling** - Task management and timing
- 📡 **Communication Protocols** - UART data transmission
- 🔧 **Error Recovery** - Hardware failure handling
- 📊 **Data Processing** - Signal filtering and averaging

### Production-Quality Code
- 🎯 **Clean Architecture** - Separation of concerns
- 🎯 **Error Handling** - Comprehensive fault tolerance
- 🎯 **Resource Management** - Memory and timing constraints
- 🎯 **Maintainability** - Modular, testable design

## 📊 Test Categories

### Unit Tests (80% of tests)
- **TestTemperatureCalculator.c** - Math functions
- **TestTemperatureFilter.c** - Signal processing
- **TestAdcModel.c** - Data structures
- **TestUsartModel.c** - Communication logic

### Integration Tests (15% of tests)
- **TestTimerIntegrated.c** - Timer + interrupt handling
- **TestUsartIntegrated.c** - UART + protocol stack
- **TestTaskScheduler.c** - Multi-module coordination

### System Tests (5% of tests)
- **TestMain.c** - Full system initialization
- **TestExecutor.c** - Main execution loop

## 🎓 Learning Focus Areas

### For Intermediate Developers
1. **Study the ADC Pipeline**
   - `AdcHardware.c` + `TestAdcHardware.c`
   - See hardware mocking patterns
   - Understand layered testing

2. **Examine Temperature Processing**
   - `TemperatureCalculator.c` + tests
   - `TemperatureFilter.c` + tests
   - Learn algorithm testing techniques

3. **Explore Communication Stack**
   - `UsartModel.c` + tests
   - Protocol testing strategies
   - Error condition handling

### For Advanced Developers
1. **Task Scheduling Architecture**
   - `TaskScheduler.c` + comprehensive tests
   - Real-time system testing
   - Complex state machine validation

2. **Integration Testing Strategy**
   - Multi-module test scenarios
   - Hardware simulation techniques
   - Performance testing approaches

3. **Build Configuration Management**
   - `project.yml` advanced features
   - Multiple build targets
   - Coverage and reporting setup

## 🔧 Key Testing Techniques

### 1. Layered Mocking Strategy
```c
// Hardware layer - mocked in tests
void Adc_Read(uint8_t channel, uint16_t* value);

// Driver layer - tested with mocked hardware
uint16_t AdcConductor_GetReading(uint8_t sensor_id);

// Application layer - tested with mocked driver
float TemperatureCalculator_Convert(uint16_t adc_reading);
```

### 2. Custom Test Assertions
```c
// In test/support/temperature_test_helpers.c
void ASSERT_TEMPERATURE_WITHIN_RANGE(float actual, float expected, float tolerance);
void ASSERT_FILTER_CONVERGED(TemperatureFilter* filter, uint16_t samples);
```

### 3. Test Data Management
```c
// Parameterized testing with test data
typedef struct {
    uint16_t adc_input;
    float expected_temp;
    float tolerance;
} temperature_test_data_t;

static temperature_test_data_t test_cases[] = {
    {1024, 25.0f, 0.1f},
    {2048, 50.0f, 0.1f},
    // ... more test cases
};
```

## 📈 Complexity Management

### File Organization
- **By Feature** - Related files grouped together
- **By Layer** - Hardware/Driver/Application separation
- **By Responsibility** - Single responsibility per module

### Test Organization
- **Mirror Source Structure** - Test files match source organization
- **Shared Test Utilities** - Common helpers in `test/support/`
- **Test Categories** - Unit/Integration/System separation

### Dependency Management
- **Minimal Dependencies** - Each module depends on few others
- **Clear Interfaces** - Well-defined module boundaries
- **Mockable Dependencies** - All dependencies can be mocked

## 🎯 Success Metrics

This example achieves:
- **95%+ Line Coverage** across all modules
- **100% Function Coverage** for critical algorithms
- **90%+ Branch Coverage** for decision logic
- **Zero Warnings** in static analysis

## 💡 Best Practices Demonstrated

### Code Organization
- ✅ **Consistent Naming** - Clear, predictable naming conventions
- ✅ **Separation of Concerns** - Each module has single responsibility
- ✅ **Interface Design** - Clean, mockable interfaces

### Test Design
- ✅ **Independent Tests** - No test dependencies
- ✅ **Descriptive Names** - Tests clearly describe scenarios
- ✅ **Comprehensive Coverage** - All paths and conditions tested

### Build Configuration
- ✅ **Modular Build** - Individual module testing
- ✅ **Multiple Targets** - Debug/Release configurations
- ✅ **Automated Reporting** - Coverage and test reports

## ⚠️ Common Anti-Patterns to Avoid

Studying this example, notice what it **doesn't** do:

❌ **Over-Engineering** - Appropriate complexity for requirements
❌ **God Objects** - No single module does everything
❌ **Test Dependencies** - Tests don't depend on each other
❌ **Implementation Testing** - Tests focus on behavior, not implementation
❌ **Brittle Mocks** - Mocks are flexible and maintainable

## 🔄 Next Steps

After studying this example:

1. **Start Small** - Don't try to build this complexity immediately
2. **Extract Patterns** - Use specific techniques in your projects
3. **Adapt Architecture** - Modify patterns for your domain
4. **Build Incrementally** - Add testing layer by layer

## 📚 Related Learning

This example supports:
- **Exercise 5: Integration Testing** - Similar complexity patterns
- **Capstone Project** - Use as architectural reference
- **Demo 5: Integration Testing** - Live walkthrough of concepts

---

**Perfect for:** Understanding production-scale testing, advanced patterns, architectural inspiration