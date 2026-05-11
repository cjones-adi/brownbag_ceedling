// CEEDLING NOTICE: This generated file only to be consumed for test runner creation

#include "build/vendor/unity/src/unity.h"
#include "src/temp_sensor.h"
#include "mock_i2c_hal.h"

void setUp(void)
{
    mock_i2c_hal_Init();
}

void tearDown(void)
{
    mock_i2c_hal_Verify();
    mock_i2c_hal_Destroy();
}
void test_convert_raw_normal_temp_no_overflow(void)
{
    int16_t result = 0;
    int ret = temp_sensor_convert_raw(128, &result);
    UnityAssertEqualNumber((UNITY_INT)((0)), (UNITY_INT)((ret)), (
   ((void *)0)
   ), (UNITY_UINT)(70), UNITY_DISPLAY_STYLE_INT);
    UnityAssertEqualNumber((UNITY_INT)(UNITY_INT16)((25184)), (UNITY_INT)(UNITY_INT16)((result)), (
   ((void *)0)
   ), (UNITY_UINT)(71), UNITY_DISPLAY_STYLE_INT16);
}

void test_convert_raw_demonstrates_overflow_value(void)
{
    int16_t result = 0;
    int ret = temp_sensor_convert_raw(200, &result);
    UnityAssertEqualNumber((UNITY_INT)((0)), (UNITY_INT)((ret)), (
   ((void *)0)
   ), (UNITY_UINT)(81), UNITY_DISPLAY_STYLE_INT);
    UnityAssertEqualNumber((UNITY_INT)(UNITY_INT16)((5971)), (UNITY_INT)(UNITY_INT16)((result)), (
   ((void *)0)
   ), (UNITY_UINT)(82), UNITY_DISPLAY_STYLE_INT16);
}

void test_convert_raw_overflow_safety_check_fails_to_trigger(void)
{
    int16_t stored_millideg = 0;
    temp_sensor_convert_raw(200, &stored_millideg);

    int vals[2];
    vals[0] = (int32_t)stored_millideg / 1000;
    vals[1] = (int32_t)(stored_millideg - vals[0] * 1000) * 1000;

    UnityAssertEqualNumber((UNITY_INT)((5)), (UNITY_INT)((vals[0])), (
   ((void *)0)
   ), (UNITY_UINT)(95), UNITY_DISPLAY_STYLE_INT);
    UnityAssertGreaterOrLessOrEqualNumber((UNITY_INT)((40)), (UNITY_INT)((vals[0])), UNITY_SMALLER_THAN, (
   ((void *)0)
   ), (UNITY_UINT)(96), UNITY_DISPLAY_STYLE_INT);
}

void test_convert_raw_null_pointer_returns_error(void)
{
    int ret = temp_sensor_convert_raw(128, (int16_t *)0);
    UnityAssertEqualNumber((UNITY_INT)((-1)), (UNITY_INT)((ret)), (
   ((void *)0)
   ), (UNITY_UINT)(103), UNITY_DISPLAY_STYLE_INT);
}
static int fake_i2c_read_25C(uint8_t dev_addr,
                               const uint8_t *tx, uint8_t tx_len,
                               uint8_t *rx, uint8_t rx_len,
                               int num_calls)
{
    (void)dev_addr; (void)tx; (void)tx_len; (void)rx_len; (void)num_calls;
    rx[0] = 0x13u;
    rx[1] = 0x88u;
    return 0;
}

static int fake_i2c_timeout(uint8_t dev_addr,
                              const uint8_t *tx, uint8_t tx_len,
                              uint8_t *rx, uint8_t rx_len,
                              int num_calls)
{
    (void)dev_addr; (void)tx; (void)tx_len; (void)rx; (void)rx_len;
    (void)num_calls;
    return -1;
}
void test_temp_sensor_read_parses_positive_temperature_correctly(void)
{
    i2c_hal_write_read_Stub(fake_i2c_read_25C);

    int32_t temp = 0;
    int ret = temp_sensor_read(&temp);

    UnityAssertEqualNumber((UNITY_INT)((0)), (UNITY_INT)((ret)), (
   ((void *)0)
   ), (UNITY_UINT)(195), UNITY_DISPLAY_STYLE_INT);
    UnityAssertEqualNumber((UNITY_INT)(UNITY_INT32)((25000)), (UNITY_INT)(UNITY_INT32)((temp)), (
   ((void *)0)
   ), (UNITY_UINT)(196), UNITY_DISPLAY_STYLE_INT32);
}
void test_temp_sensor_read_handles_i2c_timeout_safely(void)
{
    i2c_hal_write_read_Stub(fake_i2c_timeout);

    int32_t temp = 99999;
    int ret = temp_sensor_read(&temp);

    UnityAssertEqualNumber((UNITY_INT)((-1)), (UNITY_INT)((ret)), (
   ((void *)0)
   ), (UNITY_UINT)(218), UNITY_DISPLAY_STYLE_INT);
    UnityAssertEqualNumber((UNITY_INT)(UNITY_INT32)((99999)), (UNITY_INT)(UNITY_INT32)((temp)), (
   ((void *)0)
   ), (UNITY_UINT)(219), UNITY_DISPLAY_STYLE_INT32);
}

static int fake_i2c_bus_hung(uint8_t dev_addr,
                               const uint8_t *tx, uint8_t tx_len,
                               uint8_t *rx, uint8_t rx_len,
                               int num_calls)
{
    (void)dev_addr; (void)tx; (void)tx_len; (void)rx; (void)rx_len;
    (void)num_calls;
    return -2;
}
void test_temp_sensor_read_calls_reset_on_bus_hung(void)
{
    i2c_hal_write_read_Stub(fake_i2c_bus_hung);

    i2c_hal_reset_CMockExpectAndReturn(251, 0);

    int32_t temp = 99999;
    int ret = temp_sensor_read(&temp);

    UnityAssertEqualNumber((UNITY_INT)((-2)), (UNITY_INT)((ret)), (
   ((void *)0)
   ), (UNITY_UINT)(256), UNITY_DISPLAY_STYLE_INT);
    UnityAssertEqualNumber((UNITY_INT)(UNITY_INT32)((99999)), (UNITY_INT)(UNITY_INT32)((temp)), (
   ((void *)0)
   ), (UNITY_UINT)(257), UNITY_DISPLAY_STYLE_INT32);
}