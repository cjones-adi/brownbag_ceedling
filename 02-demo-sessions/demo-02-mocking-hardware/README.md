# 🎭 Demo 2: Mocking Hardware (45 minutes)

**Testing Without Hardware - The Magic of Mocks**

## 🎯 Demo Objectives

By the end of this demo, attendees will:
- ✅ Understand what mocking is and why it's powerful for embedded
- ✅ See CMock generate mocks automatically from header files
- ✅ Watch a complete sensor driver tested without I2C hardware
- ✅ Learn to test error conditions that are hard to reproduce on hardware
- ✅ Recognize the difference between unit testing and hardware validation

## 📋 Prerequisites

**For Instructor:**
- Ceedling with CMock configured
- Demo environment prepared
- I2C sensor example ready

**For Attendees:**
- Attended Demo 1 or familiar with basic unit testing
- Understanding of I2C/SPI protocols helpful but not required
- Embedded development experience

## ⏱️ Demo Timeline

| Time | Activity | Duration |
|------|----------|----------|
| 0-5 min | **Hook & Problem** - The hardware dependency nightmare | 5 min |
| 5-15 min | **Mocking Concepts** - What are mocks and why use them? | 10 min |
| 15-35 min | **Live Demo** - Build BME280 sensor driver with mocks | 20 min |
| 35-40 min | **Error Testing** - Test impossible scenarios | 5 min |
| 40-45 min | **Discussion** - Q&A and real-world applications | 5 min |

## 🎬 Demo Script

### Opening Hook (2 minutes)

> **"Raise your hand if this has happened to you:"**
>
> 📍 **"The PCB is delayed 6 weeks, but software deadline is next week"**
> 📍 **"The sensor works fine on your desk but fails randomly in the field"**
> 📍 **"You need to test I2C timeout handling but can't break the bus reliably"**
>
> **"Today I'll show you how to develop and test sensor drivers before the hardware arrives, and test error conditions you'd never want to create on real hardware."**

### The Classic Problem (3 minutes)

#### The Development Bottleneck
```
Hardware Team: "PCB delayed, sensor not working yet"
    ↓
Software Team: "Can't write driver without hardware"
    ↓
Integration: "Everything breaks when we put it together"
    ↓
Deadline: "Ship it anyway, we'll fix it in the field"
```

#### The Real-World Scenario
> **"We need to interface with a BME280 temperature/humidity sensor over I2C. Requirements:"**

```c
// BME280 Driver Requirements:
// 1. Read temperature in Celsius
// 2. Read humidity percentage
// 3. Handle I2C communication errors gracefully
// 4. Retry on transient failures
// 5. Report sensor status (connected/disconnected)
// 6. Validate sensor ID on startup
```

> **"Traditional approach: Wait for hardware, then debug everything at once."**
> **"Unit testing approach: Test the logic first, integrate later."**

### Part 1: Understanding Mocks (10 minutes)

#### What is a Mock? (3 minutes)

> **"A mock is a fake implementation of a dependency that:"**
> - ✅ **Replaces** real hardware interfaces
> - ✅ **Controls** return values and behaviors
> - ✅ **Verifies** that functions were called correctly
> - ✅ **Simulates** error conditions on demand

#### Mock vs. Real Implementation (4 minutes)

```c
// Real I2C (Production Code)
i2c_status_t i2c_write(uint8_t addr, uint8_t* data, size_t len) {
    // Actual register manipulation
    I2C_DR = addr;
    while (!(I2C_SR1 & I2C_SR1_ADDR));
    // ... complex hardware interaction
}

// Mock I2C (Test Code)
i2c_status_t i2c_write(uint8_t addr, uint8_t* data, size_t len) {
    // Controlled behavior for testing
    return mock_return_value;  // We control what this returns!
}
```

> **"The magic: Our driver code doesn't know the difference!"**

#### Why Mocks are Perfect for Embedded (3 minutes)

