// CEEDLING NOTICE: This generated file only to be consumed for test runner creation

#include "build/vendor/unity/src/unity.h"
#include "src/led_control.h"

void setUp(void)
{
    led_control_init();
}

void tearDown(void)
{
}

void test_led_control_init_should_succeed(void)
{
    int result = led_control_init();
    UnityAssertEqualNumber((UNITY_INT)((0)), (UNITY_INT)((result)), (
   ((void *)0)
   ), (UNITY_UINT)(24), UNITY_DISPLAY_STYLE_INT);
}

void test_led_control_init_clears_state(void)
{
    red_led_on();
    led_control_init();
    do { if (!(led_control_get(LED_COLOR_RED)))
{ } else { UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((32))); } } while (0);
    do { if (!(led_control_get(LED_COLOR_GREEN)))
{ } else { UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((33))); } } while (0);
    do { if (!(led_control_get(LED_COLOR_BLUE)))
{ } else { UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((34))); } } while (0);
}

void test_red_led_on_sets_red(void)
{
    int result = red_led_on();
    UnityAssertEqualNumber((UNITY_INT)((0)), (UNITY_INT)((result)), (
   ((void *)0)
   ), (UNITY_UINT)(42), UNITY_DISPLAY_STYLE_INT);
    do { if ((led_control_get(LED_COLOR_RED)))
{ } else { UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((43))); } } while (0);
}

void test_red_led_off_clears_red(void)
{
    red_led_on();
    int result = red_led_off();
    UnityAssertEqualNumber((UNITY_INT)((0)), (UNITY_INT)((result)), (
   ((void *)0)
   ), (UNITY_UINT)(50), UNITY_DISPLAY_STYLE_INT);
    do { if (!(led_control_get(LED_COLOR_RED)))
{ } else { UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((51))); } } while (0);
}

void test_red_led_on_does_not_affect_green(void)
{
    red_led_on();
    do { if (!(led_control_get(LED_COLOR_GREEN)))
{ } else { UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((57))); } } while (0);
}

void test_green_led_on_sets_green(void)
{
    int result = green_led_on();
    UnityAssertEqualNumber((UNITY_INT)((0)), (UNITY_INT)((result)), (
   ((void *)0)
   ), (UNITY_UINT)(65), UNITY_DISPLAY_STYLE_INT);
    do { if ((led_control_get(LED_COLOR_GREEN)))
{ } else { UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((66))); } } while (0);
}

void test_green_led_off_clears_green(void)
{
    green_led_on();
    int result = green_led_off();
    UnityAssertEqualNumber((UNITY_INT)((0)), (UNITY_INT)((result)), (
   ((void *)0)
   ), (UNITY_UINT)(73), UNITY_DISPLAY_STYLE_INT);
    do { if (!(led_control_get(LED_COLOR_GREEN)))
{ } else { UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((74))); } } while (0);
}

void test_green_led_on_does_not_affect_red(void)
{
    green_led_on();
    do { if (!(led_control_get(LED_COLOR_RED)))
{ } else { UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((80))); } } while (0);
}

void test_blue_led_on_sets_blue(void)
{
    int result = blue_led_on();
    UnityAssertEqualNumber((UNITY_INT)((0)), (UNITY_INT)((result)), (
   ((void *)0)
   ), (UNITY_UINT)(88), UNITY_DISPLAY_STYLE_INT);
    do { if ((led_control_get(LED_COLOR_BLUE)))
{ } else { UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((89))); } } while (0);
}

void test_blue_led_off_clears_blue(void)
{
    blue_led_on();
    blue_led_off();
    do { if (!(led_control_get(LED_COLOR_BLUE)))
{ } else { UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((96))); } } while (0);
}

