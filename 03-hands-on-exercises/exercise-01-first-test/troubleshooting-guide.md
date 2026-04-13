# 🛠️ Exercise 1: Troubleshooting Guide

Common issues and solutions when completing your first unit testing exercise.

## 🚨 Setup Issues

### "Command 'ceedling' not found"
**Problem:** Ceedling is not installed or not in your PATH

**Solutions:**
1. **Install Ceedling:**
   ```bash
   gem install ceedling
   ```

2. **Check if Ruby/Gems are in PATH:**
   ```bash
   ruby --version
   gem --version
   which ceedling
   ```

3. **Add gem bin directory to PATH (if needed):**
   ```bash
   # Add to your ~/.bashrc or ~/.zshrc
   export PATH="$PATH:$(gem environment gemdir)/bin"
   ```

### "ceedling new" Command Fails
**Problem:** Permission issues or missing dependencies

**Solutions:**
1. **Check current directory permissions:**
   ```bash
   ls -la
   # You should have write permission in current directory
   ```

2. **Try creating in your home directory:**
   ```bash
   cd ~
   mkdir my-tests
   cd my-tests
   ceedling new math_utils_project
   ```

3. **Install missing dependencies:**
   ```bash
   gem install rake
   gem install constructor
   ```

## 📝 Compilation Issues

### "'unity.h' file not found"
**Problem:** Test can't find Unity framework

**Solutions:**
1. **Make sure you're in the Ceedling project directory:**
   ```bash
   # Should contain project.yml file
   ls project.yml
   ```

2. **Regenerate the project:**
   ```bash
   ceedling clean
   ceedling test:all
   ```

3. **Check project.yml configuration:**
   - Ensure `:test_file_prefix: test_` is set
   - Verify paths are correct

### "Function 'math_add' not found"
**Problem:** Implementation file missing or not linked properly

**Solutions:**
1. **Check file locations:**
   ```bash
   ls src/math_utils.h     # Header should exist
   ls src/math_utils.c     # Implementation should exist
   ls test/test_math_utils.c  # Test should exist
   ```

2. **Verify includes in test file:**
   ```c
   #include "unity.h"
   #include "math_utils.h"  // This should match your header file name
   ```

3. **Check function signatures match between .h and .c files:**
   ```c
   // In math_utils.h
   int math_add(int a, int b);

   // In math_utils.c
   int math_add(int a, int b)  // Must match exactly
   {
       return a + b;
   }
   ```

### "Multiple definition" Errors
**Problem:** Function defined multiple times

**Solutions:**
1. **Check for duplicate function definitions:**
   - Only define functions once in the .c file
   - Only declare functions in the .h file

2. **Verify header guards:**
   ```c
   #ifndef MATH_UTILS_H
   #define MATH_UTILS_H
   // ... declarations ...
   #endif // MATH_UTILS_H
   ```

## 🧪 Test Issues

### Tests Not Running
**Problem:** Test functions not being discovered

**Solutions:**
1. **Check test function naming:**
   ```c
   // Correct - starts with "test_"
   void test_math_add_should_return_sum(void)

   // Wrong - doesn't start with "test_"
   void check_math_add(void)
   ```

2. **Verify test file naming:**
   ```bash
   # Correct
   test/test_math_utils.c

   # Wrong
   test/math_utils_test.c
   ```

3. **Check for compilation errors:**
   ```bash
   ceedling test:test_math_utils
   # Look for any error messages
   ```

### Test Assertions Failing
**Problem:** Tests failing unexpectedly

**Solutions:**
1. **Check exact assertion syntax:**
   ```c
   // Correct
   TEST_ASSERT_EQUAL(expected, actual);

   // Wrong order
   TEST_ASSERT_EQUAL(actual, expected);
   ```

2. **Debug with printf (temporarily):**
   ```c
   void test_math_add_should_work(void)
   {
       int result = math_add(5, 3);
       printf("Result: %d\n", result);  // Remove after debugging
       TEST_ASSERT_EQUAL(8, result);
   }
   ```

