/******************************************************************************
 * @file    led_control.h
 * @brief   LED control interface for unit testing example
 ******************************************************************************/

#ifndef LED_CONTROL_H
#define LED_CONTROL_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief LED color definitions
 */
typedef enum {
    LED_COLOR_BLUE = 0x1,
    LED_COLOR_RED = 0x2,
    LED_COLOR_BOTH = 0x3
} led_color_t;

/**
 * @brief Initialize LED control
 * @return 0 on success, -1 on failure
 */
int led_control_init(void);

/**
 * @brief Set LED state
 * @param color LED color to control
 * @param state true to turn on, false to turn off
 * @return 0 on success, -1 on failure
 */
int led_control_set(led_color_t color, bool state);

/**
 * @brief Get LED state
 * @param color LED color to query
 * @return true if on, false if off
 */
bool led_control_get(led_color_t color);

/**
 * @brief Toggle LED state
 * @param color LED color to toggle
 * @return 0 on success, -1 on failure
 */
int led_control_toggle(led_color_t color);

#endif /* LED_CONTROL_H */
