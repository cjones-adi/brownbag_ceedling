/******************************************************************************
 *
 * Copyright (C) 2024 Analog Devices, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/

/**
 * @file    main.c
 * @brief   Safety Charging Demo
 * @details Combines ADC voltage monitoring, I2C temperature sensing, and
 *          RGB LED control on the MAX32655FTHR.
 *
 *          Demo logic:
 *            1. Red LED is turned on to indicate the system is running.
 *            2. Temperature (°C) is read from the MAX31889 via I2C.
 *            3. Voltage (V)  is read from AIN0 via the ADC.
 *            4. Safety check: if Temp > 40 °C AND Voltage >= 4.2 V,
 *               the Green LED turns on (simulating a "stop charging" signal).
 */

/***** Includes *****/
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "mxc_device.h"
#include "mxc_delay.h"
#include "mxc_errors.h"
#include "i2c.h"

/* Project modules */
#include "led_control.h"
#include "adc_monitor.h"
#include "temp_sensor.h"

/***** Definitions *****/
/* I2C instance shared between the LED PMIC and the temperature sensor.
 * The MAX20303 PMIC (LED) and MAX31889 (temp sensor) both live on I2C1. */
#define PMIC_I2C            MXC_I2C1
#define PMIC_SLAVE_ADDR     (0x28)
#define PMIC_I2C_FREQ       100000

/* MAX20303 PMIC LED register addresses */
#define LED_CFG_REG_ADDR    0x2C
#define LED_SET_REG_ADDR    0x2D

/* Safety thresholds – integer units match the sensor API:
 *   temperature : int32_t  millidegrees C  (40 °C  = 40000 mdeg)
 *   voltage     : uint32_t millivolts      (4.2 V  = 4200  mV  ) */
#define TEMP_THRESHOLD_MDEG  40000
#define VOLT_THRESHOLD_MV    4200

/***** Globals *****/
static uint8_t tx_buf[4];

/***** Functions *****/

/**
 * @brief Write the current led_control software state to the MAX20303 PMIC
 *        via I2C so the physical RGB LED reflects the logical state.
 */
static void apply_leds_to_hardware(void)
{
    uint8_t state = led_control_get_state();

    mxc_i2c_req_t req;
    req.i2c     = PMIC_I2C;
    req.addr    = PMIC_SLAVE_ADDR;
    req.tx_buf  = tx_buf;
    req.tx_len  = 4;
    req.rx_buf  = NULL;
    req.rx_len  = 0;
    req.restart = 0;

    tx_buf[0] = LED_SET_REG_ADDR;
    tx_buf[1] = (uint8_t)((state & LED_COLOR_BLUE)  ? (1 << 5) : 0); /* Blue  */
    tx_buf[2] = (uint8_t)((state & LED_COLOR_RED)   ? (1 << 4) : 0); /* Red   */
    tx_buf[3] = (uint8_t)((state & LED_COLOR_GREEN) ? (1 << 3) : 0); /* Green */

    int err = MXC_I2C_MasterTransaction(&req);
    if (err != 0) {
        printf("LED I2C write error: %d\n", err);
    }
}

/**
 * @brief Configure the MAX20303 PMIC LED current (1 mA) via I2C.
 */
static int pmic_led_init(void)
{
    int err;

    err = MXC_I2C_Init(PMIC_I2C, 1, 0);
    if (err != E_NO_ERROR) {
        printf("PMIC I2C init failed: %d\n", err);
        return err;
    }
    MXC_I2C_SetFrequency(PMIC_I2C, PMIC_I2C_FREQ);

    mxc_i2c_req_t req;
    req.i2c     = PMIC_I2C;
    req.addr    = PMIC_SLAVE_ADDR;
    req.tx_buf  = tx_buf;
    req.tx_len  = 2;
    req.rx_buf  = NULL;
    req.rx_len  = 0;
    req.restart = 0;

    tx_buf[0] = LED_CFG_REG_ADDR;
    tx_buf[1] = 1; /* 1 mA LED current */

    err = MXC_I2C_MasterTransaction(&req);
    if (err != 0) {
        printf("PMIC LED cfg error: %d\n", err);
    }
    return err;
}

// *****************************************************************************
int main(void)
{
    int      err;
    int32_t  temperature = 0;  /* millidegrees C  (e.g. 25 °C = 25000) */
    uint32_t voltage     = 0;  /* millivolts      (e.g. 4.2 V = 4200)  */

    MXC_Delay(MXC_DELAY_MSEC(500)); /* Wait for PMIC to power up */

    printf("\n******* Safety Charging Demo – MAX32655FTHR *******\n");
    printf("Combines: ADC voltage monitor + I2C temp sensor + RGB LED\n\n");

    /* ------------------------------------------------------------------ */
    /* Initialise peripherals                                              */
    /* ------------------------------------------------------------------ */

    /* LED software state tracker */
    led_control_init();

    /* PMIC (MAX20303) for RGB LED via I2C */
    err = pmic_led_init();
    if (err != E_NO_ERROR) {
        printf("PMIC init failed – halting.\n");
        while (1) {}
    }

    /* ADC for voltage monitoring (AIN0) */
    err = adc_monitor_init();
    if (err != E_NO_ERROR) {
        printf("ADC init failed: %d – halting.\n", err);
        while (1) {}
    }

    /* Temperature sensor (MAX31889) via I2C */
    err = temp_sensor_init();
    if (err != E_NO_ERROR) {
        printf("Temp sensor init failed: %d – halting.\n", err);
        while (1) {}
    }

    printf("Initialisation complete.\n\n");

    /* ------------------------------------------------------------------ */
    /* Main loop                                                           */
    /* ------------------------------------------------------------------ */
    while (1) {
        /* Reset LED state each iteration */
        led_control_init();

        /* Step 1: Red LED ON – system is active */
        red_led_on();

        /* Step 2: Read temperature */
        err = temp_sensor_read(&temperature);
        if (err != E_NO_ERROR) {
            printf("Temp read error: %d\n", err);
            temperature = 0;
        }

        /* Step 3: Read voltage */
        err = adc_monitor_read_voltage(&voltage);
        if (err != E_NO_ERROR) {
            printf("ADC read error: %d\n", err);
            voltage = 0;
        }

        /* Display in human-readable degrees / volts using integer arithmetic */
        int32_t  temp_c    = temperature / 1000;
        int32_t  temp_mdeg = (temperature >= 0) ? (temperature % 1000)
                                                 : -(temperature % 1000);
        uint32_t volt_v    = voltage / 1000;
        uint32_t volt_mv   = voltage % 1000;
        printf("Temp: %ld.%03ld C  |  Voltage: %lu.%03lu V\n",
               (long)temp_c, (long)temp_mdeg,
               (unsigned long)volt_v, (unsigned long)volt_mv);

        /* Step 4: Safety logic – Green LED ON if Temp > 40000 mdeg AND V >= 4200 mV */
        if (temperature > TEMP_THRESHOLD_MDEG && voltage >= VOLT_THRESHOLD_MV) {
            green_led_on(); /* stop_charging(); */
            printf("  >> SAFETY: Over-temp + Over-voltage! Charging stopped.\n");
        }

        /* Apply logical LED state to the physical PMIC hardware */
        apply_leds_to_hardware();

        MXC_Delay(MXC_DELAY_SEC(1));
    }

    return E_NO_ERROR;
}
