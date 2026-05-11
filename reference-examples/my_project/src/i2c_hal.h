/******************************************************************************
 * @file    i2c_hal.h
 * @brief   Thin I2C Hardware Abstraction Layer (HAL).
 *
 *          Wraps the MXC I2C driver into a minimal interface that:
 *           - Keeps hardware details out of higher-level modules.
 *           - Is fully mockable by CMock so unit tests can simulate any
 *             I2C response — success, timeout, or bus-hung — without
 *             touching real hardware.
 *
 *          CMock mock generation
 *          ---------------------
 *          When test_temp_sensor.c includes "mock_i2c_hal.h", Ceedling
 *          auto-generates mock_i2c_hal.h / mock_i2c_hal.c from this header.
 *          The generated mock provides:
 *            i2c_hal_write_read_StubWithCallback()  – inject any response
 *            i2c_hal_write_read_ExpectAndReturn()   – strict expect
 *            i2c_hal_reset_Expect()                 – verify reset called
 ******************************************************************************/

#ifndef I2C_HAL_H
#define I2C_HAL_H

#include <stdint.h>

/* ---- Return codes -------------------------------------------------------- */
#define I2C_SUCCESS    0   /* Transaction completed successfully              */
#define I2C_TIMEOUT   -1   /* Slave did not respond within the timeout window */
#define I2C_BUS_HUNG  -2   /* SDA/SCL stuck low – bus cannot be recovered
                            * by a normal transaction; call i2c_hal_reset()  */

/* ---- API ----------------------------------------------------------------- */

/**
 * @brief  Perform a combined I2C write followed by an I2C read.
 *
 *         Writes @p tx_len bytes from @p tx, then reads @p rx_len bytes into
 *         @p rx.  Both phases target @p dev_addr on the shared I2C bus.
 *
 * @param dev_addr  7-bit I2C slave address.
 * @param tx        Bytes to write (typically a register address).
 * @param tx_len    Number of bytes to write.
 * @param rx        Buffer to receive the read-back bytes.
 * @param rx_len    Number of bytes to read.
 * @return I2C_SUCCESS, I2C_TIMEOUT, or I2C_BUS_HUNG.
 */
int i2c_hal_write_read(uint8_t dev_addr,
                        const uint8_t *tx, uint8_t tx_len,
                        uint8_t *rx,       uint8_t rx_len);

/**
 * @brief  Reset the I2C bus after a bus-hung condition.
 *
 *         Toggles the SCL line to un-jam a locked SDA, then re-initialises
 *         the peripheral.  Must be called when i2c_hal_write_read() returns
 *         I2C_BUS_HUNG.
 *
 * @return I2C_SUCCESS if the bus was successfully recovered, negative otherwise.
 */
int i2c_hal_reset(void);

#endif /* I2C_HAL_H */
