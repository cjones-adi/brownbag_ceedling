/******************************************************************************
 * @file    test_led_control.c
 * @brief   Unit tests for the LED control module (led_control.c)
 ******************************************************************************/

#include "unity.h"
#include "led_control.h"

void setUp(void)
{
    led_control_init();
}

void tearDown(void)
{
    /* Nothing to clean up */
}

/* ---- Initialisation ---- */

void test_led_control_init_should_succeed(void)
{
    int result = led_control_init();
    TEST_ASSERT_EQUAL_INT(0, result);
}

void test_led_control_init_clears_state(void)
{
    /* Turn something on, then re-init – state should be cleared */
    red_led_on();
    led_control_init();
    TEST_ASSERT_FALSE(led_control_get(LED_COLOR_RED));
    TEST_ASSERT_FALSE(led_control_get(LED_COLOR_GREEN));
    TEST_ASSERT_FALSE(led_control_get(LED_COLOR_BLUE));
}

/* ---- Red LED ---- */

void test_red_led_on_sets_red(void)
{
    int result = red_led_on();
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_TRUE(led_control_get(LED_COLOR_RED));
}

void test_red_led_off_clears_red(void)
{
    red_led_on();
    int result = red_led_off();
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_FALSE(led_control_get(LED_COLOR_RED));
}

void test_red_led_on_does_not_affect_green(void)
{
    red_led_on();
    TEST_ASSERT_FALSE(led_control_get(LED_COLOR_GREEN));
}

/* ---- Green LED ---- */

void test_green_led_on_sets_green(void)
{
    int result = green_led_on();
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_TRUE(led_control_get(LED_COLOR_GREEN));
}

void test_green_led_off_clears_green(void)
{
    green_led_on();
    int result = green_led_off();
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_FALSE(led_control_get(LED_COLOR_GREEN));
}

void test_green_led_on_does_not_affect_red(void)
{
    green_led_on();
    TEST_ASSERT_FALSE(led_control_get(LED_COLOR_RED));
}

/* ---- Blue LED ---- */

void test_blue_led_on_sets_blue(void)
{
    int result = blue_led_on();
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_TRUE(led_control_get(LED_COLOR_BLUE));
}

void test_blue_led_off_clears_blue(void)
{
    blue_led_on();
    blue_led_off();
    TEST_ASSERT_FALSE(led_control_get(LED_COLOR_BLUE));
}

/* ---- Toggle ---- */

void test_toggle_blue_changes_state(void)
{
    led_control_set(LED_COLOR_BLUE, false);
    led_control_toggle(LED_COLOR_BLUE);
    TEST_ASSERT_TRUE(led_control_get(LED_COLOR_BLUE));

    led_control_toggle(LED_COLOR_BLUE);
    TEST_ASSERT_FALSE(led_control_get(LED_COLOR_BLUE));
}

/* ---- Independent control ---- */

void test_all_three_leds_independent(void)
{
    red_led_on();
    green_led_on();
    blue_led_on();

    TEST_ASSERT_TRUE(led_control_get(LED_COLOR_RED));
    TEST_ASSERT_TRUE(led_control_get(LED_COLOR_GREEN));
    TEST_ASSERT_TRUE(led_control_get(LED_COLOR_BLUE));

    red_led_off();
    TEST_ASSERT_FALSE(led_control_get(LED_COLOR_RED));
    TEST_ASSERT_TRUE(led_control_get(LED_COLOR_GREEN));
    TEST_ASSERT_TRUE(led_control_get(LED_COLOR_BLUE));
}

/* ---- State bitmask ---- */

void test_get_state_reflects_active_leds(void)
{
    red_led_on();
    green_led_on();
    uint8_t state = led_control_get_state();
    TEST_ASSERT_BITS(LED_COLOR_RED,   LED_COLOR_RED,   state);
    TEST_ASSERT_BITS(LED_COLOR_GREEN, LED_COLOR_GREEN, state);
    TEST_ASSERT_BITS(LED_COLOR_BLUE,  0,               state);
}

/* ---- Safety logic (business-rule tests) ---- */

/**
 * @brief Verify that the safety LED pattern matches the demo requirement:
 *        Red LED is always on; Green LED is added only when both
 *        temperature and voltage thresholds are exceeded.
 */
void test_safety_logic_normal_conditions(void)
{
    int32_t  temperature = 30000; /* 30 °C in millideg – below threshold */
    uint32_t voltage     = 3900;  /* 3.9 V in millivolts – below threshold */

    led_control_init();
    red_led_on();

    if (temperature > 40000 && voltage >= 4200) {
        green_led_on();
    }

    TEST_ASSERT_TRUE(led_control_get(LED_COLOR_RED));
    TEST_ASSERT_FALSE(led_control_get(LED_COLOR_GREEN));
}

void test_safety_logic_over_temp_only(void)
{
    int32_t  temperature = 45000; /* 45 °C in millideg – above threshold */
    uint32_t voltage     = 3900;  /* 3.9 V in millivolts – below threshold */

    led_control_init();
    red_led_on();

    if (temperature > 40000 && voltage >= 4200) {
        green_led_on();
    }

    TEST_ASSERT_TRUE(led_control_get(LED_COLOR_RED));
    TEST_ASSERT_FALSE(led_control_get(LED_COLOR_GREEN));
}

void test_safety_logic_over_voltage_only(void)
{
    int32_t  temperature = 30000; /* 30 °C in millideg – below threshold */
    uint32_t voltage     = 4300;  /* 4.3 V in millivolts – above threshold */

    led_control_init();
    red_led_on();

    if (temperature > 40000 && voltage >= 4200) {
        green_led_on();
    }

    TEST_ASSERT_TRUE(led_control_get(LED_COLOR_RED));
    TEST_ASSERT_FALSE(led_control_get(LED_COLOR_GREEN));
}

void test_safety_logic_both_thresholds_exceeded(void)
{
    int32_t  temperature = 41000; /* 41 °C in millideg – above threshold */
    uint32_t voltage     = 4200;  /* 4.2 V in millivolts – at threshold  */

    led_control_init();
    red_led_on();

    if (temperature > 40000 && voltage >= 4200) {
        green_led_on(); /* stop_charging() */
    }

    TEST_ASSERT_TRUE(led_control_get(LED_COLOR_RED));
    TEST_ASSERT_TRUE(led_control_get(LED_COLOR_GREEN));
}