**🔧 Development Benefits:**
- Develop without waiting for hardware
- Test faster than hardware (microseconds vs milliseconds)
- Test multiple scenarios quickly

**🧪 Testing Benefits:**
- Force error conditions (I2C_TIMEOUT, I2C_NACK)
- Test boundary conditions exactly
- Verify exact function call sequences

**🔄 Integration Benefits:**
- Know software logic works before integration
- Isolate hardware issues from software issues
- Reduce integration debugging time

### Part 2: Live Demo - BME280 Driver (20 minutes)

#### Step 1: Project Setup (3 minutes)

```bash
# [INSTRUCTOR TYPES LIVE]
mkdir demo_bme280_driver
cd demo_bme280_driver
ceedling new demo_bme280_driver
cd demo_bme280_driver
```

> **"Same setup as Demo 1, but now we'll use mocking!"**

Edit `project.yml` to show mocking is enabled:
```yaml
:project:
  :use_mocks: TRUE
```

#### Step 2: Define Hardware Interface (4 minutes)

```bash
# [INSTRUCTOR CREATES]
vim src/i2c_hal.h
```

```c
#ifndef I2C_HAL_H
#define I2C_HAL_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    I2C_OK = 0,
    I2C_TIMEOUT,
    I2C_NACK,
    I2C_BUSY,
    I2C_ERROR
} i2c_status_t;

// Hardware Abstraction Layer for I2C
i2c_status_t i2c_init(void);
i2c_status_t i2c_write(uint8_t device_addr, uint8_t reg_addr, uint8_t* data, size_t len);
i2c_status_t i2c_read(uint8_t device_addr, uint8_t reg_addr, uint8_t* data, size_t len);

#endif
```

> **"Clean hardware abstraction - this is what we'll mock!"**
> **"Notice: No hardware registers, no platform-specific code."**

#### Step 3: Create Sensor Driver Header (3 minutes)

```bash
vim src/bme280_driver.h
```

```c
#ifndef BME280_DRIVER_H
#define BME280_DRIVER_H

#include <stdint.h>
#include <stdbool.h>

#define BME280_CHIP_ID 0x60
#define BME280_ADDR 0x76

typedef enum {
    BME280_OK = 0,
    BME280_COMM_ERROR,
    BME280_INVALID_CHIP_ID,
    BME280_NOT_INITIALIZED
} bme280_status_t;

// Sensor driver interface
bme280_status_t bme280_init(void);
bme280_status_t bme280_read_temperature(float* temperature);
bme280_status_t bme280_read_humidity(float* humidity);
bool bme280_is_connected(void);

#endif
```

#### Step 4: Write First Test with Mock (6 minutes)

```bash
vim test/test_bme280_driver.c
```

```c
#include "unity.h"
#include "bme280_driver.h"
#include "mock_i2c_hal.h"  // CMock generates this!

void setUp(void) {
    // Fresh state for each test
}

void tearDown(void) {
    // Cleanup mocks
}

void test_bme280_init_should_verify_chip_id_and_return_ok_when_valid(void) {
    // Arrange - Set up mock expectations
    uint8_t expected_chip_id = BME280_CHIP_ID;
    i2c_init_ExpectAndReturn(I2C_OK);
    i2c_read_ExpectAndReturn(BME280_ADDR, 0xD0, NULL, 1, I2C_OK);
    i2c_read_IgnoreArg_data();
    i2c_read_ReturnMemThruPtr_data(&expected_chip_id, 1);

    // Act - Call the function under test
    bme280_status_t result = bme280_init();

    // Assert - Verify result
    TEST_ASSERT_EQUAL(BME280_OK, result);
}
```

> **"Look at the magic happening here:"**
> - **`mock_i2c_hal.h`** - CMock will generate this from our interface!
> - **`i2c_read_ExpectAndReturn`** - We control what I2C returns
> - **`ReturnMemThruPtr_data`** - We provide the chip ID data

#### Step 5: See Mock Generation (2 minutes)

```bash
# [INSTRUCTOR RUNS]
ceedling test:test_bme280_driver
```

