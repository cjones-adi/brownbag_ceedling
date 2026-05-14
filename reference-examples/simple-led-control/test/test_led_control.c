/******************************************************************************
 * @file    test_led_control.c
 * @brief   Unit tests for led_control — Demo 1 "Simple Start" reference.
 *
 * GOAL
 *   Verify led_control.c logic WITHOUT any hardware, mocks, or board support.
 *   Only Unity and the module under test are needed.
 *
 * CONCEPTS COVERED
 *   - Arrange-Act-Assert (AAA) test structure
 *   - setUp() / tearDown() for test isolation
 *   - Basic Unity assertions: TEST_ASSERT_TRUE/FALSE, TEST_ASSERT_EQUAL
 *   - Boundary-condition testing (out-of-range index)
 *   - Independence: toggling one LED must not affect others
 *
 * HOW TO RUN
 *   ceedling test:test_led_control
 ******************************************************************************/
#include "unity.h"
#include "led_control.h"

/* ==========================================================================
 * setUp / tearDown
 * ========================================================================== */

void setUp(void)
{
    led_control_init();   /* fresh state before every test — no mocks needed */
}

void tearDown(void)
{
    /* nothing to clean up */
}

/* ==========================================================================
 * Group A: Initialisation
 * ========================================================================== */

void test_led_control_should_initialize_all_leds_to_off(void)
{
    for (uint8_t i = 0u; i < led_control_get_max_leds(); i++) {
        TEST_ASSERT_FALSE_MESSAGE(led_control_is_on(i),
                                  "All LEDs must be OFF after init");
    }
}

/* ==========================================================================
 * Group B: Turn on / turn off
 * ========================================================================== */

void test_led_control_turn_on_sets_led_to_on(void)
{
    /* Arrange — setUp already called init */
    uint8_t led = 3u;

    /* Act */
    led_control_turn_on(led);

    /* Assert */
    TEST_ASSERT_TRUE(led_control_is_on(led));
}

void test_led_control_turn_off_clears_led(void)
{
    uint8_t led = 5u;

    led_control_turn_on(led);
    led_control_turn_off(led);

    TEST_ASSERT_FALSE(led_control_is_on(led));
}

void test_led_control_turn_on_does_not_affect_other_leds(void)
{
    led_control_turn_on(0u);
    led_control_turn_on(1u);
    led_control_turn_on(2u);

    led_control_turn_off(1u);

    TEST_ASSERT_TRUE(led_control_is_on(0u));   /* unaffected */
    TEST_ASSERT_FALSE(led_control_is_on(1u));  /* turned off */
    TEST_ASSERT_TRUE(led_control_is_on(2u));   /* unaffected */
}

/* ==========================================================================
 * Group C: Boundary conditions
 * ========================================================================== */

void test_led_control_is_on_returns_false_for_out_of_range_index(void)
{
    uint8_t out_of_range = led_control_get_max_leds(); /* one past the end */

    TEST_ASSERT_FALSE(led_control_is_on(out_of_range));
}

void test_led_control_turn_on_out_of_range_does_not_crash(void)
{
    /* Must not crash or corrupt in-range state */
    led_control_turn_on(255u);

    for (uint8_t i = 0u; i < led_control_get_max_leds(); i++) {
        TEST_ASSERT_FALSE(led_control_is_on(i));
    }
}

/* ==========================================================================
 * Group D: get_max_leds
 * ========================================================================== */

void test_led_control_get_max_leds_returns_nonzero(void)
{
    TEST_ASSERT_GREATER_THAN(0u, led_control_get_max_leds());
}
