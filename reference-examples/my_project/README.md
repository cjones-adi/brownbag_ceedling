## Safety Charging Demo – MAX32655FTHR

### Description

This demo project combines three MAX32655 example projects to demonstrate a
battery safety charging monitor:

| Feature | Source Example | Peripheral |
|---------|---------------|------------|
| RGB LED control | `FTHR_I2C` | I2C → MAX20303 PMIC |
| Temperature monitoring | `I2C_Sensor` | I2C → MAX31889 sensor |
| Voltage monitoring | `ADC` | ADC AIN0 |

**Demo logic**

```c
red_led_on();   // System active indicator

// Safety logic: Green LED ON (stop charging) if Temp > 40°C AND Voltage >= 4.2V
if (temperature > 40 && voltage >= 4.2) {
    green_led_on(); // stop_charging();
}
```

---

### Hardware

* **Board:** MAX32655FTHR (`FTHR_Apps_P1`)
* **Temperature sensor:** MAX31889 connected on I2C1
* **RGB LED:** MAX20303 PMIC on-board (I2C1, address 0x28)
* **Voltage input:** 0–4.2 V signal scaled via resistor divider to AIN0 (0–1.22 V)

**Required connections**
- USB cable between PC and J4 (USB/PWR)
- External voltage source (0–4.2 V) through resistor divider to AIN0 on header J9
- MAX31889 temperature sensor on I2C1 (SCL=P0.10, SDA=P0.11)
- Terminal application at 115200 8-N-1 on the console UART

---

### Software

#### Firmware Build (MaximSDK)

Universal instructions on building, flashing, and debugging this project can be
found in the **[MSDK User Guide](https://analogdevicesinc.github.io/msdk/USERGUIDE/)**.

```
make -r TARGET=MAX32655 BOARD=FTHR_Apps_P1 MAXIM_PATH=C:/MaximSDK
```

#### Unit Tests (Ceedling)

Install Ceedling (requires Ruby):

```
gem install ceedling
```

Run all tests:

```
ceedling test:all
```

Run tests with coverage:

```
ceedling gcov:all utils:gcov
```

---

### Project Structure

```
my_project/
├── main.c                  # Combined demo application
├── Makefile                # MaximSDK standalone build system
├── project.mk              # Board + driver configuration
├── project.yml             # Ceedling test configuration
├── src/
│   ├── led_control.h/.c    # LED state abstraction (unit-testable)
│   ├── adc_monitor.h/.c    # ADC voltage monitoring abstraction
│   └── temp_sensor.h/.c    # MAX31889 temperature sensor abstraction
└── test/
    ├── test_led_control.c  # Unit tests (LED control + safety logic)
    └── support/
```

---

### Expected Console Output

```
******* Safety Charging Demo – MAX32655FTHR *******
Combines: ADC voltage monitor + I2C temp sensor + RGB LED

Initialisation complete.

Temp: 25.00 C  |  Voltage: 3.850 V
Temp: 25.10 C  |  Voltage: 3.851 V
...
Temp: 41.20 C  |  Voltage: 4.210 V
  >> SAFETY: Over-temp + Over-voltage! Charging stopped.
```
