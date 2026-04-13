# 📡 Exercise 3: Sensor Driver with Mocks (3-4 hours)

**Master Hardware Mocking and Embedded Testing Patterns**

## 🎯 Learning Objectives

By the end of this exercise, you will:
- ✅ Mock hardware interfaces confidently
- ✅ Test embedded drivers without physical hardware
- ✅ Handle communication errors and timeouts gracefully
- ✅ Use CMock to simulate complex hardware behaviors
- ✅ Apply dependency injection patterns in embedded code

## 📋 Prerequisites

- Completed Exercise 1 and 2
- Understanding of I2C/SPI protocols (basic level)
- Familiarity with mocking concepts from Demo 2

## 🎮 Exercise Overview

You'll build a complete **Environmental Sensor Driver** for the BME280 (temperature/humidity/pressure sensor) using hardware mocking. You'll test every scenario without touching real I2C hardware.

## 🏗️ What You'll Build

### Sensor Driver Features
- Initialize BME280 sensor over I2C
- Read temperature, humidity, and pressure
- Handle communication failures gracefully
- Implement retry logic for transient errors
- Provide sensor health monitoring
- Support multiple sensor instances

### Hardware Abstraction Layer
You'll create a clean HAL that can be mocked:
```c
// i2c_hal.h - Your hardware abstraction interface
typedef enum {
    I2C_OK,
    I2C_TIMEOUT,
    I2C_NACK,
    I2C_BUS_ERROR,
    I2C_BUSY
} i2c_status_t;

i2c_status_t i2c_write(uint8_t device_addr, uint8_t reg_addr, uint8_t* data, size_t len);
i2c_status_t i2c_read(uint8_t device_addr, uint8_t reg_addr, uint8_t* data, size_t len);
i2c_status_t i2c_init(void);
```

### Sensor Driver Interface
```c
// bme280_driver.h - What you'll implement and test
typedef enum {
    BME280_OK,
    BME280_INIT_FAILED,
    BME280_COMM_ERROR,
    BME280_INVALID_CHIP_ID,
    BME280_NOT_READY,
    BME280_CALIBRATION_ERROR
} bme280_status_t;

typedef struct {
    float temperature_c;
    float humidity_percent;
    float pressure_pa;
    uint32_t timestamp;
    bool valid;
} bme280_reading_t;

// Core functions you'll implement
bme280_status_t bme280_init(uint8_t device_address);
bme280_status_t bme280_read_all(bme280_reading_t* reading);
bme280_status_t bme280_read_temperature(float* temp_c);
bme280_status_t bme280_read_humidity(float* humidity_percent);
bme280_status_t bme280_read_pressure(float* pressure_pa);
bool bme280_is_connected(void);
bme280_status_t bme280_reset(void);
```

## 🧪 Testing Scenarios You'll Master

### 🟢 Happy Path Scenarios
- **Successful initialization** - Sensor responds correctly
- **Valid readings** - Temperature, humidity, pressure data
- **Calibration data** - Proper coefficient loading
- **Multiple reads** - Consistent sensor operation

### 🔴 Error Scenarios
- **I2C communication failures** - Timeout, NACK, bus errors
- **Invalid chip ID** - Wrong sensor or communication failure
- **Sensor not ready** - Measurement in progress
- **Calibration failures** - Invalid calibration data

### ⚡ Advanced Scenarios
- **Retry logic** - Automatic recovery from transient errors
- **Multiple sensors** - Different I2C addresses
- **Power management** - Sleep/wake cycles
- **Performance testing** - Measurement timing validation

## 🎯 Key Mocking Patterns You'll Learn

### Pattern 1: Return Value Control
```c
void test_bme280_init_should_fail_when_i2c_times_out(void) {
    // Arrange - Force I2C to timeout
    i2c_init_ExpectAndReturn(I2C_OK);
    i2c_read_ExpectAndReturn(BME280_ADDR, BME280_CHIP_ID_REG, NULL, 1, I2C_TIMEOUT);
    i2c_read_IgnoreArg_data();

    // Act
    bme280_status_t status = bme280_init(BME280_ADDR);

    // Assert
    TEST_ASSERT_EQUAL(BME280_COMM_ERROR, status);
}
```

### Pattern 2: Output Parameter Control
```c
void test_bme280_init_should_verify_correct_chip_id(void) {
    // Arrange - Return specific chip ID
    uint8_t expected_chip_id = BME280_CHIP_ID;
    i2c_init_ExpectAndReturn(I2C_OK);
    i2c_read_ExpectAndReturn(BME280_ADDR, BME280_CHIP_ID_REG, NULL, 1, I2C_OK);
    i2c_read_IgnoreArg_data();
    i2c_read_ReturnMemThruPtr_data(&expected_chip_id, 1);

    // Act & Assert
    TEST_ASSERT_EQUAL(BME280_OK, bme280_init(BME280_ADDR));
}
```

### Pattern 3: Sequence Testing
```c
void test_bme280_read_should_follow_correct_sequence(void) {
    // Arrange - Expect specific sequence of I2C calls
    i2c_write_ExpectAndReturn(BME280_ADDR, BME280_CTRL_MEAS_REG, NULL, 1, I2C_OK);
    i2c_write_IgnoreArg_data();
    i2c_read_ExpectAndReturn(BME280_ADDR, BME280_STATUS_REG, NULL, 1, I2C_OK);
    i2c_read_IgnoreArg_data();
    i2c_read_ExpectAndReturn(BME280_ADDR, BME280_PRESS_MSB_REG, NULL, 6, I2C_OK);
    i2c_read_IgnoreArg_data();

    // Act
    bme280_reading_t reading;
    bme280_read_all(&reading);

    // CMock automatically verifies the sequence was followed
}
```

