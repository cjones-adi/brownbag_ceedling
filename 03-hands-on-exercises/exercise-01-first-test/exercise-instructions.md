# 🎯 Exercise 1: Your First Test - Step by Step Instructions

## 🚀 Getting Started

Welcome to your first hands-on unit testing experience! This exercise will guide you through creating your first Ceedling project and writing meaningful unit tests.

**⏰ Estimated Time:** 1-3 hours depending on experience level

## 📋 Prerequisites Checklist

Before starting, ensure you have:
- [ ] Ceedling installed and working (`ceedling version` should work)
- [ ] C compiler available (GCC recommended)
- [ ] Text editor or IDE ready
- [ ] Read the basic unit testing concepts from reading materials
- [ ] Completed or watched Demo 1: Simple Start (recommended)

## 📁 Step 1: Project Setup (15 minutes)

### 1.1 Create Project Directory

```bash
# Create a new directory for your exercise
mkdir my-first-test-exercise
cd my-first-test-exercise

# Initialize a new Ceedling project
ceedling new math_utils_project
cd math_utils_project

# Verify the project structure was created
ls -la
```

**Expected Output:**
```
project.yml    # Ceedling configuration
src/          # Your source code goes here
test/         # Your test files go here
```

### 1.2 Test Your Setup

```bash
# Run the test command to ensure everything works
ceedling test:all
```

**Expected Output:**
```
No tests to run.
```

**✅ Success Indicator:** No errors, just a message about no tests to run.

**❌ If You See Errors:**
- Verify Ceedling is in your PATH
- Check that you're in the project directory
- Ensure you have write permissions

### 1.3 Examine Project Configuration

```bash
# Look at the project configuration
cat project.yml
```

**Key Settings to Notice:**
- `:use_mocks: TRUE` - We can create mock objects
- `:test_file_prefix: test_` - Test files must start with "test_"
- `:default_tasks: - test:all` - Default command runs all tests

## 📝 Step 2: Create the Header File (10 minutes)

### 2.1 Create the Math Utils Header

```bash
# Create the header file in the src directory
touch src/math_utils.h
```

### 2.2 Define the Interface

Open `src/math_utils.h` in your editor and add:

```c
#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <stdbool.h>
#include <limits.h>

/**
 * Math Utilities Library
 *
 * A simple library demonstrating unit testing concepts
 * with basic mathematical operations.
 */

// Basic arithmetic operations
int math_add(int a, int b);
int math_subtract(int a, int b);
int math_multiply(int a, int b);
int math_divide(int a, int b);

// Utility functions
bool math_is_even(int number);
bool math_is_positive(int number);
int math_absolute(int number);
int math_power(int base, int exponent);

#endif // MATH_UTILS_H
```

**📝 Note:** We're defining the interface first - what our functions should do, not how they do it.

## 🧪 Step 3: Write Your First Test (20 minutes)

### 3.1 Create the Test File

```bash
# Create your first test file
touch test/test_math_utils.c
```

### 3.2 Set Up the Basic Test Structure

Open `test/test_math_utils.c` and add:

```c
#include "unity.h"
#include "math_utils.h"

// This function runs before each individual test
void setUp(void)
{
    // Initialize any test data or state here
    // For this exercise, we don't need any setup
}

// This function runs after each individual test
void tearDown(void)
{
    // Clean up any test data or state here
    // For this exercise, we don't need any cleanup
}

// Your first test function
void test_math_add_should_return_sum_of_two_positive_numbers(void)
{
    // This test will verify that math_add works correctly
    // We'll implement it step by step
}
```

### 3.3 Implement Your First Test (Arrange-Act-Assert Pattern)

Replace the empty test function with:

```c
void test_math_add_should_return_sum_of_two_positive_numbers(void)
{
    // ARRANGE: Set up the test data
    int first_number = 5;
    int second_number = 3;
    int expected_result = 8;

    // ACT: Call the function we're testing
    int actual_result = math_add(first_number, second_number);

    // ASSERT: Verify the result is what we expected
    TEST_ASSERT_EQUAL(expected_result, actual_result);
}
```

**📚 Understanding the Pattern:**
- **Arrange**: Set up test data and expected results
- **Act**: Call the function being tested
- **Assert**: Verify the actual result matches expectations

### 3.4 Experience the RED Phase

```bash
# Run the test - it should FAIL
ceedling test:test_math_utils
```

**Expected Output:**
```
Compilation failed: math_add function not found
```

**🔴 This is the RED phase!** The test fails because we haven't implemented the function yet. This proves our test is actually testing something.

## 💚 Step 4: Make Your Test Pass (GREEN Phase) (10 minutes)

### 4.1 Create the Implementation File

```bash
# Create the source file
touch src/math_utils.c
```

### 4.2 Implement the Minimal Function

Open `src/math_utils.c` and add:

```c
#include "math_utils.h"

int math_add(int a, int b)
{
    return a + b;  // Simplest implementation that makes the test pass
}

// We'll implement other functions as we write tests for them
// This is the TDD approach: implement only what's needed to pass tests
```

