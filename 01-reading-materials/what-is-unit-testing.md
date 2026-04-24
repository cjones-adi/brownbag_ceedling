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

Running tests on a host PC instead of flashing hardware every time is central to the Ceedling workflow. Three principles explain why this matters:

> *A complex system that works is invariably found to have evolved from a simple system that worked.*

> *A complex system designed from scratch never works and cannot be made to work — you have to start over, beginning with a working simple system.*

> *Nothing works until everything works.*

**Implication:** Build and verify small, isolated units on the host first. Integration on the target is then validation, not debugging.

| | Host Testing | On-Target Testing |
|---|---|---|
| **Speed** | ⚡ Milliseconds per run | 🐢 Flash + boot cycle each run |
| **Convenience** | ✅ No hardware required | ❌ Board, cables, debug probe needed |
| **Fault injection** | ✅ Mock any failure on demand | ❌ Hard to reproduce hardware faults |
| **CI/CD friendly** | ✅ Runs in any pipeline | ❌ Requires physical lab hardware |

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
void setUp(void)    { /* reset module state before each test */ }
void tearDown(void) { /* release resources after each test  */ }

void test_add_two_positive_numbers(void)
{
    // Arrange
    int a = 2, b = 3;

    // Act
    int result = add(a, b);

    // Assert
    TEST_ASSERT_EQUAL_INT(5, result);
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
(host PC)      (host or target)       (target)          (target)
   70–80%           15–25%              5–10%
```

Unit tests form the **wide base** of the testing pyramid — they are fast, cheap, and catch the majority of logic errors before any hardware is involved.

---

**Next:** See `why-unit-testing.md` for real-world cases where this prevents disasters, and `ceedling-basics.md` to start writing tests.
