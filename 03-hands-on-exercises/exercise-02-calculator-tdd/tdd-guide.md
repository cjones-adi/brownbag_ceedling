# 🔴🟢🔧 TDD Step-by-Step Guide

**Your Complete Guide to Test-Driven Calculator Development**

This guide walks you through each TDD cycle in detail. Follow it religiously to master the discipline of Test-Driven Development.

## 🎯 Phase 1: Calculator Initialization (20 minutes)

### Cycle 1.1: Basic Calculator Creation

#### 🔴 RED (Write Failing Test)
**Time: 2 minutes**

Create `test/test_calculator.c`:

```c
#include "unity.h"
#include "calculator.h"  // This doesn't exist yet - that's good!

calculator_t calc;

void setUp(void) {
    // We'll initialize the calculator in each test for now
}

void tearDown(void) {
    // Nothing to clean up yet
}

void test_calculator_should_initialize_to_zero(void) {
    // Arrange
    calculator_t calc;

    // Act
    calculator_init(&calc);

    // Assert
    double result = calculator_get_result(&calc);
    TEST_ASSERT_EQUAL_DOUBLE(0.0, result);
}
```

**Run the test:**
```bash
ceedling test:test_calculator
```

**Expected:** Compilation failure - header doesn't exist. ✅ This is RED!

#### 🟢 GREEN (Make It Pass)
**Time: 3 minutes**

Create `src/calculator.h`:
```c
#ifndef CALCULATOR_H
#define CALCULATOR_H

typedef struct {
    double current_value;
} calculator_t;

void calculator_init(calculator_t* calc);
double calculator_get_result(calculator_t* calc);

#endif
```

Create `src/calculator.c`:
```c
#include "calculator.h"

void calculator_init(calculator_t* calc) {
    calc->current_value = 0.0;  // Simplest implementation
}

double calculator_get_result(calculator_t* calc) {
    return calc->current_value;  // Just return what we stored
}
```

**Run the test:**
```bash
ceedling test:test_calculator
```

**Expected:** Test passes! ✅ This is GREEN!

#### 🔧 REFACTOR (Clean Up)
**Time: 1 minute**

Code is already clean - nothing to refactor yet. Let's move to the next cycle.

---

### Cycle 1.2: Calculator Clear Function

#### 🔴 RED (Write Failing Test)
**Time: 1 minute**

Add to `test/test_calculator.c`:

```c
void test_calculator_should_clear_to_zero(void) {
    // Arrange
    calculator_t calc;
    calc.current_value = 42.0;  // Set to non-zero value

    // Act
    calculator_clear(&calc);

    // Assert
    TEST_ASSERT_EQUAL_DOUBLE(0.0, calculator_get_result(&calc));
}
```

**Run the test:**
```bash
ceedling test:test_calculator
```

**Expected:** Compilation failure - `calculator_clear` doesn't exist. ✅ This is RED!

#### 🟢 GREEN (Make It Pass)
**Time: 2 minutes**

Add to `src/calculator.h`:
```c
void calculator_clear(calculator_t* calc);
```

Add to `src/calculator.c`:
```c
void calculator_clear(calculator_t* calc) {
    calc->current_value = 0.0;  // Same as init for now
}
```

**Run the test:**
```bash
ceedling test:test_calculator
```

**Expected:** All tests pass! ✅ This is GREEN!

#### 🔧 REFACTOR (Clean Up)
**Time: 2 minutes**

Notice duplication between `calculator_init` and `calculator_clear`:

```c
void calculator_init(calculator_t* calc) {
    calculator_clear(calc);  // Reuse clear logic
}

void calculator_clear(calculator_t* calc) {
    calc->current_value = 0.0;
}
```

**Run tests to ensure refactoring didn't break anything:**
```bash
ceedling test:test_calculator
```

**Expected:** All tests still pass! ✅ Refactoring successful!

---

## 🎯 Phase 2: Number Input (30 minutes)

### Cycle 2.1: Single Number Input

#### 🔴 RED (Write Failing Test)
**Time: 2 minutes**

```c
void test_calculator_should_accept_single_number_input(void) {
    // Arrange
    calculator_t calc;
    calculator_init(&calc);

    // Act
    calculator_input_number(&calc, 42.0);

    // Assert
    TEST_ASSERT_EQUAL_DOUBLE(42.0, calculator_get_result(&calc));
}
```

**Run the test:**
```bash
ceedling test:test_calculator
```

**Expected:** Compilation failure - `calculator_input_number` doesn't exist. ✅ This is RED!