### 4.3 See Your Test Pass

```bash
# Run the test again
ceedling test:test_math_utils
```

**Expected Output:**
```
test_math_utils.c:XX:test_math_add_should_return_sum_of_two_positive_numbers:PASS

-----------------------
1 Tests 0 Failures 0 Ignored
```

**🟢 Congratulations! Your first test is GREEN!** 🎉

## 🔄 Step 5: Add More Tests (TDD Cycle) (45 minutes)

Now let's add more tests one at a time, following the Red-Green-Refactor cycle:

### 5.1 Test Addition with Negative Numbers

Add this test to your test file:

```c
void test_math_add_should_handle_negative_numbers_correctly(void)
{
    // Arrange
    int negative_number = -5;
    int positive_number = 3;
    int expected_result = -2;

    // Act
    int actual_result = math_add(negative_number, positive_number);

    // Assert
    TEST_ASSERT_EQUAL(expected_result, actual_result);
}
```

**Run the test:**
```bash
ceedling test:test_math_utils
```

**Expected:** This should PASS immediately because our implementation already handles this case!

### 5.2 Test Addition with Zero

Add this test:

```c
void test_math_add_should_handle_zero_correctly(void)
{
    // Arrange
    int number = 42;
    int zero = 0;
    int expected_result = 42;

    // Act
    int actual_result = math_add(number, zero);

    // Assert
    TEST_ASSERT_EQUAL(expected_result, actual_result);
}
```

### 5.3 Now Let's Test Subtraction (RED-GREEN cycle)

Add this test:

```c
void test_math_subtract_should_return_difference_of_two_numbers(void)
{
    // Arrange
    int minuend = 10;
    int subtrahend = 4;
    int expected_result = 6;

    // Act
    int actual_result = math_subtract(minuend, subtrahend);

    // Assert
    TEST_ASSERT_EQUAL(expected_result, actual_result);
}
```

**Run the test - it should FAIL (RED):**
```bash
ceedling test:test_math_utils
```

**Now implement the function (GREEN):**

Add to `src/math_utils.c`:
```c
int math_subtract(int a, int b)
{
    return a - b;
}
```

**Run the test again - it should PASS (GREEN):**
```bash
ceedling test:test_math_utils
```

### 5.4 Continue with More Functions

Following the same RED-GREEN pattern, add tests and implementations for:

#### Test Multiplication:
```c
void test_math_multiply_should_return_product_of_two_numbers(void)
{
    // Arrange
    int first_factor = 6;
    int second_factor = 7;
    int expected_result = 42;

    // Act
    int actual_result = math_multiply(first_factor, second_factor);

    // Assert
    TEST_ASSERT_EQUAL(expected_result, actual_result);
}
```

#### Test Multiplication by Zero:
```c
void test_math_multiply_should_return_zero_when_multiplied_by_zero(void)
{
    // Arrange
    int number = 100;
    int zero = 0;
    int expected_result = 0;

    // Act
    int actual_result = math_multiply(number, zero);

    // Assert
    TEST_ASSERT_EQUAL(expected_result, actual_result);
}
```

**Implement multiplication:**
```c
int math_multiply(int a, int b)
{
    return a * b;
}
```

### 5.5 Test Division (Including Edge Cases)

```c
void test_math_divide_should_return_quotient_when_divisor_is_not_zero(void)
{
    // Arrange
    int dividend = 15;
    int divisor = 3;
    int expected_result = 5;

    // Act
    int actual_result = math_divide(dividend, divisor);

    // Assert
    TEST_ASSERT_EQUAL(expected_result, actual_result);
}

void test_math_divide_should_return_zero_when_dividing_by_zero(void)
{
    // Arrange
    int dividend = 15;
    int divisor = 0;
    int expected_result = 0;  // Our specification says return 0 for division by zero

    // Act
    int actual_result = math_divide(dividend, divisor);

    // Assert
    TEST_ASSERT_EQUAL(expected_result, actual_result);
}
```

**Implement division with error handling:**
```c
int math_divide(int a, int b)
{
    if (b == 0) {
        return 0;  // Handle division by zero as specified
    }
    return a / b;
}
```

## 🧪 Step 6: Test Boolean Functions (20 minutes)

### 6.1 Test Even Number Detection

```c
void test_math_is_even_should_return_true_for_even_numbers(void)
{
    // Test multiple even numbers
    TEST_ASSERT_TRUE(math_is_even(2));
    TEST_ASSERT_TRUE(math_is_even(4));
    TEST_ASSERT_TRUE(math_is_even(0));    // Zero is even
    TEST_ASSERT_TRUE(math_is_even(-4));   // Negative even numbers
}

void test_math_is_even_should_return_false_for_odd_numbers(void)
{
    // Test multiple odd numbers
    TEST_ASSERT_FALSE(math_is_even(1));
    TEST_ASSERT_FALSE(math_is_even(3));
    TEST_ASSERT_FALSE(math_is_even(-3));  // Negative odd numbers
}
```

