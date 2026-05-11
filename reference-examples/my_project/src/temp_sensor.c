/******************************************************************************
 * @file    temp_sensor.c
 * @brief   Temperature sensor implementation.
 *
 *          Wraps the MAX31889 driver (from MaximSDK MiscDrivers) and the
 *          MXC I2C driver to provide a simple read interface.
 ******************************************************************************/

#include "temp_sensor.h"

/* Hardware drivers are only needed in the firmware build.
 * In the Ceedling host-test build (TEST is defined by project.yml),
 * the hardware-dependent functions are replaced with lightweight stubs
 * so the pure-math convert_raw function can be tested natively. */
#ifndef TEST
#include "mxc_errors.h"
#include "i2c.h"
#include "max31889_driver.h"

#define I2C_MASTER  MXC_I2C1
#define I2C_FREQ    100000

static max31889_driver_t sensor;
static int initialized = 0;

int temp_sensor_init(void)
{
    int err = MXC_I2C_Init(I2C_MASTER, 1, 0);
    if (err != E_NO_ERROR) {
        return err;
    }

    MXC_I2C_SetFrequency(I2C_MASTER, I2C_FREQ);

    sensor = MAX31889_Open();
    sensor.init(I2C_MASTER, MAX31889_I2C_SLAVE_ADDR0);

    initialized = 1;
    return E_NO_ERROR;
}

int temp_sensor_read(int32_t *temp_out)
{
    if (!initialized || temp_out == (void *)0) {
        return -1;
    }

    /* The MAX31889 SDK driver returns a float.  Convert immediately to
     * integer millidegrees so all caller-visible values stay float-free. */
    float ftmp = 0.0f;
    int err = sensor.read(&ftmp);
    if (err != 0) {
        return err;
    }
    *temp_out = (int32_t)(ftmp * 1000);
    return 0;
}

#else  /* TEST build – use I2C HAL so tests can mock hardware interactions */

#include "i2c_hal.h"

/* MAX31889 device address (ADDR pin low: 0xA0 >> 1 = 0x50) */
#define MAX31889_ADDR       (0xA0u >> 1u)
/* FIFO data register – holds the 2-byte temperature reading */
#define MAX31889_FIFO_DATA_REG  0x08u

int temp_sensor_init(void) { return 0; }

int temp_sensor_read(int32_t *temp_out)
{
    if (!temp_out)
        return -1;

    /* Write the FIFO data register address, then read back 2 temperature bytes.
     * This mirrors what max318889_read_temperature() does inside the SDK driver
     * but uses the mockable HAL so tests can inject any response. */
    uint8_t reg    = MAX31889_FIFO_DATA_REG;
    uint8_t rx[2]  = { 0u, 0u };

    int err = i2c_hal_write_read(MAX31889_ADDR, &reg, 1u, rx, 2u);

    if (err == I2C_BUS_HUNG) {
        i2c_hal_reset();   /* Attempt bus recovery before surfacing the error */
        return err;
    }
    if (err != I2C_SUCCESS) {
        return err;
    }

    /* MAX31889 conversion: temp_celsius = raw_uint16 * 0.005
     * Integer equivalent (no float):  temp_millideg = raw * 5
     * raw = (rx[0] << 8) | rx[1]   (big-endian, MSB first)              */
    uint16_t raw   = ((uint16_t)rx[0] << 8u) | (uint16_t)rx[1];
    *temp_out      = (int32_t)raw * 5;

    return 0;
}

#endif /* TEST */

/* ============================================================================
 * TEMPERATURE OVERFLOW DEMO  –  see temp_sensor.h for toggle instructions
 * ============================================================================
 * The formula and (uint16_t) cast below replicate the bug found in
 * lt8460_get_temp_channel_temp() / lt8460_iio_read_temp_channel_temp().
 *
 * Overflow look-up table (C integer arithmetic, MILLI = 1000):
 *
 *   raw  |  int32_t temp (millideg) |  BUG int16_t stored  |  Error
 *   -----|--------------------------|----------------------|-------------------
 *     0  |  -290183  (-290.2 °C)   |  -28040  (-28.0 °C)  |  +262 °C wrong!
 *   128  |    25184  (  25.2 °C)   |   25184  ( 25.2 °C)  |  no overflow
 *   200  |   202579  ( 202.6 °C)   |    5971  (  6.0 °C)  |  -196 °C wrong!
 *
 * The IIO read path (mirrored by the unit test) then splits the stored value:
 *   vals[0] = temp_millideg / 1000          (integer degrees C)
 *   vals[1] = (temp_millideg % 1000) * 1000 (micro-degrees)
 * and formats it for display.  With the bug, a 202 °C battery appears as
 * 5 °C, so the >40 °C safety threshold is never crossed — a silent failure.
 * ========================================================================== */

#define MILLI 1000

#ifdef TEMP_CONVERT_BUG_ENABLED

/* ---- BUG version (active while TEMP_CONVERT_BUG_ENABLED is defined) ------ */
int temp_sensor_convert_raw(uint8_t raw, int16_t *value_millideg)
{
    if (!value_millideg)
        return -1;

    /* Replicates lt8460_get_temp_channel_temp() */
    int32_t temp = ((int32_t)raw * 1200000 / 255 - 554250) * MILLI / 1910;

    /* BUG: (uint16_t) cast discards bits [31:16] of the int32_t result.
     *      When |temp| > 32767 the stored value is wrong.
     *      For raw=200: temp=202579 → (uint16_t)202579=5971 → stored 5971.
     *      Mirrors the original:  *value = (uint16_t)1temp; */
    *value_millideg = (int16_t)(uint16_t)temp;  /* <-- BUG line */

    return 0;
}

#else  /* TEMP_CONVERT_BUG_ENABLED not defined  →  FIX active */

/* ---- FIX version (active when TEMP_CONVERT_BUG_ENABLED is commented out) - */
int temp_sensor_convert_raw(uint8_t raw, int32_t *value_millideg)
{
    if (!value_millideg)
        return -1;

    /* FIX: store into int32_t directly – full range, no truncation. */
    *value_millideg = ((int32_t)raw * 1200000 / 255 - 554250) * MILLI / 1910;

    return 0;
}

#endif /* TEMP_CONVERT_BUG_ENABLED */