#### 🟢 GREEN (Make It Pass)
**Time: 2 minutes**

Add to `src/calculator.h`:
```c
void calculator_input_number(calculator_t* calc, double number);
```

Add to `src/calculator.c`:
```c
void calculator_input_number(calculator_t* calc, double number) {
    calc->current_value = number;  // Just store the number
}
```

**Run the test:**
```bash
ceedling test:test_calculator
```

**Expected:** All tests pass! ✅ This is GREEN!

#### 🔧 REFACTOR (Clean Up)
**Time: 1 minute**

Code looks clean. No refactoring needed yet.

---

### Cycle 2.2: Decimal Number Input

#### 🔴 RED (Write Failing Test)
**Time: 1 minute**

```c
void test_calculator_should_accept_decimal_numbers(void) {
    // Arrange
    calculator_t calc;
    calculator_init(&calc);

    // Act
    calculator_input_number(&calc, 3.14);

    // Assert
    TEST_ASSERT_EQUAL_DOUBLE(3.14, calculator_get_result(&calc));
}
```

**Run the test:**
```bash
ceedling test:test_calculator
```

**Expected:** Test passes immediately! This means our implementation already handles decimals. That's fine - we learned something about our design.

---

### Cycle 2.3: Negative Number Input

#### 🔴 RED (Write Failing Test)
**Time: 1 minute**

```c
void test_calculator_should_accept_negative_numbers(void) {
    // Arrange
    calculator_t calc;
    calculator_init(&calc);

    // Act
    calculator_input_number(&calc, -25.0);

    // Assert
    TEST_ASSERT_EQUAL_DOUBLE(-25.0, calculator_get_result(&calc));
}
```

**Run the test:**
```bash
ceedling test:test_calculator
```

**Expected:** Test passes immediately! Again, our implementation already handles this.

---

## 🎯 Phase 3: Simple Addition (45 minutes)

### Cycle 3.1: Two Number Addition

#### 🔴 RED (Write Failing Test)
**Time: 3 minutes**

Now we need to think about operations. Let's start with the simplest case:

```c
void test_calculator_should_add_two_positive_numbers(void) {
    // Arrange
    calculator_t calc;
    calculator_init(&calc);

    // Act
    calculator_input_number(&calc, 5.0);
    calculator_input_operation(&calc, '+');
    calculator_input_number(&calc, 3.0);
    double result = calculator_calculate(&calc);

    // Assert
    TEST_ASSERT_EQUAL_DOUBLE(8.0, result);
}
```

**Run the test:**
```bash
ceedling test:test_calculator
```

**Expected:** Compilation failures - neither `calculator_input_operation` nor `calculator_calculate` exist. ✅ This is RED!

#### 🟢 GREEN (Make It Pass)
**Time: 5 minutes**

This is where it gets interesting. Let's think about what we need:

Add to `src/calculator.h`:
```c
void calculator_input_operation(calculator_t* calc, char operation);
double calculator_calculate(calculator_t* calc);
```

Update the struct in `src/calculator.h`:
```c
typedef struct {
    double current_value;
    double stored_value;      // To hold first operand
    char pending_operation;   // To remember the operation
    bool has_pending_operation; // To track if we have an operation waiting
} calculator_t;
```

Add to top of `calculator.h`:
```c
#include <stdbool.h>
```

Update `src/calculator.c`:
```c
#include "calculator.h"

void calculator_init(calculator_t* calc) {
    calculator_clear(calc);
}

void calculator_clear(calculator_t* calc) {
    calc->current_value = 0.0;
    calc->stored_value = 0.0;
    calc->pending_operation = '\0';
    calc->has_pending_operation = false;
}

void calculator_input_number(calculator_t* calc, double number) {
    calc->current_value = number;
}

void calculator_input_operation(calculator_t* calc, char operation) {
    calc->stored_value = calc->current_value;  // Store first operand
    calc->pending_operation = operation;       // Remember operation
    calc->has_pending_operation = true;
}

double calculator_calculate(calculator_t* calc) {
    if (!calc->has_pending_operation) {
        return calc->current_value;
    }

    if (calc->pending_operation == '+') {
        return calc->stored_value + calc->current_value;
    }

    return calc->current_value;  // Fallback (shouldn't happen yet)
}

double calculator_get_result(calculator_t* calc) {
    return calc->current_value;
}
```

**Run the test:**
```bash
ceedling test:test_calculator
```

**Expected:** All tests pass! ✅ This is GREEN!

