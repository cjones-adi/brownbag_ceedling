# 🎭 Demo 2: Quick Reference Script

**Total Time: 45 minutes**

## 📝 Opening Hook (5 min)
```
"Raise your hand if this happened:"
- PCB delayed 6 weeks, software deadline next week
- Sensor works on desk, fails randomly in field
- Need to test I2C timeout but can't break bus reliably

"Today: Develop sensor drivers BEFORE hardware arrives"
```

## 🎯 The Problem (3 min)
```
HW Team: "PCB delayed, sensor not working"
SW Team: "Can't write driver without hardware"
Integration: "Everything breaks together"
Deadline: "Ship anyway, fix in field"

BME280 Requirements:
- Read temp/humidity over I2C
- Handle comm errors gracefully
- Retry on failures, validate chip ID
```

## 🧠 Mocking Concepts (7 min)

### What is a Mock?
```
Mock = Fake implementation that:
✅ Replaces real hardware interfaces
✅ Controls return values/behaviors
✅ Verifies function calls
✅ Simulates errors on demand
```

### Real vs Mock I2C
```c
// Real I2C - Complex hardware interaction
i2c_status_t i2c_write(...) {
    I2C_DR = addr;
    while (!(I2C_SR1 & I2C_SR1_ADDR));
    // ... register manipulation
}

// Mock I2C - Controlled for testing
i2c_status_t i2c_write(...) {
    return mock_return_value;  // We control this!
}
```

### Why Perfect for Embedded?
- Develop without hardware delays
- Test faster (microseconds vs milliseconds)
- Force error conditions safely
- Verify exact call sequences

## ⚡ Live Demo Setup (3 min)
```bash
mkdir demo_bme280_driver
cd demo_bme280_driver
ceedling new demo_bme280_driver
cd demo_bme280_driver

# Show project.yml has :use_mocks: TRUE
```

## 🔌 Hardware Interface (4 min)
```c
// src/i2c_hal.h - What we'll mock
typedef enum {
    I2C_OK = 0, I2C_TIMEOUT, I2C_NACK, I2C_BUSY, I2C_ERROR
} i2c_status_t;

i2c_status_t i2c_init(void);
i2c_status_t i2c_write(uint8_t device_addr, uint8_t reg_addr,
                       uint8_t* data, size_t len);
i2c_status_t i2c_read(uint8_t device_addr, uint8_t reg_addr,
                      uint8_t* data, size_t len);
```

## 📡 Sensor Driver Interface (3 min)
```c
// src/bme280_driver.h
#define BME280_CHIP_ID 0x60
#define BME280_ADDR 0x76

typedef enum {
    BME280_OK = 0,
    BME280_COMM_ERROR,
    BME280_INVALID_CHIP_ID,
    BME280_NOT_INITIALIZED
} bme280_status_t;

bme280_status_t bme280_init(void);
bme280_status_t bme280_read_temperature(float* temperature);
bme280_status_t bme280_read_humidity(float* humidity);
bool bme280_is_connected(void);
```

## 🧪 First Test with Mock (6 min)
```c
// test/test_bme280_driver.c
#include "unity.h"
#include "bme280_driver.h"
#include "mock_i2c_hal.h"  // CMock generates this!

void test_bme280_init_should_verify_chip_id_and_return_ok_when_valid(void) {
    // Arrange - Control mock behavior
    uint8_t expected_chip_id = BME280_CHIP_ID;
    i2c_init_ExpectAndReturn(I2C_OK);
    i2c_read_ExpectAndReturn(BME280_ADDR, 0xD0, NULL, 1, I2C_OK);
    i2c_read_IgnoreArg_data();
    i2c_read_ReturnMemThruPtr_data(&expected_chip_id, 1);

    // Act
    bme280_status_t result = bme280_init();

    // Assert
    TEST_ASSERT_EQUAL(BME280_OK, result);
}
```

## 🔴 See Mock Generation (2 min)
```bash
ceedling test:test_bme280_driver  # Will fail but generate mocks

# Show generated files
ls -la build/test/mocks/
cat build/test/mocks/mock_i2c_hal.h
```
**Say:** "CMock read our header and created controllable mock functions!"

## 🟢 Implement Driver (8 min)
```c
// src/bme280_driver.c
#include "bme280_driver.h"
#include "i2c_hal.h"

static bool initialized = false;

bme280_status_t bme280_init(void) {
    if (i2c_init() != I2C_OK) {
        return BME280_COMM_ERROR;
    }

    uint8_t chip_id = 0;
    if (i2c_read(BME280_ADDR, 0xD0, &chip_id, 1) != I2C_OK) {
        return BME280_COMM_ERROR;
    }

    if (chip_id != BME280_CHIP_ID) {
        return BME280_INVALID_CHIP_ID;
    }

    initialized = true;
    return BME280_OK;
}

// ... implement other functions
```

```bash
ceedling test:test_bme280_driver  # GREEN!
```

## ⚠️ Error Testing (5 min)
```c
void test_bme280_init_should_return_comm_error_when_i2c_fails(void) {
    // Force I2C timeout
    i2c_init_ExpectAndReturn(I2C_OK);
    i2c_read_ExpectAndReturn(BME280_ADDR, 0xD0, NULL, 1, I2C_TIMEOUT);
    i2c_read_IgnoreArg_data();

    bme280_status_t result = bme280_init();
    TEST_ASSERT_EQUAL(BME280_COMM_ERROR, result);
}

void test_bme280_init_should_return_invalid_chip_id_when_wrong_id(void) {
    // Return wrong chip ID
    uint8_t wrong_id = 0x42;
    i2c_init_ExpectAndReturn(I2C_OK);
    i2c_read_ExpectAndReturn(BME280_ADDR, 0xD0, NULL, 1, I2C_OK);
    i2c_read_IgnoreArg_data();
    i2c_read_ReturnMemThruPtr_data(&wrong_id, 1);

    bme280_status_t result = bme280_init();
    TEST_ASSERT_EQUAL(BME280_INVALID_CHIP_ID, result);
}
```

**Say:** "These errors are hard to create on real hardware, easy with mocks!"

## 💬 Discussion (4 min)

### What We Accomplished
- ✅ Complete sensor driver without I2C hardware
- ✅ Tested normal operation and error conditions
- ✅ Verified state management and chip validation
- ✅ 100% reproducible tests

### Real Benefits
- **Development**: No hardware delays, instant feedback
- **Quality**: Error path testing, boundary conditions
- **Integration**: Logic verified before HW integration

### Handle Common Questions
**"Mocks test fake behavior?"** → "Mocks test real logic with controlled inputs"
**"How ensure mocks match reality?"** → "Integration tests validate HW interface"
**"Seems like extra work?"** → "Initial cost, massive debugging savings"

---

## 🎯 Instructor Notes

### Key Points to Emphasize
- Mock the **interface**, test the **logic**
- Unit tests **complement**, don't replace HW testing
- Error testing is **much easier** with mocks
- CMock **automates** the mock generation

### Demo Flow Tips
- **Move quickly** through setup (attendees saw this in Demo 1)
- **Spend time** explaining mock expectations
- **Show excitement** when CMock generates files
- **Emphasize speed** - millisecond tests vs hardware delays

### Handle Issues
- **CMock generation fails**: Have backup mocks ready
- **Mock expectation errors**: Simplify the expectations
- **Compilation issues**: Check include paths carefully

---

**End Goal**: Attendees excited about testing without hardware dependencies!