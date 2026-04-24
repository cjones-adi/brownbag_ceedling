# 💥 CException – Exception Handling Basics

## What Is It?
CException is a **simple exception handling library** for C, also by Throw The Switch. It brings try/catch semantics to C using `setjmp`/`longjmp` — with a tiny footprint suited for embedded targets.

## Why It Matters for Testing
Embedded code often needs to handle error conditions (hardware faults, out-of-range values). CException lets you **throw and catch error codes** rather than relying solely on return values, and Unity can **assert that exceptions are thrown** as expected.

## Core API

| Construct | Usage |
|---|---|
| `Try { }` | Wrap code that might throw |
| `Catch(e) { }` | Handle the thrown exception code |
| `Throw(ERROR_CODE)` | Raise an exception (integer code) |

## Basic Example
```c
#include "CException.h"

void divide(int a, int b) {
    if (b == 0) Throw(DIVIDE_BY_ZERO);
    return a / b;
}

// In your test:
void test_divide_throws_on_zero(void) {
    CEXCEPTION_T e;
    Try {
        divide(10, 0);
        TEST_FAIL_MESSAGE("Should have thrown");
    }
    Catch(e) {
        TEST_ASSERT_EQUAL(DIVIDE_BY_ZERO, e);
    }
}
```

## Key Constraints
- Exception values are **integers only** (no objects or structs)
- **Do not** `Throw` across ISRs or OS task boundaries
- Nested `Try` blocks are supported
- Each `Try` must have a matching `Catch`

## Key Takeaways
- ✅ Adds structured error handling to plain C without dynamic memory
- ✅ Extremely small — a single `.c` / `.h` pair
- ✅ Unity provides `TEST_ASSERT_THROWS` style helpers when paired with Ceedling
- ✅ Great for validating that error paths actually fire during unit tests