#### 🔧 REFACTOR (Clean Up)
**Time: 3 minutes**

The code is getting more complex. Let's see if we can clean it up:

```c
// The implementation looks reasonable for now, but let's check our tests
// We should update setUp to use calculator_init for consistency

void setUp(void) {
    calculator_init(&calc);  // Initialize calculator in setUp
}

// Now we can simplify our tests:
void test_calculator_should_add_two_positive_numbers(void) {
    // Act
    calculator_input_number(&calc, 5.0);
    calculator_input_operation(&calc, '+');
    calculator_input_number(&calc, 3.0);
    double result = calculator_calculate(&calc);

    // Assert
    TEST_ASSERT_EQUAL_DOUBLE(8.0, result);
}
```

Update all existing tests to use the global `calc` variable.

**Run tests:**
```bash
ceedling test:test_calculator
```

**Expected:** All tests still pass! ✅ Refactoring successful!

---

### Cycle 3.2: Addition with Negative Numbers

#### 🔴 RED (Write Failing Test)
**Time: 1 minute**

```c
void test_calculator_should_add_negative_numbers(void) {
    // Act
    calculator_input_number(&calc, -5.0);
    calculator_input_operation(&calc, '+');
    calculator_input_number(&calc, 3.0);
    double result = calculator_calculate(&calc);

    // Assert
    TEST_ASSERT_EQUAL_DOUBLE(-2.0, result);
}
```

**Run the test:**
```bash
ceedling test:test_calculator
```

**Expected:** Test passes! Our implementation already handles this.

---

### Cycle 3.3: Chain Multiple Additions

#### 🔴 RED (Write Failing Test)
**Time: 2 minutes**

Let's test chaining operations:

```c
void test_calculator_should_chain_multiple_additions(void) {
    // Act - Calculate 5 + 3 + 2 = 10
    calculator_input_number(&calc, 5.0);
    calculator_input_operation(&calc, '+');
    calculator_input_number(&calc, 3.0);
    calculator_input_operation(&calc, '+');  // This should complete 5+3
    calculator_input_number(&calc, 2.0);
    double result = calculator_calculate(&calc);

    // Assert
    TEST_ASSERT_EQUAL_DOUBLE(10.0, result);
}
```

**Run the test:**
```bash
ceedling test:test_calculator
```

**Expected:** Test likely fails because our `calculator_input_operation` doesn't handle chaining. ✅ This is RED!

#### 🟢 GREEN (Make It Pass)
**Time: 5 minutes**

We need to modify `calculator_input_operation` to complete pending operations:

```c
void calculator_input_operation(calculator_t* calc, char operation) {
    if (calc->has_pending_operation) {
        // Complete the pending operation
        calc->current_value = calculator_calculate(calc);
        calc->stored_value = calc->current_value;
    } else {
        calc->stored_value = calc->current_value;
    }

    calc->pending_operation = operation;
    calc->has_pending_operation = true;
}
```

**Run the test:**
```bash
ceedling test:test_calculator
```

**Expected:** All tests pass! ✅ This is GREEN!

---

## 🎯 Phase 4: All Basic Operations (60 minutes)

### Cycle 4.1: Subtraction

#### 🔴 RED (Write Failing Test)
**Time: 1 minute**

```c
void test_calculator_should_subtract_two_numbers(void) {
    // Act
    calculator_input_number(&calc, 10.0);
    calculator_input_operation(&calc, '-');
    calculator_input_number(&calc, 4.0);
    double result = calculator_calculate(&calc);

    // Assert
    TEST_ASSERT_EQUAL_DOUBLE(6.0, result);
}
```

**Run the test:**
```bash
ceedling test:test_calculator
```

**Expected:** Test fails because we only handle '+' in `calculator_calculate`. ✅ This is RED!

#### 🟢 GREEN (Make It Pass)
**Time: 2 minutes**

```c
double calculator_calculate(calculator_t* calc) {
    if (!calc->has_pending_operation) {
        return calc->current_value;
    }

    if (calc->pending_operation == '+') {
        return calc->stored_value + calc->current_value;
    } else if (calc->pending_operation == '-') {
        return calc->stored_value - calc->current_value;
    }

    return calc->current_value;
}
```

**Run the test:**
```bash
ceedling test:test_calculator
```

**Expected:** All tests pass! ✅ This is GREEN!

---

### Cycle 4.2: Multiplication

#### 🔴 RED (Write Failing Test)
**Time: 1 minute**

