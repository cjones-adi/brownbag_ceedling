/******************************************************************************
 * @file    temp_sensor.h
 * @brief   Temperature sensor interface for the safety charging demo.
 *
 *          Abstracts the MAX31889 I2C temperature sensor so the business logic
 *          in main.c can be unit-tested independently of hardware.
 ******************************************************************************/

#ifndef TEMP_SENSOR_H
#define TEMP_SENSOR_H

#include <stdint.h>

/**
 * @brief  Initialize the I2C master and the MAX31889 temperature sensor.
 * @return 0 on success, negative on error.
 */
int temp_sensor_init(void);

/**
 * @brief  Read the current temperature in millidegrees Celsius.
 *
 *         The MAX31889 driver result (float °C) is converted internally to
 *         integer millidegrees so the public API remains float-free.
 *         Example: 25.5 °C is returned as 25500.
 *
 * @param[out] temp_out  Pointer to int32_t to receive millidegrees C.
 * @return 0 on success, negative on error.
 */
int temp_sensor_read(int32_t *temp_out);

/* ============================================================================
 * TEMPERATURE OVERFLOW DEMO
 * ============================================================================
 * Reproduces the lt8460_get_temp_channel_temp() / iio_lt8460 bug where a wide
 * int32_t intermediate value is silently truncated when stored through a
 * (uint16_t) cast into an int16_t output pointer.
 *
 * Formula (identical to lt8460_get_temp_channel_temp):
 *   temp = (read_temp * 1200000 / 255 - 554250) * MILLI / 1910   [int32_t]
 *
 * BUG:  *value = (int16_t)(uint16_t)temp
 *       -- takes only the lower 16 bits, silently discarding the upper bits.
 *       -- For |temp| > 32767 millideg (roughly outside –32 °C … +32 °C)
 *          the stored value is completely wrong.
 *
 * FIX:  function returns via int32_t* so the full range is preserved.
 *
 * HOW TO TOGGLE between bug and fix
 * ----------------------------------
 *   1. Comment / uncomment the #define below (single line change).
 *   2. Re-run:  ceedling test:all
 *   3. Observe which tests pass and which fail.
 * ========================================================================== */

/* vvv  Comment this line out to switch to the fixed version  vvv */
#define TEMP_CONVERT_BUG_ENABLED
/* ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

#ifdef TEMP_CONVERT_BUG_ENABLED
/**
 * @brief [BUG ACTIVE] Convert a raw sensor byte to millidegrees Celsius.
 *
 *        Mirrors lt8460_get_temp_channel_temp().  The (uint16_t) cast on
 *        the int32_t intermediate silently truncates temperatures outside
 *        roughly –32 °C … +32 °C, producing a dangerously wrong reading.
 *
 *        Example:  raw=200  →  correct 202,579 millideg (202.6 °C)
 *                              stored   5,971 millideg  (  5.97 °C)  ← BUG
 *
 * @param raw           Raw byte from the temperature sensor register (0-255).
 * @param value_millideg Pointer to int16_t to receive millidegrees C.
 * @return 0 on success, -1 if value_millideg is NULL.
 */
int temp_sensor_convert_raw(uint8_t raw, int16_t *value_millideg);

#else  /* TEMP_CONVERT_BUG_ENABLED not defined  →  FIX active */

/**
 * @brief [FIX ACTIVE] Convert a raw sensor byte to millidegrees Celsius.
 *
 *        Uses int32_t output so the full value range is preserved with no
 *        truncation.
 *
 *        Example:  raw=200  →  202,579 millideg (202.6 °C)  ← CORRECT
 *
 * @param raw           Raw byte from the temperature sensor register (0-255).
 * @param value_millideg Pointer to int32_t to receive millidegrees C.
 * @return 0 on success, -1 if value_millideg is NULL.
 */
int temp_sensor_convert_raw(uint8_t raw, int32_t *value_millideg);

#endif /* TEMP_CONVERT_BUG_ENABLED */

#endif /* TEMP_SENSOR_H */
