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

// Example test function - you can use this as a template for your tests
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

// TODO: Add more test functions following the pattern above
// Remember to:
// 1. Name your tests descriptively (test_function_should_behavior_when_condition)
// 2. Follow Arrange-Act-Assert pattern
// 3. Test one specific behavior per test function

// Suggested tests to implement:
//
// void test_math_add_should_handle_negative_numbers_correctly(void)
// void test_math_add_should_handle_zero_correctly(void)
// void test_math_subtract_should_return_difference_of_two_numbers(void)
// void test_math_multiply_should_return_product_of_two_numbers(void)
// void test_math_multiply_should_return_zero_when_multiplied_by_zero(void)
// void test_math_divide_should_return_quotient_when_divisor_is_not_zero(void)
// void test_math_divide_should_return_zero_when_dividing_by_zero(void)
// void test_math_is_even_should_return_true_for_even_numbers(void)
// void test_math_is_even_should_return_false_for_odd_numbers(void)
// void test_math_is_positive_should_return_true_for_positive_numbers(void)
// void test_math_is_positive_should_return_false_for_zero_and_negative_numbers(void)