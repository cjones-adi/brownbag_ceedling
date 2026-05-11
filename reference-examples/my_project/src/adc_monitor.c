/******************************************************************************
 * @file    adc_monitor.c
 * @brief   ADC voltage monitoring implementation.
 *
 *          Uses the MAX32655 ADC peripheral (MXC ADC driver) to read AIN0
 *          and scales the 10-bit raw value to Volts using an external voltage
 *          divider ratio.
 ******************************************************************************/

#include "adc_monitor.h"
#include "mxc_errors.h"
#include "adc.h"

/* Adjust CELL_FULL_SCALE_MV to match your hardware resistor-divider ratio.
 * The MAX32655 ADC full-scale input is 1220 mV (10-bit, 0x000–0x3FF).
 * Example: a 4200 mV Li-ion cell is divided down to fit 0–1220 mV at AIN0.
 *   voltage_mV = raw_count * CELL_FULL_SCALE_MV / ADC_MAX_COUNT
 *              = raw_count * 4200            / 1023
 */
#define CELL_FULL_SCALE_MV  (4200u)   /* mV – maximum cell voltage          */
#define ADC_MAX_COUNT       (1023u)   /* 10-bit ADC full-scale count        */

static int initialized = 0;

int adc_monitor_init(void)
{
    int err = MXC_ADC_Init();
    if (err != E_NO_ERROR) {
        return err;
    }

    /* Set trip-point monitor on AIN0 (optional – mirrors the ADC example) */
    MXC_ADC_SetMonitorChannel(MXC_ADC_MONITOR_0, MXC_ADC_CH_0);
    MXC_ADC_SetMonitorHighThreshold(MXC_ADC_MONITOR_0, 0x300);
    MXC_ADC_SetMonitorLowThreshold(MXC_ADC_MONITOR_0, 0x25);
    MXC_ADC_EnableMonitor(MXC_ADC_MONITOR_0);

    initialized = 1;
    return E_NO_ERROR;
}

int adc_monitor_read_voltage(uint32_t *voltage_out)
{
    if (!initialized || voltage_out == (void *)0) {
        return E_BAD_PARAM;
    }

    int32_t raw = MXC_ADC_StartConversion(MXC_ADC_CH_0);
    if (raw == E_OVERFLOW) {
        return E_OVERFLOW;
    }

    /* Integer-only conversion: scale raw count to millivolts.
     * raw is always 0–1023 for a valid reading, so the product fits
     * comfortably in uint32_t before the division. */
    *voltage_out = (uint32_t)raw * CELL_FULL_SCALE_MV / ADC_MAX_COUNT;

    return E_NO_ERROR;
}
