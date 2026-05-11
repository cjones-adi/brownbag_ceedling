/******************************************************************************
 * @file    i2c_hal.c
 * @brief   I2C HAL – firmware implementation (MAX32655 MXC I2C driver).
 *
 *          This file is compiled only in the firmware build.  In the Ceedling
 *          host-test build (TEST defined), CMock generates mock_i2c_hal.c
 *          which replaces these functions with controllable stubs.
 ******************************************************************************/

#include "i2c_hal.h"

#ifndef TEST  /* ---- Firmware build ---------------------------------------- */

#include "mxc_errors.h"
#include "i2c.h"

#define I2C_INSTANCE  MXC_I2C1
#define I2C_FREQ      100000u

static mxc_i2c_req_t req;

int i2c_hal_write_read(uint8_t dev_addr,
                        const uint8_t *tx, uint8_t tx_len,
                        uint8_t *rx,       uint8_t rx_len)
{
    req.i2c     = I2C_INSTANCE;
    req.addr    = dev_addr;
    req.tx_buf  = (uint8_t *)tx;
    req.tx_len  = tx_len;
    req.rx_buf  = rx;
    req.rx_len  = rx_len;
    req.restart = 0;

    int err = MXC_I2C_MasterTransaction(&req);

    if (err == E_TIME_OUT)  return I2C_TIMEOUT;
    if (err == E_COMM_ERR)  return I2C_BUS_HUNG;
    if (err != E_NO_ERROR)  return err;
    return I2C_SUCCESS;
}

int i2c_hal_reset(void)
{
    /* Toggle SCL to un-jam a locked SDA, then re-initialise the peripheral. */
    MXC_I2C_Init(I2C_INSTANCE, 1, 0);
    MXC_I2C_SetFrequency(I2C_INSTANCE, I2C_FREQ);
    return I2C_SUCCESS;
}

#endif /* TEST */