> **"ERROR! Let's see what happened..."**
> **"CMock automatically generated `mock_i2c_hal.h` and `mock_i2c_hal.c`!"**

```bash
ls -la build/test/mocks/
cat build/test/mocks/mock_i2c_hal.h
```

> **"CMock read our `i2c_hal.h` and created mock functions that we can control in tests!"**

#### Step 6: Implement Driver (4 minutes)

```bash
vim src/bme280_driver.c
```

```c
#include "bme280_driver.h"
#include "i2c_hal.h"

static bool initialized = false;

bme280_status_t bme280_init(void) {
    // Initialize I2C
    if (i2c_init() != I2C_OK) {
        return BME280_COMM_ERROR;
    }

    // Read and verify chip ID
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

bme280_status_t bme280_read_temperature(float* temperature) {
    if (!initialized) {
        return BME280_NOT_INITIALIZED;
    }

    // Read raw temperature data (simplified)
    uint8_t temp_data[3];
    if (i2c_read(BME280_ADDR, 0xFA, temp_data, 3) != I2C_OK) {
        return BME280_COMM_ERROR;
    }

    // Convert to temperature (simplified calculation)
    int32_t raw_temp = (temp_data[0] << 12) | (temp_data[1] << 4) | (temp_data[2] >> 4);
    *temperature = (float)raw_temp / 1000.0f;  // Simplified conversion

    return BME280_OK;
}

bme280_status_t bme280_read_humidity(float* humidity) {
    if (!initialized) {
        return BME280_NOT_INITIALIZED;
    }

    uint8_t hum_data[2];
    if (i2c_read(BME280_ADDR, 0xFD, hum_data, 2) != I2C_OK) {
        return BME280_COMM_ERROR;
    }

    int32_t raw_hum = (hum_data[0] << 8) | hum_data[1];
    *humidity = (float)raw_hum / 1024.0f;  // Simplified conversion

    return BME280_OK;
}

bool bme280_is_connected(void) {
    return initialized;
}
```

```bash
# [INSTRUCTOR RUNS]
ceedling test:test_bme280_driver
```

> **"GREEN! Our driver works - tested without any I2C hardware!"**

### Part 3: Testing Error Conditions (5 minutes)

#### Test I2C Communication Failure

```c
void test_bme280_init_should_return_comm_error_when_i2c_read_fails(void) {
    // Arrange - Force I2C to fail
    i2c_init_ExpectAndReturn(I2C_OK);
    i2c_read_ExpectAndReturn(BME280_ADDR, 0xD0, NULL, 1, I2C_TIMEOUT);
    i2c_read_IgnoreArg_data();

    // Act
    bme280_status_t result = bme280_init();

    // Assert
    TEST_ASSERT_EQUAL(BME280_COMM_ERROR, result);
}

void test_bme280_init_should_return_invalid_chip_id_when_wrong_id_received(void) {
    // Arrange - Return wrong chip ID
    uint8_t wrong_chip_id = 0x42;  // Not BME280_CHIP_ID (0x60)
    i2c_init_ExpectAndReturn(I2C_OK);
    i2c_read_ExpectAndReturn(BME280_ADDR, 0xD0, NULL, 1, I2C_OK);
    i2c_read_IgnoreArg_data();
    i2c_read_ReturnMemThruPtr_data(&wrong_chip_id, 1);

    // Act
    bme280_status_t result = bme280_init();

    // Assert
    TEST_ASSERT_EQUAL(BME280_INVALID_CHIP_ID, result);
}
```

> **"These error conditions are:**
> - **Hard to create** on real hardware
> - **Easy to test** with mocks
> - **Critical for robustness** in production"**

#### Run Error Tests

```bash
ceedling test:test_bme280_driver
```

> **"All tests pass! We've verified our driver handles:**
> - ✅ **Normal operation** - Read sensor correctly
> - ✅ **I2C timeouts** - Communication failures
> - ✅ **Wrong chip ID** - Hardware validation
> - ✅ **Uninitialized state** - Proper state management"**

