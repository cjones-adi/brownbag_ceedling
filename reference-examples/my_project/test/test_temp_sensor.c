/******************************************************************************
 * @file    test_temp_sensor.c
 * @brief   Unit tests for the temperature sensor module.
 *
 * Three independent test scenarios are demonstrated:
 *
 * SCENARIO 1 -- Overflow bug demo  (temp_sensor_convert_raw)
 * -----------------------------------------------------------
 *   The lt8460-style (uint16_t) cast bug.  Toggle TEMP_CONVERT_BUG_ENABLED
 *   in temp_sensor.h to switch between the bug and fix test sets.
 *
 * SCENARIO 2 -- Simulated hardware, no board required  (temp_sensor_read)
 * -------------------------------------------------------------------------
 *   CMock replaces the real I2C HAL with a controllable stub so we can:
 *     a) Prove the driver parses a positive temperature correctly.
 *     b) Prove the driver returns the correct error code on I2C timeout.
 *
 * SCENARIO 3 -- Simulated hardware failure: I2C bus hung
 * -------------------------------------------------------
 *   Force i2c_hal_write_read() to return I2C_BUS_HUNG and verify that
 *   i2c_hal_reset() is called exactly once as recovery -- without ever
 *   physically shorting the SDA/SCL pins on the board.
 *
 * HOW TO TOGGLE the overflow bug (Scenario 1 only)
 * -------------------------------------------------
 *   Comment / uncomment  #define TEMP_CONVERT_BUG_ENABLED  in temp_sensor.h
 *   then re-run:  ceedling test:all
 *
 * MAX31889 temperature encoding (Scenarios 2 and 3 test vectors)
 * --------------------------------------------------------------
 *   temp_celsius  = raw_uint16 * 0.005
 *   temp_millideg = raw_uint16 * 5         (integer -- no float)
 *   raw           = (rx[0] << 8) | rx[1]  (big-endian, MSB first)
 *
 *   25 C  ->  raw = 5000 = 0x1388  ->  rx = {0x13,0x88}  ->  25000 mdeg
 *   41 C  ->  raw = 8200 = 0x2008  ->  rx = {0x20,0x08}  ->  41000 mdeg
 ******************************************************************************/

#include "unity.h"
#include "temp_sensor.h"
#include "mock_i2c_hal.h"   /* CMock-generated mock for src/i2c_hal.h */

/* ---- setUp / tearDown ---------------------------------------------------- */

void setUp(void)
{
    /* Initialise CMock tracking for every test.
     * Scenario 1 tests do not set expectations so Verify() passes cleanly. */
    mock_i2c_hal_Init();
}

void tearDown(void)
{
    /* Verify all CMock expectations were satisfied, then free resources. */
    mock_i2c_hal_Verify();
    mock_i2c_hal_Destroy();
}

/* ==========================================================================
 * SCENARIO 1 -- Overflow bug demo  (temp_sensor_convert_raw)
 * Does NOT call any I2C HAL functions.
 * ========================================================================== */
#ifdef TEMP_CONVERT_BUG_ENABLED

/* Normal range: raw=128 -> 25184 mdeg (25.2 C). No overflow; bug and fix agree. */
void test_convert_raw_normal_temp_no_overflow(void)
{
    int16_t result = 0;
    int ret = temp_sensor_convert_raw(128, &result);
    TEST_ASSERT_EQUAL_INT(0, ret);
    TEST_ASSERT_EQUAL_INT16(25184, result);
}

/* raw=200 -> int32_t 202579 mdeg (~202 C).
 * BUG: (uint16_t) cast keeps only lower 16 bits -> 5971 (~6 C). DANGEROUS.
 * PASSES with bug; will not compile with the fix (wrong pointer type). */
void test_convert_raw_demonstrates_overflow_value(void)
{
    int16_t result = 0;
    int ret = temp_sensor_convert_raw(200, &result);
    TEST_ASSERT_EQUAL_INT(0, ret);
    TEST_ASSERT_EQUAL_INT16(5971, result);   /* overflow value -- bug expected */
}

/* Safety consequence: 202 C reads as 5 C so the threshold is never crossed. */
void test_convert_raw_overflow_safety_check_fails_to_trigger(void)
{
    int16_t stored_millideg = 0;
    temp_sensor_convert_raw(200, &stored_millideg);

    int vals[2];
    vals[0] = (int32_t)stored_millideg / 1000;
    vals[1] = (int32_t)(stored_millideg - vals[0] * 1000) * 1000;

    TEST_ASSERT_EQUAL_INT(5, vals[0]);       /* BUG: reads 5 C not 202       */
    TEST_ASSERT_LESS_THAN_INT(40, vals[0]);  /* safety shutoff does NOT fire */
}

/* Null pointer guard */
void test_convert_raw_null_pointer_returns_error(void)
{
    int ret = temp_sensor_convert_raw(128, (int16_t *)0);
    TEST_ASSERT_EQUAL_INT(-1, ret);
}

#else  /* FIX active */

void test_convert_raw_normal_temp_no_overflow(void)
{
    int32_t result = 0;
    int ret = temp_sensor_convert_raw(128, &result);
    TEST_ASSERT_EQUAL_INT(0, ret);
    TEST_ASSERT_EQUAL_INT32(25184, result);
}

void test_convert_raw_full_range_preserved_by_fix(void)
{
    int32_t result = 0;
    int ret = temp_sensor_convert_raw(200, &result);
    TEST_ASSERT_EQUAL_INT(0, ret);
    TEST_ASSERT_EQUAL_INT32(202579, result);   /* PASSES with fix */
}