3. **Check for off-by-one errors:**
   ```c
   // Common mistake
   if (number < 0)  // Should be number <= 0 for some cases
   ```

### "'TEST_ASSERT_EQUAL' not found"
**Problem:** Unity macros not available

**Solutions:**
1. **Verify unity.h is included:**
   ```c
   #include "unity.h"  // Must be first include
   #include "math_utils.h"
   ```

2. **Check Ceedling configuration:**
   ```yaml
   # In project.yml
   :plugins:
     :enabled:
       - stdout_pretty_tests_report
   ```

## 🔄 Logic Issues

### Division by Zero Handling
**Problem:** Program crashes on division by zero

**Solutions:**
1. **Add explicit check:**
   ```c
   int math_divide(int a, int b)
   {
       if (b == 0) {
           return 0;  // Or handle as specified in requirements
       }
       return a / b;
   }
   ```

2. **Test the edge case:**
   ```c
   void test_math_divide_should_handle_zero_divisor(void)
   {
       int result = math_divide(10, 0);
       TEST_ASSERT_EQUAL(0, result);  // Based on requirements
   }
   ```

### Integer Overflow Issues
**Problem:** Arithmetic operations overflow

**Solutions:**
1. **Basic overflow detection:**
   ```c
   int math_add(int a, int b)
   {
       if (a > 0 && b > 0 && a > INT_MAX - b) {
           return INT_MAX;  // Overflow
       }
       return a + b;
   }
   ```

2. **Include limits.h:**
   ```c
   #include <limits.h>  // For INT_MAX, INT_MIN
   ```

## 🏃 Performance Issues

### Tests Running Slowly
**Problem:** Test execution takes too long

**Solutions:**
1. **Reduce test complexity:**
   ```c
   // Instead of testing many values in one test
   void test_all_numbers(void)
   {
       for (int i = 0; i < 10000; i++) {  // Too many iterations
           // ...
       }
   }

   // Create focused, fast tests
   void test_specific_case(void)
   {
       TEST_ASSERT_EQUAL(8, math_add(5, 3));  // Fast and focused
   }
   ```

2. **Use parallel testing:**
   ```yaml
   # In project.yml
   :project:
     :test_threads: 8
     :compile_threads: 8
   ```

## 🎯 Best Practices Reminders

### Test Naming
```c
// Good - describes behavior clearly
void test_math_add_should_return_sum_when_given_positive_numbers(void)

// Bad - unclear what's being tested
void test_add(void)
void test_1(void)
```

### Test Structure
```c
void test_function_should_behavior_when_condition(void)
{
    // ARRANGE - Set up test data
    int input1 = 5;
    int input2 = 3;
    int expected = 8;

    // ACT - Call the function being tested
    int actual = math_add(input1, input2);

    // ASSERT - Verify the result
    TEST_ASSERT_EQUAL(expected, actual);
}
```

### One Assertion Per Test (Usually)
```c
// Good - tests one specific behavior
void test_math_is_even_should_return_true_for_even_numbers(void)
{
    TEST_ASSERT_TRUE(math_is_even(2));
}

// Acceptable - related assertions
void test_math_is_even_should_handle_zero_and_negatives(void)
{
    TEST_ASSERT_TRUE(math_is_even(0));   // Zero is even
    TEST_ASSERT_TRUE(math_is_even(-2));  // Negative even
}
```

## 📞 Getting Additional Help

If you're still stuck after trying these solutions:

1. **Check the solution directory** for reference implementations
2. **Review the demo sessions** for similar examples
3. **Ask on team forums** or during office hours
4. **Pair with a colleague** who has completed the exercise
5. **Take a break** and come back with fresh eyes

## ✅ Success Indicators

You know you've successfully completed the exercise when:

- [ ] All tests pass with `ceedling test:all`
- [ ] You can explain what each test validates
- [ ] You can intentionally break a test and see it fail
- [ ] You understand the Red-Green-Refactor cycle
- [ ] You feel confident writing new tests for additional functions

---

**Remember:** The goal is learning, not perfection. Every error teaches you something valuable about testing and development!