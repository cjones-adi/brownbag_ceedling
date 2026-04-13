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

// ===== ADDITION TESTS =====

void test_math_add_should_return_sum_of_two_positive_numbers(void)
{
    // Arrange
    int first_number = 5;
    int second_number = 3;
    int expected_result = 8;

    // Act
    int actual_result = math_add(first_number, second_number);

    // Assert
    TEST_ASSERT_EQUAL(expected_result, actual_result);
}

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

void test_math_add_should_handle_zero_correctly(void)
{
    // Test adding zero to a positive number
    TEST_ASSERT_EQUAL(42, math_add(42, 0));

    // Test adding zero to a negative number
    TEST_ASSERT_EQUAL(-15, math_add(-15, 0));

    // Test adding zero to zero
    TEST_ASSERT_EQUAL(0, math_add(0, 0));
}

void test_math_add_should_handle_two_negative_numbers(void)
{
    // Arrange
    int first_negative = -10;
    int second_negative = -5;
    int expected_result = -15;

    // Act
    int actual_result = math_add(first_negative, second_negative);

    // Assert
    TEST_ASSERT_EQUAL(expected_result, actual_result);
}

// ===== SUBTRACTION TESTS =====

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

void test_math_subtract_should_handle_negative_result(void)
{
    // Arrange
    int minuend = 3;
    int subtrahend = 8;
    int expected_result = -5;

    // Act
    int actual_result = math_subtract(minuend, subtrahend);

    // Assert
    TEST_ASSERT_EQUAL(expected_result, actual_result);
}

void test_math_subtract_should_handle_zero(void)
{
    // Test subtracting zero
    TEST_ASSERT_EQUAL(15, math_subtract(15, 0));

    // Test subtracting from zero
    TEST_ASSERT_EQUAL(-15, math_subtract(0, 15));
}

// ===== MULTIPLICATION TESTS =====

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

void test_math_multiply_should_return_zero_when_multiplied_by_zero(void)
{
    // Test zero times positive number
    TEST_ASSERT_EQUAL(0, math_multiply(100, 0));

    // Test positive number times zero
    TEST_ASSERT_EQUAL(0, math_multiply(0, 100));

    // Test zero times zero
    TEST_ASSERT_EQUAL(0, math_multiply(0, 0));
}

void test_math_multiply_should_handle_negative_numbers(void)
{
    // Positive * negative = negative
    TEST_ASSERT_EQUAL(-20, math_multiply(4, -5));

    // Negative * positive = negative
    TEST_ASSERT_EQUAL(-20, math_multiply(-4, 5));

    // Negative * negative = positive
    TEST_ASSERT_EQUAL(20, math_multiply(-4, -5));
}

// ===== DIVISION TESTS =====

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

void test_math_divide_should_handle_integer_division_correctly(void)
{
    // Integer division truncates toward zero
    TEST_ASSERT_EQUAL(3, math_divide(10, 3));  // 10/3 = 3 remainder 1
    TEST_ASSERT_EQUAL(2, math_divide(7, 3));   // 7/3 = 2 remainder 1
}

void test_math_divide_should_handle_negative_numbers(void)
{
    // Positive / negative = negative
    TEST_ASSERT_EQUAL(-5, math_divide(15, -3));

    // Negative / positive = negative
    TEST_ASSERT_EQUAL(-5, math_divide(-15, 3));

    // Negative / negative = positive
    TEST_ASSERT_EQUAL(5, math_divide(-15, -3));
}

// ===== EVEN NUMBER TESTS =====

void test_math_is_even_should_return_true_for_even_numbers(void)
{
    TEST_ASSERT_TRUE(math_is_even(2));
    TEST_ASSERT_TRUE(math_is_even(4));
    TEST_ASSERT_TRUE(math_is_even(0));      // Zero is even
    TEST_ASSERT_TRUE(math_is_even(100));
    TEST_ASSERT_TRUE(math_is_even(-4));     // Negative even numbers
    TEST_ASSERT_TRUE(math_is_even(-100));
}

