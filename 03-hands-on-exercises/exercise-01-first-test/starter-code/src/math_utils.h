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