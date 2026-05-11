/******************************************************************************
 * @file    test_led_control.c
 * @brief   Unit tests for LED control module
 ******************************************************************************/

#include "unity.h"
#include "led_control.h"

// This function is called before each test
void setUp(void)
{
    // Initialize the LED control before each test
    led_control_init();
}

// This function is called after each test
void tearDown(void)
{
    // Clean up after each test if needed
}

// Test: Initialization should succeed
void test_led_control_init_should_succeed(void)
{
    int result = led_control_init();
    TEST_ASSERT_EQUAL_INT(0, result);
}

// Test: Setting blue LED on should work
void test_led_control_set_blue_on(void)
{
    int result = led_control_set(LED_COLOR_BLUE, true);
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_TRUE(led_control_get(LED_COLOR_BLUE));
}

// Test: Setting blue LED off should work
void test_led_control_set_blue_off(void)
{
    led_control_set(LED_COLOR_BLUE, true);
    int result = led_control_set(LED_COLOR_BLUE, false);
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_FALSE(led_control_get(LED_COLOR_BLUE));
}

// Test: Setting red LED on should work
void test_led_control_set_red_on(void)
{
    int result = led_control_set(LED_COLOR_RED, true);
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_TRUE(led_control_get(LED_COLOR_RED));
}

// Test: Toggle should change LED state
void test_led_control_toggle_blue(void)
{
    // Start with LED off
    led_control_set(LED_COLOR_BLUE, false);
    TEST_ASSERT_FALSE(led_control_get(LED_COLOR_BLUE));
    
    // Toggle on
    led_control_toggle(LED_COLOR_BLUE);
    TEST_ASSERT_TRUE(led_control_get(LED_COLOR_BLUE));
    
    // Toggle off
    led_control_toggle(LED_COLOR_BLUE);
    TEST_ASSERT_FALSE(led_control_get(LED_COLOR_BLUE));
}

// Test: Setting both LEDs should work independently
void test_led_control_set_both_leds(void)
{
    led_control_set(LED_COLOR_BLUE, true);
    led_control_set(LED_COLOR_RED, true);
    
    TEST_ASSERT_TRUE(led_control_get(LED_COLOR_BLUE));
    TEST_ASSERT_TRUE(led_control_get(LED_COLOR_RED));
    
    // Turn off blue, red should still be on
    led_control_set(LED_COLOR_BLUE, false);
    TEST_ASSERT_FALSE(led_control_get(LED_COLOR_BLUE));
    TEST_ASSERT_TRUE(led_control_get(LED_COLOR_RED));
}

// Test: Get should return false for uninitialized LED
void test_led_control_get_initial_state(void)
{
    // Re-initialize to ensure clean state
    led_control_init();
    TEST_ASSERT_FALSE(led_control_get(LED_COLOR_BLUE));
    TEST_ASSERT_FALSE(led_control_get(LED_COLOR_RED));
}
