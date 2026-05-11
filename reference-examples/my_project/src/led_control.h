/******************************************************************************
 * @file    led_control.h
 * @brief   LED control interface for the safety charging demo
 ******************************************************************************/

#ifndef LED_CONTROL_H
#define LED_CONTROL_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief LED color bit-mask definitions (matches MAX20303 PMIC register bits)
 */
typedef enum {
    LED_COLOR_BLUE  = 0x1,
    LED_COLOR_RED   = 0x2,
    LED_COLOR_GREEN = 0x4
} led_color_t;

/**
 * @brief Initialize LED control module
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

/**
 * @brief Get the raw combined LED state bitmask
 * @return bitmask of all active LED colors
 */
uint8_t led_control_get_state(void);

/* Convenience wrappers ---------------------------------------------------- */
static inline int red_led_on(void)   { return led_control_set(LED_COLOR_RED,   true);  }
static inline int red_led_off(void)  { return led_control_set(LED_COLOR_RED,   false); }
static inline int green_led_on(void) { return led_control_set(LED_COLOR_GREEN, true);  }
static inline int green_led_off(void){ return led_control_set(LED_COLOR_GREEN, false); }
static inline int blue_led_on(void)  { return led_control_set(LED_COLOR_BLUE,  true);  }
static inline int blue_led_off(void) { return led_control_set(LED_COLOR_BLUE,  false); }

#endif /* LED_CONTROL_H */