void test_convert_raw_fix_correctly_triggers_safety_threshold(void)
{
    int32_t stored_millideg = 0;
    temp_sensor_convert_raw(200, &stored_millideg);

    int vals[2];
    vals[0] = stored_millideg / 1000;
    vals[1] = (int32_t)(stored_millideg - vals[0] * 1000) * 1000;

    TEST_ASSERT_EQUAL_INT(202, vals[0]);
    TEST_ASSERT_GREATER_OR_EQUAL_INT(40, vals[0]);
}

void test_convert_raw_extreme_cold_preserved(void)
{
    int32_t result = 0;
    temp_sensor_convert_raw(0, &result);
    TEST_ASSERT_EQUAL_INT32(-290183, result);
}

void test_convert_raw_null_pointer_returns_error(void)
{
    int ret = temp_sensor_convert_raw(128, (int32_t *)0);
    TEST_ASSERT_EQUAL_INT(-1, ret);
}

#endif /* TEMP_CONVERT_BUG_ENABLED */

/* ==========================================================================
 * SCENARIO 2 -- Simulated hardware: positive parse + I2C timeout
 * CMock drives i2c_hal_write_read() via a callback.
 * ========================================================================== */

/* Simulates a successful read returning 25 C.
 * raw = 5000 = 0x1388 -> rx[0]=0x13, rx[1]=0x88 -> 5000*5=25000 mdeg */
static int fake_i2c_read_25C(uint8_t dev_addr,
                               const uint8_t *tx,  uint8_t tx_len,
                               uint8_t       *rx,  uint8_t rx_len,
                               int            num_calls)
{
    (void)dev_addr; (void)tx; (void)tx_len; (void)rx_len; (void)num_calls;
    rx[0] = 0x13u;  /* MSB */
    rx[1] = 0x88u;  /* LSB */
    return I2C_SUCCESS;
}

/* Simulates a slave that never ACKs (e.g. unplugged sensor cable). */
static int fake_i2c_timeout(uint8_t dev_addr,
                              const uint8_t *tx,  uint8_t tx_len,
                              uint8_t       *rx,  uint8_t rx_len,
                              int            num_calls)
{
    (void)dev_addr; (void)tx; (void)tx_len; (void)rx; (void)rx_len;
    (void)num_calls;
    return I2C_TIMEOUT;
}

/**
 * @brief Scenario 2a -- Driver parses a positive temperature correctly.
 *
 *        CMock injects the 25 C byte pattern {0x13, 0x88}.
 *        raw = 5000  ->  5000 * 5 = 25000 millideg.
 *        No hardware required.
 */
void test_temp_sensor_read_parses_positive_temperature_correctly(void)
{
    i2c_hal_write_read_StubWithCallback(fake_i2c_read_25C);

    int32_t temp = 0;
    int ret = temp_sensor_read(&temp);

    TEST_ASSERT_EQUAL_INT(0, ret);
    TEST_ASSERT_EQUAL_INT32(25000, temp);
}

/**
 * @brief Scenario 2b -- Driver handles I2C timeout safely.
 *
 *        We simulate a disconnected sensor (no ACK from slave).
 *        Verified:
 *          (a) I2C_TIMEOUT is returned to the caller.
 *          (b) temp_out is NOT modified (sentinel stays 99999).
 *          (c) i2c_hal_reset() is NOT called -- timeout != bus-hung.
 *              Any unexpected reset() call FAILS the test because
 *              fail_on_unexpected_calls is TRUE in project.yml.
 */
void test_temp_sensor_read_handles_i2c_timeout_safely(void)
{
    i2c_hal_write_read_StubWithCallback(fake_i2c_timeout);
    /* No i2c_hal_reset_Expect() -- accidental call will fail the test. */

    int32_t temp = 99999;   /* sentinel */
    int ret = temp_sensor_read(&temp);

    TEST_ASSERT_EQUAL_INT(I2C_TIMEOUT, ret);
    TEST_ASSERT_EQUAL_INT32(99999, temp);   /* output must be untouched */
}

/* ==========================================================================
 * SCENARIO 3 -- Simulated hardware failure: I2C bus hung + recovery
 * ========================================================================== */

/* Simulates SDA stuck low: no transaction can complete. */
static int fake_i2c_bus_hung(uint8_t dev_addr,
                               const uint8_t *tx,  uint8_t tx_len,
                               uint8_t       *rx,  uint8_t rx_len,
                               int            num_calls)
{
    (void)dev_addr; (void)tx; (void)tx_len; (void)rx; (void)rx_len;
    (void)num_calls;
    return I2C_BUS_HUNG;
}

/**
 * @brief Scenario 3 -- Recovery logic fires exactly once on I2C_BUS_HUNG.
 *
 *        ExpectAndReturn() records a strict expectation for i2c_hal_reset().
 *        tearDown() calls mock_i2c_hal_Verify() which fails if reset() was
 *        skipped or called more than once.
 *        The error code must propagate back to the caller.
 *        No physical SDA/SCL short-circuit is needed.
 */
void test_temp_sensor_read_calls_reset_on_bus_hung(void)
{
    i2c_hal_write_read_StubWithCallback(fake_i2c_bus_hung);

    /* Strict: i2c_hal_reset() must be called exactly once. */
    i2c_hal_reset_ExpectAndReturn(I2C_SUCCESS);

    int32_t temp = 99999;   /* sentinel */
    int ret = temp_sensor_read(&temp);

    TEST_ASSERT_EQUAL_INT(I2C_BUS_HUNG, ret);
    TEST_ASSERT_EQUAL_INT32(99999, temp);   /* output must be untouched */
}