### Part 4: Discussion & Q&A (5 minutes)

#### What We Accomplished
> **"In 20 minutes, we:"**
> 1. **Built a complete sensor driver** without touching I2C hardware
> 2. **Tested normal operation** - sensor reading works
> 3. **Tested error conditions** - timeouts, wrong chip IDs
> 4. **Verified state management** - initialization requirements
> 5. **Created 100% reproducible tests** - same results every time

#### Real-World Benefits

**Development Speed:**
> - **No waiting** for hardware prototypes
> - **Instant feedback** - tests run in milliseconds
> - **Parallel development** - HW and SW teams work simultaneously

**Quality Improvement:**
> - **Error path testing** - Force failures safely
> - **Boundary condition testing** - Test exact edge cases
> - **Regression protection** - Changes don't break existing functionality

**Integration Confidence:**
> - **Logic verified** before hardware integration
> - **Error handling proven** before field deployment
> - **Interface contracts** clearly defined and tested

#### Common Questions & Answers

**Q: "Don't mocks just test fake behavior?"**
> **A: "Mocks test real logic with controlled inputs. Integration tests verify the real hardware interface works."**

**Q: "How do I know my mocks match real hardware behavior?"**
> **A: "Integration tests validate this. Unit tests prove your logic handles various hardware responses correctly."**

**Q: "What about timing and real-time requirements?"**
> **A: "Unit tests verify algorithms work. Performance/timing tests verify they meet real-time constraints."**

**Q: "This seems like a lot of extra work?"**
> **A: "Initial setup cost, but massive savings in debugging time and much higher confidence in correctness."**

#### Next Demo Teasers
> **"Coming up:"**
> - **Demo 3: Test-Driven Development** - Write tests first for better design
> - **Demo 4: Boundary Testing** - Systematically test edge cases
> - **Demo 5: Integration Testing** - When and how to test multiple modules together

## 🛠️ Instructor Preparation

### Pre-Demo Setup
- [ ] Ceedling with CMock working
- [ ] Demo directory cleaned
- [ ] Code samples ready as backup
- [ ] BME280 datasheet reference (optional)

### Key Teaching Points

#### Emphasize These Concepts
- **Mocks simulate dependencies, not implementations**
- **Unit tests prove logic correctness**
- **Integration tests prove hardware compatibility**
- **Error testing is easier with mocks**

#### Common Misconceptions to Address
- ❌ "Mocks test fake behavior" → ✅ "Mocks test real logic with controlled inputs"
- ❌ "Unit tests replace hardware testing" → ✅ "Unit tests complement hardware testing"
- ❌ "Mocking is too complex" → ✅ "CMock automates the complexity"

### Demo Tips

#### For Technical Flow
- **Show CMock generation** happening automatically
- **Explain mock expectations** clearly
- **Demonstrate error injection** power
- **Compare test speed** to hardware testing

#### For Engagement
- **Relate to real projects** attendees work on
- **Ask about their hardware delays** experiences
- **Show excitement** when tests pass
- **Encourage questions** throughout

### Backup Plans

**If CMock fails:**
- Have pre-generated mocks ready to copy
- Explain what CMock would have generated
- Continue with demo using backup files

**If compilation errors:**
- Double-check mock function signatures
- Verify include paths are correct
- Use simpler mock expectations if needed

## 📊 Success Metrics

**Demo succeeds if attendees:**
- [ ] Understand what mocks are and why they're useful
- [ ] See the value of testing without hardware dependencies
- [ ] Recognize how to test error conditions safely
- [ ] Want to try mocking in their own projects
- [ ] Ask questions about integration testing

**Follow-up Actions:**
- [ ] Share demo code and project files
- [ ] Provide CMock documentation links
- [ ] Schedule hands-on workshop for practice
- [ ] Collect specific use cases from attendees

---

**Key Message: "Mocks let you develop faster, test more thoroughly, and integrate with confidence - the secret weapon of embedded testing!"**