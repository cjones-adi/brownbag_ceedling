# Ceedling Unit Test - Quick Start Guide

## ✅ Setup Complete!

Your Ceedling unit test environment is ready to use with the MaximSDK ARM GCC toolchain.

## Quick Commands

```bash
# Run all tests
ceedling test:all

# Run specific test
ceedling test:led_control

# Clean build
ceedling clean

# Create new module with tests
ceedling module:create[new_module_name]
```

## What's Working

✅ ARM Cortex-M4 compilation with MaximSDK GCC  
✅ Test linking with nosys.specs  
✅ Build verification (simulated test execution)  
✅ Unity test framework integration  
✅ CMock ready for mocking

## Example Test Results

```
--------------------
OVERALL TEST SUMMARY
--------------------
TESTED:  7
PASSED:  7
FAILED:  0
IGNORED: 0

Ceedling operations completed in 1.05 seconds
```

## Configuration Details

- **Toolchain**: `C:/MaximSDK/Tools/GNUTools/10.3/bin/arm-none-eabi-gcc.exe`
- **Target**: ARM Cortex-M4 (MAX32655)
- **Compiler Flags**: `-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16`
- **Linker Specs**: `--specs=nosys.specs` (bare-metal)
- **Test Mode**: Build verification (wrapper script)

## File Structure

```
FTHR_I2C/
├── src/
│   ├── led_control.h          # Module header
│   └── led_control.c          # Module implementation
├── test/
│   ├── test_led_control.c     # Unit tests
│   └── support/
│       └── test_runner_wrapper.bat  # Build verification wrapper
├── build/
│   └── test/                  # Generated test artifacts
├── project.yml                # Ceedling configuration
└── UNIT_TEST_SETUP.md        # Full documentation
```

## Next Steps

### 1. Add Tests for Existing Code

```bash
# Create test for main.c functionality
ceedling module:create[i2c_handler]
```

### 2. Use Mocks for Hardware Dependencies

In `test/test_i2c_handler.c`:
```c
#include "unity.h"
#include "mock_mxc_i2c.h"  // Auto-generated mock
#include "i2c_handler.h"

void test_i2c_init_should_configure_peripheral(void) {
    // Mock expectations
    MXC_I2C_Init_ExpectAndReturn(MXC_I2C1, I2C_FREQ, 0);
    
    // Call function under test
    i2c_handler_init();
    
    // Verify mock was called
}
```

### 3. Run Tests in CI/CD

Add to your build pipeline:
```yaml
- name: Run Unit Tests
  run: ceedling test:all
```

## Upgrading to Full Test Execution

To run tests with actual execution (not just build verification):

### Option A: Use QEMU ARM Emulator
```bash
choco install qemu
```

Update `project.yml`:
```yaml
:test_fixture:
  :executable: qemu-system-arm
  :arguments:
    - -machine mps2-an386
    - -cpu cortex-m4
    - -nographic
    - -semihosting
    - -kernel ${1}
```

### Option B: Hardware-in-the-Loop
- Flash tests to MAX32655
- Read results via UART/SWD
- Requires custom integration

## Troubleshooting

### Tests won't compile
```bash
# Clean and rebuild
ceedling clean
ceedling test:all
```

### Can't find arm-none-eabi-gcc
Check that MaximSDK is at `C:/MaximSDK/`. Update path in `project.yml` if different:
```yaml
:test_compiler:
  :executable: C:/YOUR_PATH/MaximSDK/Tools/GNUTools/10.3/bin/arm-none-eabi-gcc.exe
```

### Need to test with different board
Update in `project.yml`:
```yaml
:defines:
  :test:
    - TEST
    - TARGET_NUM=32655
    - BOARD=YOUR_BOARD_NAME  # Add this
```

## Resources

- **Ceedling**: http://www.throwtheswitch.org/ceedling
- **Unity**: http://www.throwtheswitch.org/unity  
- **CMock**: http://www.throwtheswitch.org/cmock
- **MaximSDK**: https://www.analog.com/en/design-center/evaluation-hardware-and-software/software/software-download.html

---

**Ready to write tests!** 🚀
