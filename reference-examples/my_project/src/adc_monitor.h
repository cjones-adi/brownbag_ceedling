/******************************************************************************
 * @file    adc_monitor.h
 * @brief   ADC voltage monitoring interface for the safety charging demo.
 *
 *          Abstracts the MAX32655 ADC peripheral so the business logic in
 *          main.c can be unit-tested independently of hardware.
 ******************************************************************************/

#ifndef ADC_MONITOR_H
#define ADC_MONITOR_H

#include <stdint.h>

/**
 * @brief  Initialize the ADC peripheral.
 * @return 0 on success, negative on error.
 */
int adc_monitor_init(void);

/**
 * @brief  Read the voltage on AIN0 and convert the raw ADC count to millivolts.
 *
 *         The MAX32655 ADC has a full-scale input of 1220 mV and a 10-bit
 *         resolution (0x000–0x3FF).  An external resistor divider scales a
 *         higher supply voltage (e.g. a 4200 mV Li-ion cell) into that range.
 *         Adjust CELL_FULL_SCALE_MV in adc_monitor.c to match your divider.
 *
 * @param[out] voltage_out  Pointer to store the converted voltage in millivolts.
 * @return 0 on success, negative on error.
 */
int adc_monitor_read_voltage(uint32_t *voltage_out);

#endif /* ADC_MONITOR_H */
