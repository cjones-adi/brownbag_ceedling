/******************************************************************************
 * @file    led_control.c
 * @brief   LED control implementation for unit testing example
 ******************************************************************************/

#include "led_control.h"

// Internal state tracking
static uint8_t led_state = 0;
static bool initialized = false;

int led_control_init(void)
{
    led_state = 0;
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