## 📚 Exercise Structure

### Phase 1: Hardware Abstraction (45 minutes)
**Goal:** Create mockable I2C interface
- Define I2C HAL interface
- Set up CMock for I2C functions
- Write basic communication tests

### Phase 2: Sensor Initialization (60 minutes)
**Goal:** Initialize BME280 sensor with full error handling
- Test chip ID verification
- Handle I2C communication failures
- Load calibration coefficients
- Reset sensor functionality

### Phase 3: Data Reading (90 minutes)
**Goal:** Read sensor data reliably
- Raw data reading from registers
- Calibration math implementation
- Data conversion and validation
- Combined reading functions

### Phase 4: Error Handling (60 minutes)
**Goal:** Robust error recovery
- Retry logic for transient failures
- Sensor health monitoring
- Timeout handling
- Invalid data detection

### Phase 5: Advanced Features (45 minutes)
**Goal:** Production-ready features
- Multiple sensor support
- Power management
- Performance optimization
- Comprehensive edge case testing

## 🎮 Exercise Approaches

### 🎯 Guided Approach (Recommended for Beginners)
- Follow detailed step-by-step instructions
- Use provided test templates
- Focus on understanding mocking patterns
- **Time:** 3-4 hours

### 🚀 Challenge Approach (For Intermediate)
- Start with requirements only
- Design your own test cases
- Implement advanced error handling
- **Time:** 4-5 hours

### 🏆 Expert Approach (For Advanced)
- Add real BME280 datasheet compliance
- Implement full sensor feature set
- Create production-quality driver
- **Time:** 6-8 hours

## 🛠️ Tools and Resources

### CMock Features You'll Use
- **ExpectAndReturn** - Control function return values
- **IgnoreArg** - Ignore specific parameters
- **ReturnMemThruPtr** - Return data through pointer parameters
- **Callback** - Custom mock behavior
- **Call count verification** - Ensure functions called expected times

### BME280 References
- Simplified register map provided
- Example calibration data sets
- Typical sensor response patterns
- Common error conditions

### Debugging Tools
- Mock verification failures
- Parameter validation helpers
- Test data generators
- Coverage analysis

## 📊 Success Criteria

### Technical Mastery
- [ ] All sensor functions implemented and tested
- [ ] 95%+ test coverage achieved
- [ ] All error paths validated
- [ ] Mocks used effectively throughout

### Embedded Skills
- [ ] Hardware abstraction layer designed cleanly
- [ ] Communication protocols handled correctly
- [ ] Error recovery implemented robustly
- [ ] Production-ready code quality

### Testing Skills
- [ ] Complex mock scenarios implemented
- [ ] Edge cases comprehensively covered
- [ ] Test code is clean and maintainable
- [ ] Mock verification used effectively

## 🎯 Real-World Applications

This exercise teaches patterns you'll use for:

### Communication Protocols
- I2C sensor drivers (accelerometers, gyroscopes, temperature)
- SPI peripheral drivers (displays, ADCs, flash memory)
- UART device communication (GPS, WiFi modules)

### Hardware Abstraction
- GPIO control and monitoring
- Timer and PWM configuration
- ADC reading and calibration
- Interrupt handling

### Error Handling
- Communication timeout recovery
- Device fault detection
- System health monitoring
- Graceful degradation

## 🚨 Common Pitfalls

### Mock Over-Specification
```c
❌ Bad - Too rigid
i2c_read_ExpectAndReturn(0x77, 0xD0, ptr, 1, I2C_OK);  // Hardcoded values

✅ Good - Flexible
i2c_read_ExpectAndReturn(BME280_ADDR, BME280_CHIP_ID_REG, NULL, 1, I2C_OK);
i2c_read_IgnoreArg_data();  // Don't care about data pointer
```

### Testing Implementation Instead of Behavior
```c
❌ Bad - Tests internal details
void test_reads_register_0xF7_exactly_once(void)

✅ Good - Tests external behavior
void test_returns_valid_temperature_when_sensor_ready(void)
```

### Insufficient Error Testing
```c
❌ Bad - Only tests happy path
void test_sensor_reads_temperature(void)

✅ Good - Tests error scenarios
void test_sensor_handles_i2c_timeout_gracefully(void)
void test_sensor_retries_on_transient_failure(void)
```

## 🏆 Bonus Challenges

### Advanced Sensor Features
- **Environmental compensation** - Pressure altitude correction
- **Oversampling control** - Configurable measurement precision
- **IIR filtering** - Hardware-based noise reduction
- **Standby timing** - Configurable measurement intervals

### Multiple Device Support
- **I2C address scanning** - Auto-detect connected sensors
- **Sensor arrays** - Manage multiple BME280 instances
- **Hot-plugging** - Handle sensors connecting/disconnecting
- **Bus arbitration** - Share I2C bus with other devices

### Performance Optimization
- **Bulk reading** - Read all data in single transaction
- **Async operations** - Non-blocking sensor reads
- **Caching** - Avoid unnecessary I2C transactions
- **Power optimization** - Minimal power consumption

---

**Ready to master embedded testing with hardware mocking? Let's build a bulletproof sensor driver!** 🚀