/******************************************************************************
 * @file    led_control.c
 * @brief   LED control implementation – pure software state tracker.
 *
 *          This module tracks which LEDs are logically on/off without any
 *          hardware dependencies so it can be compiled and tested natively on
 *          the host with Ceedling.
 *
 *          In the firmware build, main.c reads led_control_get_state() and
 *          forwards the bitmask to the MAX20303 PMIC via I2C.
 ******************************************************************************/

#include "led_control.h"

/* Internal state ----------------------------------------------------------- */
static uint8_t  led_state   = 0;
static bool     initialized = false;

/* Public API --------------------------------------------------------------- */

int led_control_init(void)
{
    led_state   = 0;
    initialized = true;
    return 0;
}

int led_control_set(led_color_t color, bool state)
{
    if (!initialized) {
        return -1;
    }

    if (state) {
        led_state |= (uint8_t)color;
    } else {
        led_state &= ~(uint8_t)color;
    }

    return 0;
}

bool led_control_get(led_color_t color)
{
    if (!initialized) {
        return false;
    }

    return (led_state & (uint8_t)color) != 0;
}

int led_control_toggle(led_color_t color)
{
    if (!initialized) {
        return -1;
    }

    led_state ^= (uint8_t)color;
    return 0;
}

uint8_t led_control_get_state(void)
{
    return led_state;
}
