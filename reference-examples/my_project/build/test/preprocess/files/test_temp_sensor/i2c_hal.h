// CEEDLING NOTICE: This generated file only to be consumed by CMock

#ifndef _I2C_HAL_H_ // Ceedling-generated include guard
#define _I2C_HAL_H_

int i2c_hal_write_read(uint8_t dev_addr,
                        const uint8_t *tx, uint8_t tx_len,
                        uint8_t *rx, uint8_t rx_len);
int i2c_hal_reset(void);

#endif // _I2C_HAL_H_