```c
void test_calculator_should_multiply_two_numbers(void) {
    // Act
    calculator_input_number(&calc, 6.0);
    calculator_input_operation(&calc, '*');
    calculator_input_number(&calc, 7.0);
    double result = calculator_calculate(&calc);

    // Assert
    TEST_ASSERT_EQUAL_DOUBLE(42.0, result);
}
```

#### 🟢 GREEN (Make It Pass)
**Time: 2 minutes**

```c
double calculator_calculate(calculator_t* calc) {
    if (!calc->has_pending_operation) {
        return calc->current_value;
    }

    switch (calc->pending_operation) {
        case '+': return calc->stored_value + calc->current_value;
        case '-': return calc->stored_value - calc->current_value;
        case '*': return calc->stored_value * calc->current_value;
        default:  return calc->current_value;
    }
}
```

#### 🔧 REFACTOR (Clean Up)
**Time: 2 minutes**

Switched to switch statement for cleaner code. Tests still pass!

---

### Cycle 4.3: Division

#### 🔴 RED (Write Failing Test)
**Time: 2 minutes**

```c
void test_calculator_should_divide_two_numbers(void) {
    // Act
    calculator_input_number(&calc, 15.0);
    calculator_input_operation(&calc, '/');
    calculator_input_number(&calc, 3.0);
    double result = calculator_calculate(&calc);

    // Assert
    TEST_ASSERT_EQUAL_DOUBLE(5.0, result);
}
```

#### 🟢 GREEN (Make It Pass)
**Time: 1 minute**

```c
case '/': return calc->stored_value / calc->current_value;
```

---

### Cycle 4.4: Division by Zero

#### 🔴 RED (Write Failing Test)
**Time: 2 minutes**

```c
void test_calculator_should_handle_division_by_zero(void) {
    // Act
    calculator_input_number(&calc, 10.0);
    calculator_input_operation(&calc, '/');
    calculator_input_number(&calc, 0.0);
    double result = calculator_calculate(&calc);

    // Assert - Let's say we want NaN for division by zero
    TEST_ASSERT_TRUE(isnan(result));
}
```

Add to top of test file:
```c
#include <math.h>
```

#### 🟢 GREEN (Make It Pass)
**Time: 2 minutes**

```c
case '/':
    if (calc->current_value == 0.0) {
        return NAN;  // Return NaN for division by zero
    }
    return calc->stored_value / calc->current_value;
```

Add to top of calculator.c:
```c
#include <math.h>
```

---

## 🎯 Phase 5: Memory Functions (45 minutes)

Continue with this pattern for memory functions...

### Cycle 5.1: Memory Store

#### 🔴 RED (Write Failing Test)

```c
void test_calculator_should_store_value_in_memory(void) {
    // Arrange
    calculator_input_number(&calc, 42.0);

    // Act
    calculator_memory_store(&calc);
    calculator_input_number(&calc, 99.0);  // Change current value
    calculator_memory_recall(&calc);

    // Assert
    TEST_ASSERT_EQUAL_DOUBLE(42.0, calculator_get_result(&calc));
}
```

#### 🟢 GREEN (Make It Pass)

Add to struct:
```c
double memory_value;
```

Implement functions:
```c
void calculator_memory_store(calculator_t* calc) {
    calc->memory_value = calc->current_value;
}

void calculator_memory_recall(calculator_t* calc) {
    calc->current_value = calc->memory_value;
}
```

---

## 🎯 Continue the Pattern...

**Keep following this exact RED-GREEN-REFACTOR cycle for:**

- Memory clear
- Complex operation chains
- Error state handling
- Edge cases you discover

## 🏆 TDD Mastery Indicators

You know you're mastering TDD when:

- [ ] You feel uncomfortable writing code without a test
- [ ] You take tiny steps (1-2 lines at a time)
- [ ] You refactor fearlessly because tests protect you
- [ ] Your design emerges naturally from tests
- [ ] You catch bugs immediately when tests fail
- [ ] You have complete confidence in your code

## 💡 TDD Insights You'll Gain

### Design Insights
- Tests force you to think about interfaces first
- Simple implementations often emerge naturally
- Refactoring reveals better abstractions
- Edge cases become obvious through testing

### Development Insights
- Bugs are caught within minutes of writing code
- Debugging time drops dramatically
- Code confidence skyrockets
- Development rhythm becomes predictable

### Quality Insights
- Test coverage is comprehensive automatically
- Code is naturally more modular
- Documentation is built-in through test names
- Regression protection is complete

---

**Remember: The goal isn't speed, it's learning the TDD discipline. Take your time and follow the process religiously!** 🎯