void test_math_is_even_should_return_false_for_odd_numbers(void)
{
    TEST_ASSERT_FALSE(math_is_even(1));
    TEST_ASSERT_FALSE(math_is_even(3));
    TEST_ASSERT_FALSE(math_is_even(99));
    TEST_ASSERT_FALSE(math_is_even(-1));    // Negative odd numbers
    TEST_ASSERT_FALSE(math_is_even(-3));
    TEST_ASSERT_FALSE(math_is_even(-99));
}

// ===== POSITIVE NUMBER TESTS =====

void test_math_is_positive_should_return_true_for_positive_numbers(void)
{
    TEST_ASSERT_TRUE(math_is_positive(1));
    TEST_ASSERT_TRUE(math_is_positive(100));
    TEST_ASSERT_TRUE(math_is_positive(999999));
}

void test_math_is_positive_should_return_false_for_zero_and_negative_numbers(void)
{
    // Zero is not positive
    TEST_ASSERT_FALSE(math_is_positive(0));

    // Negative numbers are not positive
    TEST_ASSERT_FALSE(math_is_positive(-1));
    TEST_ASSERT_FALSE(math_is_positive(-100));
    TEST_ASSERT_FALSE(math_is_positive(-999999));
}

// ===== ABSOLUTE VALUE TESTS =====

void test_math_absolute_should_return_positive_value_for_negative_input(void)
{
    TEST_ASSERT_EQUAL(5, math_absolute(-5));
    TEST_ASSERT_EQUAL(100, math_absolute(-100));
    TEST_ASSERT_EQUAL(1, math_absolute(-1));
}

void test_math_absolute_should_return_same_value_for_positive_input(void)
{
    TEST_ASSERT_EQUAL(5, math_absolute(5));
    TEST_ASSERT_EQUAL(100, math_absolute(100));
    TEST_ASSERT_EQUAL(1, math_absolute(1));
}

void test_math_absolute_should_return_zero_for_zero_input(void)
{
    TEST_ASSERT_EQUAL(0, math_absolute(0));
}

// ===== POWER FUNCTION TESTS =====

void test_math_power_should_calculate_power_correctly(void)
{
    TEST_ASSERT_EQUAL(8, math_power(2, 3));    // 2^3 = 8
    TEST_ASSERT_EQUAL(25, math_power(5, 2));   // 5^2 = 25
    TEST_ASSERT_EQUAL(1000, math_power(10, 3)); // 10^3 = 1000
}

void test_math_power_should_return_one_for_power_of_zero(void)
{
    TEST_ASSERT_EQUAL(1, math_power(5, 0));    // Any number^0 = 1
    TEST_ASSERT_EQUAL(1, math_power(100, 0));  // Any number^0 = 1
    TEST_ASSERT_EQUAL(1, math_power(1, 0));    // 1^0 = 1
}

void test_math_power_should_return_zero_for_zero_to_positive_power(void)
{
    TEST_ASSERT_EQUAL(0, math_power(0, 1));    // 0^positive = 0
    TEST_ASSERT_EQUAL(0, math_power(0, 5));    // 0^positive = 0
    TEST_ASSERT_EQUAL(0, math_power(0, 100));  // 0^positive = 0
}

void test_math_power_should_return_zero_for_negative_exponent(void)
{
    // Negative exponents not supported - return 0
    TEST_ASSERT_EQUAL(0, math_power(5, -1));
    TEST_ASSERT_EQUAL(0, math_power(2, -3));
}

void test_math_power_should_handle_power_of_one(void)
{
    TEST_ASSERT_EQUAL(1, math_power(1, 5));    // 1^anything = 1
    TEST_ASSERT_EQUAL(1, math_power(1, 100));  // 1^anything = 1
}