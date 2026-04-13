# 📝 Exercise 1: Your First Test

Welcome to your first hands-on unit testing exercise! This exercise is designed to get you comfortable with the basic concepts and syntax of unit testing in Ceedling.

## 🎯 Learning Objectives

By the end of this exercise, you will:
- ✅ Write your very first unit test
- ✅ Understand the basic test structure (Setup, Act, Assert)
- ✅ Use fundamental Unity assertions
- ✅ See a test pass and fail
- ✅ Understand test output and results

## 📋 Prerequisites

- Ceedling installed and working
- Completed reading materials (01-reading-materials)
- Attended Demo 1: Simple Start (recommended)

## 🎮 Exercise Overview

You'll write tests for a simple math utility module that provides basic arithmetic operations. This is intentionally simple to focus on learning the testing mechanics rather than complex logic.

## 🏗️ Project Structure

```
exercise-01-first-test/
├── README.md                 # This file
├── REQUIREMENTS.md           # What you need to implement
├── project.yml              # Ceedling configuration
├── src/
│   ├── math_utils.h         # Header file (provided)
│   └── math_utils.c         # Implementation (you write this)
├── test/
│   └── test_math_utils.c    # Test file (you write this)
└── solution/                # Reference solution (don't peek!)
```

## 🚀 Step-by-Step Instructions

### Step 1: Set Up the Project (5 minutes)
1. Copy the project template from `tools-and-templates/project-template/`
2. Rename it to `exercise-01-first-test`
3. Verify Ceedling works: `ceedling test:all` (should show no tests)

### Step 2: Create the Header File (5 minutes)
Create `src/math_utils.h` with these function declarations:

```c
#ifndef MATH_UTILS_H
#define MATH_UTILS_H

int math_add(int a, int b);
int math_subtract(int a, int b);
int math_multiply(int a, int b);
int math_divide(int a, int b);
bool math_is_even(int number);

#endif
```

### Step 3: Write Your First Test (15 minutes)

Create `test/test_math_utils.c`:

```c
#include "unity.h"
#include "math_utils.h"

void setUp(void)
{
    // This runs before each test
}

void tearDown(void)
{
    // This runs after each test
}

void test_math_add_should_return_sum_of_two_positive_numbers(void)
{
    // Arrange
    int a = 5;
    int b = 3;
    int expected = 8;

    // Act
    int result = math_add(a, b);

    // Assert
    TEST_ASSERT_EQUAL(expected, result);
}

// TODO: Add more test functions here
```

### Step 4: See Your Test Fail (5 minutes)
1. Run: `ceedling test:test_math_utils`
2. Observe the compilation error (function doesn't exist yet)
3. This is the "Red" phase of Red-Green-Refactor!

### Step 5: Make the Test Pass (10 minutes)
Create `src/math_utils.c` with minimal implementation:

```c
#include "math_utils.h"

int math_add(int a, int b)
{
    return a + b;  // Simplest implementation that makes test pass
}

// TODO: Implement other functions
```

### Step 6: See Your Test Pass (5 minutes)
1. Run: `ceedling test:test_math_utils`
2. Celebrate! You've written your first passing test! 🎉
3. This is the "Green" phase!

### Step 7: Add More Tests (20 minutes)

Add these test cases one at a time:

```c
void test_math_add_should_handle_negative_numbers(void)
{
    // Test with negative numbers
    // a = -5, b = 3, expected = -2
}

void test_math_subtract_should_return_difference(void)
{
    // Test subtraction
    // a = 10, b = 4, expected = 6
}

void test_math_multiply_should_return_product(void)
{
    // Test multiplication
    // a = 6, b = 7, expected = 42
}

void test_math_divide_should_return_quotient_when_divisor_is_not_zero(void)
{
    // Test division
    // a = 15, b = 3, expected = 5
}

void test_math_is_even_should_return_true_for_even_numbers(void)
{
    // Test even number detection
    // number = 4, expected = true
}

void test_math_is_even_should_return_false_for_odd_numbers(void)
{
    // Test odd number detection
    // number = 7, expected = false
}
```

### Step 8: Implement the Functions (15 minutes)

Complete the implementation in `math_utils.c` to make all tests pass.

### Step 9: Add Edge Case Tests (15 minutes)

Add tests for edge cases:
- Division by zero
- Very large numbers
- Zero as input
- Negative numbers

## 🎯 Success Criteria

You've successfully completed this exercise when:
- ✅ All tests pass (`ceedling test:all` shows green)
- ✅ You understand the Arrange-Act-Assert pattern
- ✅ You've experienced the Red-Green cycle
- ✅ You can write basic Unity assertions
- ✅ You can read and understand test output

## 📊 Common Unity Assertions

Learn these basic assertions:

```c
TEST_ASSERT_EQUAL(expected, actual)          // Equality
TEST_ASSERT_TRUE(condition)                  // Boolean true
TEST_ASSERT_FALSE(condition)                 // Boolean false
TEST_ASSERT_NULL(pointer)                    // Null pointer
TEST_ASSERT_NOT_NULL(pointer)                // Non-null pointer
TEST_ASSERT_EQUAL_STRING(expected, actual)   // String comparison
```

## ❓ Troubleshooting

### "Function not found" error
- Check that you've implemented the function in `math_utils.c`
- Verify the function signature matches the header file

### "Test runner not found" error
- Make sure your test file starts with `test_`
- Check that `#include "unity.h"` is at the top

### Tests not running
- Verify Ceedling is properly installed
- Check that `project.yml` is configured correctly

## 🔄 Next Steps

After completing this exercise:
1. **Experiment** - Try breaking tests and fixing them
2. **Add more functions** - Practice writing more tests
3. **Move to Exercise 2** - Learn Test-Driven Development
4. **Review the simple LED example** - See similar patterns in action

## 💡 Key Takeaways

- **Tests guide implementation** - Write tests first, then code
- **Small steps work** - One test at a time
- **Failing tests are good** - They show your test actually tests something
- **Simple is better** - Start with basic cases, add complexity gradually

---

**Time Estimate:** 1-2 hours for beginners, 30-60 minutes for intermediate developers

**Next Exercise:** `exercise-02-calculator-tdd/` - Learn the full TDD cycle