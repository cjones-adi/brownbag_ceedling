# 📁 Ceedling Project Setup Guide

> This guide walks through scaffolding a new Ceedling project from scratch and understanding the project structure and configuration.

---

## Step 1 — Create a New Project

```bash
ceedling new my_project
cd my_project
```

Ceedling generates the following structure:

```
my_project/
├── project.yml       ← main configuration file
├── src/              ← your production source code (.c / .h)
└── test/             ← your unit test files (test_*.c)
```

---

## Step 2 — Understand `project.yml`

This single file controls everything. Here is a annotated version of the default:

```yaml
---
# Ceedling Project Configuration

:project:
  :build_root: build/          # where all build artifacts go
  :release_build: FALSE        # set TRUE to also produce a release binary

:paths:
  :test:
    - test/**                  # all test files (must match test_*.c)
  :source:
    - src/**                   # all production source files
  :include:
    - src/**                   # header search paths

:defines:
  :test:
    - UNIT_TEST                # preprocessor symbol available in test builds

:plugins:
  :load_paths:
    - "#{Ceedling.load_path}"
  :enabled:
    - stdout_pretty_tests_report   # human-readable console output
    - module_generator             # optional: `ceedling module:create[name]`
```

---

## Step 3 — Add a Source Module

### Create `src/math_utils.h`
```c
#ifndef MATH_UTILS_H
#define MATH_UTILS_H

int add(int a, int b);
int subtract(int a, int b);

#endif
```

### Create `src/math_utils.c`
```c
#include "math_utils.h"

int add(int a, int b)      { return a + b; }
int subtract(int a, int b) { return a - b; }
```

---

## Step 4 — Write Your First Test

### Create `test/test_math_utils.c`
```c
#include "unity.h"
#include "math_utils.h"

void setUp(void)    {}
void tearDown(void) {}

void test_add_returns_correct_sum(void) {
    TEST_ASSERT_EQUAL_INT(5, add(2, 3));
}

void test_subtract_returns_correct_difference(void) {
    TEST_ASSERT_EQUAL_INT(1, subtract(3, 2));
}
```

> **Naming rules:**
> - Test file must be named `test_<module>.c`
> - Each test function must start with `test_`
> - `setUp()` and `tearDown()` are required (can be empty)

---

## Step 5 — Run the Tests

```bash
ceedling test:all
```

Expected output:
```
--------------------
UNIT TEST SUMMARY
--------------------
[test_math_utils.c]
  Test: test_add_returns_correct_sum        PASS
  Test: test_subtract_returns_correct_difference  PASS
--------------------
TESTED:  2
PASSED:  2
FAILED:  0
IGNORED: 0
```

### Run a single test file
```bash
ceedling test:path[test_math_utils]
```

---

## Step 6 — Enable Mocking with CMock

To mock a module, add it to the `:cmock:` section in `project.yml`:

```yaml
:cmock:
  :mock_prefix: mock_
  :when_no_prototypes: :warn
  :enforce_strict_ordering: TRUE
```

Then in your test file, include the auto-generated mock:
```c
#include "mock_uart_driver.h"   // CMock generates this from uart_driver.h
```

Ceedling detects the `mock_` prefix and auto-generates the mock — no manual step needed.

---

## Step 7 — Enable Code Coverage (Optional)

Add the `gcov` plugin to `project.yml`:

```yaml
:plugins:
  :enabled:
    - stdout_pretty_tests_report
    - gcov
```

Run coverage:
```bash
ceedling gcov:all
```

An HTML report is generated in `build/artifacts/gcov/`.

---

## Essential Commands Reference

| Command | Description |
|---|---|
| `ceedling new <name>` | Scaffold a new project |
| `ceedling test:all` | Build and run all tests |
| `ceedling test:path[test_foo]` | Run one specific test file |
| `ceedling gcov:all` | Run tests with coverage reporting |
| `ceedling module:create[name]` | Auto-generate src + test stubs for a module |
| `ceedling clean` | Remove all build artifacts |
| `ceedling version` | Show installed Ceedling version |

---

## Build Output Structure

After running tests, the `build/` folder contains:

```
build/
├── test/
│   ├── out/         ← compiled object files
│   └── results/     ← .pass / .fail result files
├── artifacts/
│   └── gcov/        ← HTML coverage report (if gcov plugin enabled)
└── logs/            ← build logs
```

---

**Next Step:** Proceed to `02-demo-sessions/` for guided walkthroughs of real testing scenarios.
