# 📋 Exercise 1: First Test - Requirements

## 🎯 Learning Objectives

After completing this exercise, you will:
- ✅ Write your first unit test from scratch
- ✅ Understand the Arrange-Act-Assert pattern
- ✅ Use basic Unity assertions confidently
- ✅ Experience the Red-Green-Refactor cycle
- ✅ Set up a complete Ceedling project

## 📝 What You Need to Build

Create a **Math Utilities Library** with the following functions:

### Core Functions (Required)

```c
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
```

### Function Specifications

#### `math_add(int a, int b)`
- **Purpose**: Add two integers
- **Returns**: Sum of a and b
- **Special Cases**: Handle integer overflow (return INT_MAX for overflow)

#### `math_subtract(int a, int b)`
- **Purpose**: Subtract second integer from first
- **Returns**: Result of a - b
- **Special Cases**: Handle underflow (return INT_MIN for underflow)

#### `math_multiply(int a, int b)`
- **Purpose**: Multiply two integers
- **Returns**: Product of a and b
- **Special Cases**: Handle overflow, multiplication by zero

#### `math_divide(int a, int b)`
- **Purpose**: Divide first integer by second
- **Returns**: Result of a / b (integer division)
- **Special Cases**: Return 0 for division by zero, handle negative numbers correctly

#### `math_is_even(int number)`
- **Purpose**: Check if number is even
- **Returns**: true if even, false if odd
- **Special Cases**: Handle negative numbers, zero

#### `math_is_positive(int number)`
- **Purpose**: Check if number is greater than zero
- **Returns**: true if positive, false if zero or negative
- **Special Cases**: Zero should return false

#### `math_absolute(int number)`
- **Purpose**: Get absolute value of number
- **Returns**: Absolute value
- **Special Cases**: Handle INT_MIN carefully (can't represent positive equivalent)

#### `math_power(int base, int exponent)`
- **Purpose**: Calculate base raised to exponent power
- **Returns**: base^exponent
- **Special Cases**:
  - Any number to power 0 = 1
  - 0 to any positive power = 0
  - Negative exponents not supported (return 0)

## 🧪 Required Test Cases

Write tests for these scenarios (minimum):

### Basic Functionality Tests
1. **Addition with positive numbers**
2. **Addition with negative numbers**
3. **Addition with zero**
4. **Subtraction with positive result**
5. **Subtraction with negative result**
6. **Multiplication by positive numbers**
7. **Multiplication by zero**
8. **Division with exact result**
9. **Division with remainder (integer division)**

### Edge Case Tests
10. **Division by zero** (should return 0)
11. **Even number detection** (2, 4, 100)
12. **Odd number detection** (1, 3, 99)
13. **Even/odd with negative numbers**
14. **Positive number detection**
15. **Zero and negative number with is_positive**
16. **Absolute value of positive number**
17. **Absolute value of negative number**
18. **Absolute value of zero**

### Advanced Tests (Bonus)
19. **Power function with positive exponent**
20. **Power function with zero exponent**
21. **Power function with zero base**
22. **Integer overflow conditions** (where applicable)

## ✅ Success Criteria

Your exercise is complete when:

### Code Quality
- [ ] All functions implemented according to specifications
- [ ] Code compiles without warnings
- [ ] Functions handle edge cases as specified
- [ ] Clear, readable code with meaningful variable names

### Test Coverage
- [ ] All required test cases implemented
- [ ] Tests follow Arrange-Act-Assert pattern
- [ ] Test names clearly describe what is being tested
- [ ] All tests pass when run with `ceedling test:all`

### Understanding Demonstration
- [ ] Can explain what each test is validating
- [ ] Understands when and why tests fail
- [ ] Can modify tests and see expected failures
- [ ] Knows how to add new test cases

## 📊 Complexity Levels

Choose your challenge level:

### 🌱 Beginner Level
- Implement basic arithmetic functions
- Write 10-15 simple test cases
- Focus on happy path scenarios
- **Time Estimate**: 1-2 hours

### 🚀 Intermediate Level
- Implement all required functions
- Write all required test cases including edge cases
- Add input validation
- **Time Estimate**: 2-3 hours

### 🎯 Advanced Level
- Implement all functions with comprehensive error handling
- Write parameterized tests for comprehensive coverage
- Add bonus functions (GCD, factorial, etc.)
- **Time Estimate**: 3-4 hours

## 🛠️ Tools and Resources

### Required Tools
- Ceedling framework installed and working
- C compiler (GCC recommended)
- Text editor or IDE

### Helpful Resources
- Unity assertion reference (Unity/docs/UnityAssertionsReference.md)
- C standard library documentation
- Integer limits (INT_MAX, INT_MIN) from limits.h

### Unity Assertions You'll Use
```c
TEST_ASSERT_EQUAL(expected, actual)
TEST_ASSERT_TRUE(condition)
TEST_ASSERT_FALSE(condition)
TEST_ASSERT_EQUAL_MESSAGE(expected, actual, message)
```

## 📚 Learning Path

### Step 1: Project Setup (15 minutes)
1. Create new Ceedling project
2. Set up basic project structure
3. Verify build system works

### Step 2: Header File (10 minutes)
1. Define function signatures
2. Include necessary headers
3. Add function documentation

### Step 3: First Test (20 minutes)
1. Write one simple test (e.g., math_add)
2. Experience Red phase (test fails)
3. Implement minimal function
4. Experience Green phase (test passes)

### Step 4: Test-Driven Development (45 minutes)
1. Write failing test for next function
2. Implement minimal code to pass
3. Refactor if needed
4. Repeat for all functions

### Step 5: Edge Cases (30 minutes)
1. Add edge case tests
2. Update implementations to handle edge cases
3. Verify all tests pass

### Step 6: Validation (15 minutes)
1. Run complete test suite
2. Verify coverage
3. Test failure scenarios
4. Clean up code

## 🚨 Common Pitfalls to Avoid

### Test-Related Pitfalls
- ❌ Writing tests after implementation (misses design benefits)
- ❌ Testing implementation details instead of behavior
- ❌ Tests that depend on execution order
- ❌ Unclear or misleading test names

### Implementation Pitfalls
- ❌ Not handling edge cases specified in requirements
- ❌ Inconsistent error handling across functions
- ❌ Ignoring integer overflow/underflow possibilities
- ❌ Complex implementations when simple ones suffice

### Process Pitfalls
- ❌ Writing multiple tests before any pass (stay in Red too long)
- ❌ Over-engineering solutions in the Green phase
- ❌ Skipping the Refactor phase
- ❌ Not running tests frequently enough

## 🎯 Assessment Questions

Test your understanding by answering these questions:

1. **Why write tests before implementation?**
2. **What happens if you skip the Red phase?**
3. **When should you refactor code?**
4. **How do you know if a test is good?**
5. **What makes a function easy to test?**

## 🏆 Bonus Challenges

If you complete the basic requirements:

### Bonus Functions
```c
int math_factorial(int n);
int math_gcd(int a, int b);
bool math_is_prime(int number);
int math_fibonacci(int n);
```

### Bonus Testing Techniques
- Write parameterized tests using arrays of test data
- Add performance tests for large inputs
- Create test helpers for common assertion patterns
- Add negative testing for invalid inputs

---

**Remember**: The goal is learning, not perfection. Take your time, experiment, and don't hesitate to ask for help!