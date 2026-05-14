/******************************************************************************
 * @file    led_control.c
 * @brief   LED control — pure software state, no hardware calls.
 *
 *          Demo 1 "Simple Start" reference project.
 *
 *          The implementation deliberately has no GPIO, no HAL, and no board
 *          includes.  It stores LED on/off state in a static bool array and
 *          enforces index bounds.  This makes the module fully testable on
 *          the host with Unity and no mocks whatsoever.
 *
 *          Hardware interaction (gpio_hal, CMock) is introduced in Demo 2.
 ******************************************************************************/
#include "led_control.h"

#define MAX_LEDS 8u

static bool led_states[MAX_LEDS];

void led_control_init(void)
{
    for (uint8_t i = 0u; i < MAX_LEDS; i++) {
        led_states[i] = false;
    }
}

void led_control_turn_on(uint8_t led_num)
{
    if (led_num < MAX_LEDS) {
        led_states[led_num] = true;
    }
}

void led_control_turn_off(uint8_t led_num)
{
    if (led_num < MAX_LEDS) {
        led_states[led_num] = false;
    }
}

bool led_control_is_on(uint8_t led_num)
{
    if (led_num >= MAX_LEDS) {
        return false;
    }
    return led_states[led_num];
}

uint8_t led_control_get_max_leds(void)
{
    return MAX_LEDS;
}
