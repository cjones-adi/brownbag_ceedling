# ❓ What Is Unit Testing?

## Definition

Unit testing is a method of testing software where **individual software components are isolated and tested for correctness**.

- Ensures that a **section of a program** (known as the *unit*) meets its design requirements and behaves as intended
- Involves the **smallest components** that can be isolated and individually tested — in C, that means **functions and modules**
- The most extensive type of testing the **original developer** performs as they work on their own code
- At its core: *"testing the correctness of your code at the smallest possible unit: the function"*
  — Dave Steffen, *Back to Basics: Unit Testing in C++*, CppCon 2024
- **Not** integration testing (multiple modules working together) and **not** system testing (full firmware on target hardware) — unit tests are intentionally narrow and isolated

> **Untested code is incomplete code.**

---

## Testing on Host vs. Embedded Target


Running tests in isolation (without relying on the full system or hardware) is a core principle of unit testing. Three principles explain why this matters:

> *A complex system that works is invariably found to have evolved from a simple system that worked.*

> *A complex system designed from scratch never works and cannot be made to work — you have to start over, beginning with a working simple system.*

> *Nothing works until everything works.*

**Implication:** Build and verify small, isolated units on the host first. Integration on the target is then validation, not debugging.


| | Isolated Unit Testing | On-Target Testing |
|---|---|---|
| **Speed** | ⚡ Milliseconds per run | 🐢 Flash + boot cycle each run |
| **Convenience** | ✅ No hardware required | ❌ Board, cables, debug probe needed |
| **Fault injection** | ✅ Simulate any failure on demand | ❌ Hard to reproduce hardware faults |
| **Automation friendly** | ✅ Runs in any pipeline | ❌ Requires physical lab hardware |

---

## Components of a Unit Test

Every unit test is made up of the same building blocks:

| Component | Purpose |
|---|---|
| **`setUp()`** | Runs before each test — initialise state, reset mocks |
| **`tearDown()`** | Runs after each test — clean up resources |
| **Checks / Assertions** | Compare actual output to expected: integers, strings, pointers, booleans |

Each individual test function follows the **Arrange → Act → Assert (AAA)** pattern:

| Step | What Happens | Example |
|---|---|---|
| **Arrange** | Set up inputs, mocks, and initial state | `int a = 2, b = 3;` |
| **Act** | Call the function under test | `int result = add(a, b);` |
| **Assert** | Verify the output is what you expected | `TEST_ASSERT_EQUAL_INT(5, result);` |

```c

// Example in C:
void setUp(void)    { /* reset module state before each test */ }
void tearDown(void) { /* release resources after each test  */ }

void test_add_two_positive_numbers(void)
{
  // Arrange
  int a = 2, b = 3;

  // Act
  int result = add(a, b);

  // Assert
  // Use your test framework's assertion here
  // e.g., assert(result == 5);
}
```

> One test function should test **one behaviour**. If a test fails, the name alone should tell you exactly what broke.

---

## What a Good Test Suite Covers

Ideally, unit tests cover most or all of:

| Coverage Target | Example |
|---|---|
| **Code paths** | Every `if`/`else` branch, every `case` in a `switch` |
| **Argument bounds** | Minimum value, maximum value, zero, negative, overflow |
| **Failure cases** | Invalid input, hardware errors, timeout conditions |

Achieving this coverage leads to:
- ✅ **Faster firmware stabilisation** — bugs found in seconds on the PC, not hours on the bench
- ✅ **Increased productivity** for individuals and teams
- ✅ Fewer **functional bugs**, fewer **control-flow bugs**, and far fewer **fatal issues** in the field

---

## Where Unit Testing Fits


```
Unit Tests  →  Integration Tests  →  System Tests  →  Hardware Validation
  70–80%           15–25%              5–10%
```

Unit tests form the **wide base** of the testing pyramid — they are fast, cheap, and catch the majority of logic errors before any system or hardware integration is involved.

---

## ✅ What Makes a Good Unit Test

### The F.I.R.S.T. Principles

| Letter | Property | What it means |
|--------|----------|---------------|
| **F** | **Fast** | Runs in milliseconds. If it is slow, it won't be run often — and tests that aren't run don't help anyone. |
| **I** | **Independent** | No test depends on another test's state. Any test must be able to run alone or in any order and still produce the same result. |
| **R** | **Repeatable** | Same result every single run — no flakiness, no sensitivity to timing, environment, or run order. |
| **S** | **Self-validating** | Pass or fail is determined by assertions, not by a human reading printed output and deciding if it looks right. |
| **T** | **Thorough / Timely** | Covers the happy path, boundary values, and failure cases. Written alongside or before the code it tests. |

---

### Naming — The Test Name Is the Requirement

A good test name answers three questions: *what function*, *under what condition*, *what is the expected result*.

```
test_<function>_<condition>_<expected_result>

✅  test_temp_sensor_read_returns_error_when_i2c_times_out
✅  test_safety_logic_does_not_trigger_when_only_temperature_exceeds_threshold
❌  test_thing2
❌  test_sensor
```

If the test fails, the name alone should tell you exactly what broke and why — without reading the test body.

---

### Scope — One Behaviour Per Test

A test that asserts ten things at once tells you *something* is wrong but not *what*.
One logical concept per test gives a precise, actionable failure report.

```
❌  void test_sensor_driver(void)          // tests init + read + error + timeout
✅  void test_sensor_read_parses_25C(void) // tests exactly one thing
✅  void test_sensor_read_returns_error_on_timeout(void)
```

---

### What a Good Test Should NOT Do

| Anti-pattern | Why it is a problem |
|---|---|
| Call `sleep()` or wait on real time | Makes the suite slow and timing-sensitive |
| Touch real hardware, files, or network | Introduces external dependencies that can fail for unrelated reasons |
| Depend on state left by a previous test | Run order changes → test results change → suite is unreliable |
| Test two unrelated behaviours in one function | One failure hides the other; failure message is ambiguous |
| Use magic numbers without explanation | `TEST_ASSERT_EQUAL_INT32(25000, temp)` needs a comment: *25 °C in millideg* |

---

**Next:** See `why-unit-testing.md` for real-world cases where this prevents disasters, and `ceedling-basics.md` to start writing tests.