void test_toggle_blue_changes_state(void)
{
    led_control_set(LED_COLOR_BLUE,
                                   0
                                        );
    led_control_toggle(LED_COLOR_BLUE);
    do { if ((led_control_get(LED_COLOR_BLUE)))
{ } else { UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((105))); } } while (0);

    led_control_toggle(LED_COLOR_BLUE);
    do { if (!(led_control_get(LED_COLOR_BLUE)))
{ } else { UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((108))); } } while (0);
}

void test_all_three_leds_independent(void)
{
    red_led_on();
    green_led_on();
    blue_led_on();

    do { if ((led_control_get(LED_COLOR_RED)))
{ } else { UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((119))); } } while (0);
    do { if ((led_control_get(LED_COLOR_GREEN)))
{ } else { UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((120))); } } while (0);
    do { if ((led_control_get(LED_COLOR_BLUE)))
{ } else { UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((121))); } } while (0);

    red_led_off();
    do { if (!(led_control_get(LED_COLOR_RED)))
{ } else { UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((124))); } } while (0);
    do { if ((led_control_get(LED_COLOR_GREEN)))
{ } else { UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((125))); } } while (0);
    do { if ((led_control_get(LED_COLOR_BLUE)))
{ } else { UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((126))); } } while (0);
}

void test_get_state_reflects_active_leds(void)
{
    red_led_on();
    green_led_on();
    uint8_t state = led_control_get_state();
    UnityAssertBits((UNITY_INT)((LED_COLOR_RED)), (UNITY_INT)((LED_COLOR_RED)), (UNITY_INT)((state)), (
   ((void *)0)
   ), (UNITY_UINT)(136));
    UnityAssertBits((UNITY_INT)((LED_COLOR_GREEN)), (UNITY_INT)((LED_COLOR_GREEN)), (UNITY_INT)((state)), (
   ((void *)0)
   ), (UNITY_UINT)(137));
    UnityAssertBits((UNITY_INT)((LED_COLOR_BLUE)), (UNITY_INT)((0)), (UNITY_INT)((state)), (
   ((void *)0)
   ), (UNITY_UINT)(138));
}
void test_safety_logic_normal_conditions(void)
{
    int32_t temperature = 30000;
    uint32_t voltage = 3900;

    led_control_init();
    red_led_on();

    if (temperature > 40000 && voltage >= 4200)
{
        green_led_on();
    }

    do { if ((led_control_get(LED_COLOR_RED)))
{ } else { UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((160))); } } while (0);
    do { if (!(led_control_get(LED_COLOR_GREEN)))
{ } else { UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((161))); } } while (0);
}

void test_safety_logic_over_temp_only(void)
{
    int32_t temperature = 45000;
    uint32_t voltage = 3900;

    led_control_init();
    red_led_on();

    if (temperature > 40000 && voltage >= 4200)
{
        green_led_on();
    }

    do { if ((led_control_get(LED_COLOR_RED)))
{ } else { UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((176))); } } while (0);
    do { if (!(led_control_get(LED_COLOR_GREEN)))
{ } else { UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((177))); } } while (0);
}

void test_safety_logic_over_voltage_only(void)
{
    int32_t temperature = 30000;
    uint32_t voltage = 4300;

    led_control_init();
    red_led_on();

    if (temperature > 40000 && voltage >= 4200)
{
        green_led_on();
    }

    do { if ((led_control_get(LED_COLOR_RED)))
{ } else { UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((192))); } } while (0);
    do { if (!(led_control_get(LED_COLOR_GREEN)))
{ } else { UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((193))); } } while (0);
}

void test_safety_logic_both_thresholds_exceeded(void)
{
    int32_t temperature = 41000;
    uint32_t voltage = 4200;

    led_control_init();
    red_led_on();

    if (temperature > 40000 && voltage >= 4200)
{
        green_led_on();
    }

    do { if ((led_control_get(LED_COLOR_RED)))
{ } else { UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((208))); } } while (0);
    do { if ((led_control_get(LED_COLOR_GREEN)))
{ } else { UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((209))); } } while (0);
}