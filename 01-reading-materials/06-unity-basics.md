# 🧪 Unity – Unit Testing Framework Basics

## What Is It?
Unity is a **lightweight C unit testing framework** developed by Throw The Switch. It is the test runner used under the hood by Ceedling.

## Key Facts
- Written in **pure C** — runs on any platform, including bare-metal embedded targets
- Single header + source file (`unity.h` / `unity.c`) — minimal footprint
- Each test is a **plain C function** prefixed with `test_`
- Results are printed as simple pass/fail output to stdout (or a serial port on-target)

## Core Macros

| Macro | Purpose |
|---|---|
| `TEST_ASSERT_TRUE(cond)` | Passes if condition is true |
| `TEST_ASSERT_EQUAL_INT(exp, act)` | Compares two integers |
| `TEST_ASSERT_EQUAL_FLOAT(exp, act)` | Compares two floats |
| `TEST_ASSERT_NULL(ptr)` | Passes if pointer is NULL |
| `TEST_ASSERT_EQUAL_STRING(exp, act)` | Compares C strings |
| `TEST_FAIL_MESSAGE("msg")` | Forces an immediate failure |

## Anatomy of a Test File
```c
#include "unity.h"

void setUp(void)    { /* runs before each test */ }
void tearDown(void) { /* runs after each test  */ }

void test_addition_returns_correct_result(void) {
    TEST_ASSERT_EQUAL_INT(4, add(2, 2));
}
```

## Key Takeaways
- ✅ Zero dependencies — great for embedded targets
- ✅ `setUp()` and `tearDown()` provide per-test fixture control
- ✅ Any `TEST_ASSERT_*` / `TEST_FAIL*` failure immediately aborts that test function via `longjmp` — remaining assertions in that function are skipped, but all other test functions still run
- ✅ Ceedling auto-generates the `main()` runner — no boilerplate needed
