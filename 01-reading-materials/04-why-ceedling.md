# 🤔 Why Ceedling for Embedded Unit Testing?

Once you are convinced that unit testing is worth doing, the next question is: **which tool?**
Embedded C development has unique constraints — no OS, no standard library on bare-metal,
cross-compilation, hardware dependencies — that rule out many general-purpose testing tools.
Ceedling was built specifically for this environment.

---

## 🧰 The Embedded Testing Problem

General-purpose testing frameworks (JUnit, pytest, etc.) assume a rich runtime environment.
Embedded firmware often has none of that:

| Challenge | Why it matters |
|-----------|---------------|
| Cross-compiled for a target CPU | You can't run the binary on your development machine without extra work |
| Hardware dependencies in source | Calling `MXC_I2C_MasterTransaction()` directly makes code untestable off-target |
| No dynamic memory / OS | Many C++ test frameworks require heap allocation or threading |
| Float-free or float-limited code | Embedded code often avoids `float`; test tools must handle `int32_t` millideg values cleanly |
| Build system complexity | Embedded projects have complex Makefiles, linker scripts, and SDK include paths |

---

## ✅ Why Ceedling Fits Embedded C

### 1. Automation
Ceedling automates the entire test workflow — compiling, linking, running, and reporting —
with a single command:

```bash
ceedling test:all
```

No manual compilation steps, no separate test runner invocation, no output parsing.
This low friction means tests actually get run on every change.

### 2. Automatic Mock Generation (CMock)
CMock reads a header file and generates a complete mock implementation — no manual stub writing.

```
src/i2c_hal.h  →  (CMock generates)  →  mock_i2c_hal.h + mock_i2c_hal.c
```

Generated mocks provide:
- `_ExpectAndReturn()` — strict call verification with queued return values
- `_StubWithCallback()` — inject any byte payload or custom behaviour
- `_Ignore()` / `_IgnoreAndReturn()` — allow calls without strict counting
- Automatic failure on unexpected calls

For embedded work this is critical: hardware interfaces change frequently.
When `i2c_hal.h` changes, re-running Ceedling regenerates the mock automatically —
no mock code to manually maintain.

### 3. Automatic Test Discovery
Ceedling scans the `test/` directory and automatically finds every function prefixed
with `test_`. No test registry, no manual test list to maintain. Add a new test function
and it runs next time without any configuration change.

### 4. Host-Native Test Execution
Tests compile and run on the **host PC** (Linux, macOS, Windows/WSL), not on the target.

```
Flash + boot cycle on target:  30–60 seconds per change
Ceedling test run on host:     < 1 second for the full suite
```

This speed difference is what makes TDD (write test → run → fix → repeat) practical
in an embedded workflow. Running on the host also enables CI/CD — tests run automatically
on every commit in a pipeline with no hardware attached.

### 5. Designed for Bare-Metal C
- Written in Ruby, but the tests and code under test are **pure C**
- No C++ runtime required — works on codebases targeting Cortex-M0 with no stdlib
- Unity (the assertion library inside Ceedling) has a configurable footprint and can
  even run on-target when needed
- Supports `int32_t`, `uint8_t`, bitmask assertions (`TEST_ASSERT_BITS`),
  and other embedded-friendly types natively

### 6. Integration with Existing Embedded Build Systems
Ceedling co-exists with your firmware build system. The same source files that
compile for the target also compile for the test build — no duplication.
The `#ifndef TEST` guard pattern keeps hardware-specific code out of the test build:

```c
#ifndef TEST
#include "mxc_errors.h"   // MaximSDK – only in firmware build
#include "i2c.h"
#endif
```

### 7. Built-In Code Coverage (gcov)
Enable the `gcov` plugin in `project.yml` and get a full HTML coverage report:

```bash
ceedling gcov:all
# → build/artifacts/gcov/index.html
```

Line coverage, branch coverage, and function coverage — no separate tooling needed.
This matters for safety-critical development where coverage targets are required
(ISO 26262, IEC 61508, DO-178C).

---

## 🔍 Ceedling vs. Alternatives

| Tool | Language | Mock generation | Bare-metal C | Host-native | Coverage |
|------|----------|-----------------|--------------|-------------|----------|
| **Ceedling + Unity + CMock** | C | ✅ Auto (CMock) | ✅ Yes | ✅ Yes | ✅ gcov |
| CppUTest | C/C++ | ⚠️ Manual | ⚠️ Limited | ✅ Yes | ⚠️ External |
| Google Test | C++ | ⚠️ Manual/GMock | ❌ Requires C++ runtime | ✅ Yes | ⚠️ External |
| Parasoft C/C++test | C/C++ | ✅ Auto | ✅ Yes | ✅ Yes | ✅ Yes |
| pytest + cffi | Python wrapper | ❌ No | ❌ No | ✅ Yes | ⚠️ Limited |

Ceedling is the only **free, open-source** option that combines automatic mock generation,
zero C++ runtime dependency, and built-in coverage in a single integrated tool.

---

## 📦 The Ceedling Stack at a Glance

```
┌─────────────────────────────────────────┐
│              Ceedling                   │  ← orchestrates everything
│  (build system, test runner, reporter)  │
├──────────────┬──────────────────────────┤
│    Unity     │         CMock            │
│ (assertions) │  (auto-generated mocks)  │
├──────────────┴──────────────────────────┤
│           GCC / host toolchain          │  ← compiles test binaries
└─────────────────────────────────────────┘
```

All three components are maintained by the same team (Throw The Switch) and are
designed to work together out of the box.

---

## 🎯 Key Takeaways

- ✅ **Built for C** — not a C++ framework retrofitted to C
- ✅ **Automatic mocks** — hardware interfaces become testable instantly when you write a HAL header
- ✅ **Fast feedback** — full suite in under a second, enabling TDD in embedded workflows
- ✅ **CI/CD ready** — runs on any Linux/macOS/Windows build server without hardware
- ✅ **Coverage included** — no separate tooling for gcov reporting
- ✅ **Free and open source** — actively maintained, used in production embedded projects worldwide

---

**Next:** See `ceedling-basics.md` for project structure, configuration, and essential commands.