**Implement the function:**
```c
bool math_is_even(int number)
{
    return (number % 2) == 0;
}
```

### 6.2 Test Positive Number Detection

```c
void test_math_is_positive_should_return_true_for_positive_numbers(void)
{
    TEST_ASSERT_TRUE(math_is_positive(1));
    TEST_ASSERT_TRUE(math_is_positive(100));
    TEST_ASSERT_TRUE(math_is_positive(999999));
}

void test_math_is_positive_should_return_false_for_zero_and_negative_numbers(void)
{
    TEST_ASSERT_FALSE(math_is_positive(0));     // Zero is not positive
    TEST_ASSERT_FALSE(math_is_positive(-1));
    TEST_ASSERT_FALSE(math_is_positive(-100));
}
```

**Implement the function:**
```c
bool math_is_positive(int number)
{
    return number > 0;
}
```

## ✅ Step 7: Validation and Testing (15 minutes)

### 7.1 Run All Tests

```bash
# Run the complete test suite
ceedling test:all
```

**You should see output like:**
```
Running tests...

test_math_utils.c:XX:test_math_add_should_return_sum_of_two_positive_numbers:PASS
test_math_utils.c:XX:test_math_add_should_handle_negative_numbers_correctly:PASS
test_math_utils.c:XX:test_math_subtract_should_return_difference_of_two_numbers:PASS
... (more tests)

-----------------------
XX Tests 0 Failures 0 Ignored
```

### 7.2 Test Failure Scenarios

Let's intentionally break a test to see what failure looks like:

1. **Modify one of your functions** to return a wrong value
2. **Run the tests** and observe the failure message
3. **Fix the function** and verify tests pass again

Example: Change `math_add` to return `a - b` instead of `a + b`:

```c
int math_add(int a, int b)
{
    return a - b;  // Intentionally wrong!
}
```

**Run tests:**
```bash
ceedling test:test_math_utils
```

**You'll see a detailed failure message:**
```
test_math_utils.c:XX:test_math_add_should_return_sum_of_two_positive_numbers:FAIL:
Expected 8 Was -2
```

**Fix the function** and verify tests pass again.

### 7.3 Check Your Test Count

Count your test functions:
```bash
grep "void test_" test/test_math_utils.c
```

**Goal:** You should have at least 8-10 test functions.

## 🎯 Step 8: Add More Advanced Tests (Bonus)

If you have time and want to challenge yourself:

### 8.1 Absolute Value Function

```c
void test_math_absolute_should_return_positive_value_for_negative_input(void)
{
    TEST_ASSERT_EQUAL(5, math_absolute(-5));
    TEST_ASSERT_EQUAL(100, math_absolute(-100));
}

void test_math_absolute_should_return_same_value_for_positive_input(void)
{
    TEST_ASSERT_EQUAL(5, math_absolute(5));
    TEST_ASSERT_EQUAL(0, math_absolute(0));
}
```

**Implementation:**
```c
int math_absolute(int number)
{
    if (number < 0) {
        return -number;
    }
    return number;
}
```

### 8.2 Power Function

```c
void test_math_power_should_calculate_power_correctly(void)
{
    TEST_ASSERT_EQUAL(8, math_power(2, 3));    // 2^3 = 8
    TEST_ASSERT_EQUAL(1, math_power(5, 0));    // Any number^0 = 1
    TEST_ASSERT_EQUAL(0, math_power(0, 5));    // 0^positive = 0
}
```

## 🏆 Success Celebration!

**Congratulations!** 🎉 You have successfully:

- ✅ Created your first Ceedling project
- ✅ Written multiple unit tests using the Arrange-Act-Assert pattern
- ✅ Experienced the Red-Green-Refactor cycle
- ✅ Used Unity assertions effectively
- ✅ Tested both normal and edge cases
- ✅ Built confidence in your code through automated testing

## 🤔 Reflection Questions

Before moving to the next exercise, reflect on these questions:

1. **How did writing tests first change your approach to implementation?**
2. **What bugs did the tests help you catch or prevent?**
3. **Which tests were most valuable? Why?**
4. **How confident are you now that your math functions work correctly?**
5. **What would you do differently next time?**

## 🔄 Next Steps

Now that you've completed your first exercise:

1. **Review the reference solution** (if available) to compare approaches
2. **Experiment** - try adding new functions and tests
3. **Share your experience** with teammates
4. **Move to Exercise 2** - Calculator TDD for more advanced patterns

## 📞 Getting Help

If you encountered issues:

- **Check the troubleshooting guide** in this directory
- **Review the demo sessions** for similar examples
- **Ask on the team forum** or during office hours
- **Don't get stuck** - learning is the goal, not perfection!

---

**Great job on completing your first unit testing exercise! The foundation you've built here will serve you well in more advanced testing scenarios.** 🚀