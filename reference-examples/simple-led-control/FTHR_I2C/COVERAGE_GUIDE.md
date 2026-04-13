# Code Coverage with Ceedling - Limitations and Solutions

## Current Issue

```
ERROR: default_gcov_summary >> :executable => `gcov.exe` does not exist in system search paths
```

## Why This Happens

**Code coverage requires actual test execution**, which creates `.gcda` files with runtime coverage data. However:

1. ✅ Your tests **compile successfully** for ARM Cortex-M4
2. ✅ Your tests **link successfully**
3. ❌ Your tests **cannot execute natively** on Windows (they're ARM binaries)
4. ❌ Without execution, **no coverage data is generated**

The current setup uses a wrapper script (`test/support/test_runner_wrapper.bat`) that simulates test execution for **build verification** only.

## Solutions

### Option 1: Use Native GCC for Unit Tests with Coverage (Recommended for Coverage)

Switch to native GCC for unit testing when you need coverage reports:

**1. Create a separate coverage configuration:**

Create `project_coverage.yml`:
```yaml
---
:project:
  :which_ceedling: gem
  :ceedling_version: 1.0.1
  :use_mocks: TRUE
  :build_root: build_coverage
  :test_threads: 8
  :compile_threads: 8

:plugins:
  :enabled:
    - module_generator
    - gcov
    - report_tests_pretty_stdout

:paths:
  :test:
    - +:test/**
    - -:test/support
  :source:
    - src/**
  :include:
    - src/**
  :support:
    - test/support

:defines:
  :test:
    - TEST

:gcov:
  :summaries: TRUE
  :report_task: TRUE
  :utilities:
    - gcovr
  :reports:
    - HtmlBasic
    - Text
  :gcovr:
    :html_medium_threshold: 75
    :html_high_threshold: 90

# Use native GCC (not ARM cross-compiler)
:tools: {}  # Empty = use default GCC
```

**2. Install MinGW or use WSL for native GCC:**
```bash
# Using chocolatey
choco install mingw

# Or use WSL
wsl --install
```

**3. Run coverage with native build:**
```bash
ceedling -C project_coverage.yml gcov:all
```

**Trade-offs:**
- ✅ Get actual coverage reports
- ✅ Fast test execution
- ❌ Won't catch ARM-specific issues
- ❌ Different toolchain than production

### Option 2: Use QEMU for ARM Test Execution with Coverage

Run ARM binaries in QEMU emulator to get real coverage data:

**1. Install QEMU:**
```bash
choco install qemu
```

**2. Update `project.yml`:**
```yaml
:tools:
  :gcov_fixture:
    :executable: qemu-system-arm
    :name: 'QEMU ARM Emulator for Coverage'
    :arguments:
      - -machine mps2-an386
      - -cpu cortex-m4
      - -nographic
      - -semihosting
      - -kernel ${1}
```

**3. Install gcovr:**
```bash
pip install gcovr
```

**4. Run coverage:**
```bash
ceedling gcov:all
```

**Trade-offs:**
- ✅ Uses same ARM toolchain as production
- ✅ Catches ARM-specific issues
- ✅ Real coverage data from execution
- ⏱️ Slower than native execution
- 🔧 More complex setup

### Option 3: Hardware-in-the-Loop Coverage

For the most accurate coverage on actual hardware:

**Requirements:**
- Custom test harness on MAX32655
- UART/SWD output capture
- Post-processing of `.gcda` files from device
- Complex CI/CD integration

**Trade-offs:**
- ✅ Most accurate (real hardware)
- ✅ Catches all hardware interactions
- ❌ Slowest execution
- ❌ Most complex setup
- ❌ Requires physical hardware

### Option 4: Disable Coverage (Current Setup)

Just use build verification without coverage:

```bash
# Run tests (build verification only)
ceedling test:all

# This validates:
# ✅ Code compiles for ARM
# ✅ Tests are well-formed
# ✅ No syntax errors
# ❌ No coverage data
```

To disable gcov, edit `project.yml`:
```yaml
:plugins:
  :enabled:
    # - gcov  # Comment this out
    - module_generator
    - report_tests_pretty_stdout
```

## Recommended Approach

**For Development (Coverage):**
- Use **Option 1** (Native GCC) for quick coverage feedback
- Write portable C code that works on both x86 and ARM
- Mock hardware dependencies

**For CI/CD (Build Verification):**
- Use current ARM build (Option 4) to validate ARM compilation
- Supplement with native coverage tests

**For Release Validation:**
- Use **Option 2** (QEMU) or **Option 3** (Hardware) for final validation

## Example: Dual Configuration Setup

**Directory structure:**
```
FTHR_I2C/
├── project.yml              # ARM build verification
├── project_coverage.yml     # Native GCC with coverage
├── src/
├── test/
└── scripts/
    └── run_both.bat         # Run both configurations
```

**scripts/run_both.bat:**
```batch
@echo off
echo Running Native Tests with Coverage...
ceedling -C project_coverage.yml gcov:all

echo.
echo Running ARM Build Verification...
ceedling -C project.yml test:all

echo.
echo Both test runs complete!
```

## Current Status

Your setup is configured for:
- ✅ ARM Cortex-M4 compilation
- ✅ Build verification
- ❌ Code coverage (requires one of the solutions above)

## Quick Fix to Disable GCov Errors

Edit `project.yml` line 58:
```yaml
:plugins:
  :enabled:
    - module_generator
    # - gcov  # <-- Comment this out
    - report_tests_pretty_stdout
```

Then run:
```bash
ceedling test:all  # Works without gcov errors
```

## Installing Dependencies for Option 1

```bash
# Install MinGW (native GCC for Windows)
choco install mingw

# Install gcovr (coverage report generator)
pip install gcovr

# Verify installation
gcc --version
gcov --version
gcovr --version
```

Then use the separate `project_coverage.yml` configuration for coverage runs.
