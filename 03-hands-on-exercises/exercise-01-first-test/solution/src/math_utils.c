#include "math_utils.h"

int math_add(int a, int b)
{
    // Check for overflow (basic check - not comprehensive)
    if (a > 0 && b > 0 && a > INT_MAX - b) {
        return INT_MAX;  // Overflow case
    }
    if (a < 0 && b < 0 && a < INT_MIN - b) {
        return INT_MIN;  // Underflow case
    }

    return a + b;
}

int math_subtract(int a, int b)
{
    // Check for underflow/overflow
    if (b > 0 && a < INT_MIN + b) {
        return INT_MIN;  // Underflow case
    }
    if (b < 0 && a > INT_MAX + b) {
        return INT_MAX;  // Overflow case
    }

    return a - b;
}

int math_multiply(int a, int b)
{
    // Handle simple cases
    if (a == 0 || b == 0) {
        return 0;
    }

    // Basic overflow check (simplified)
    if (a > 0 && b > 0 && a > INT_MAX / b) {
        return INT_MAX;
    }
    if (a < 0 && b < 0 && a < INT_MAX / b) {
        return INT_MAX;
    }
    if ((a > 0 && b < 0 && b < INT_MIN / a) ||
        (a < 0 && b > 0 && a < INT_MIN / b)) {
        return INT_MIN;
    }

    return a * b;
}

int math_divide(int a, int b)
{
    if (b == 0) {
        return 0;  // Division by zero returns 0 as specified
    }

    // Handle INT_MIN / -1 case which would overflow
    if (a == INT_MIN && b == -1) {
        return INT_MAX;  // Prevent overflow
    }

    return a / b;
}

bool math_is_even(int number)
{
    return (number % 2) == 0;
}

bool math_is_positive(int number)
{
    return number > 0;
}

int math_absolute(int number)
{
    if (number == INT_MIN) {
        return INT_MAX;  // Can't represent positive INT_MIN
    }

    if (number < 0) {
        return -number;
    }

    return number;
}

int math_power(int base, int exponent)
{
    // Handle negative exponents (not supported)
    if (exponent < 0) {
        return 0;
    }

    // Handle special cases
    if (exponent == 0) {
        return 1;  // Any number to power 0 is 1
    }

    if (base == 0) {
        return 0;  // 0 to any positive power is 0
    }

    if (base == 1) {
        return 1;  // 1 to any power is 1
    }

    // Calculate power iteratively
    int result = 1;
    for (int i = 0; i < exponent; i++) {
        // Simple overflow check
        if (result > INT_MAX / base) {
            return INT_MAX;  // Overflow
        }
        result *= base;
    }

    return result;
}