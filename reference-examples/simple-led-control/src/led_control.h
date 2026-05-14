/******************************************************************************
 * @file    led_control.h
 * @brief   LED control interface — pure software state, no hardware calls.
 *
 *          Demo 1 "Simple Start" reference project.
 *
 *          led_control tracks the on/off state of up to MAX_LEDS channels in
 *          a software array.  There are NO GPIO calls, NO HAL dependencies,
 *          and NO board-specific types.  The implementation compiles and runs
 *          entirely on the host — that is the point of Demo 1.
 *
 *          Hardware interaction (gpio_hal, CMock) is introduced in Demo 2.
 ******************************************************************************/
#ifndef LED_CONTROL_H
#define LED_CONTROL_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Initialise LED controller — sets all channels to OFF.
 */
void led_control_init(void);

/**
 * @brief Turn LED on by index (0-based).
 *        Does nothing if led_num is out of range.
 */
void led_control_turn_on(uint8_t led_num);

/**
 * @brief Turn LED off by index (0-based).
 *        Does nothing if led_num is out of range.
 */
void led_control_turn_off(uint8_t led_num);

/**
 * @brief Query whether an LED is on.
 * @return true if on, false if off or out of range.
 */
bool led_control_is_on(uint8_t led_num);

/**
 * @brief Return the total number of supported LED channels.
 * @return Total number of LED channels.
 */
uint8_t led_control_get_max_leds(void);

#endif /* LED_CONTROL_H */

