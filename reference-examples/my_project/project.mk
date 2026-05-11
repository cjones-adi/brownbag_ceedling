###############################################################################
 #
 # Copyright (C) 2024 Analog Devices, Inc.
 #
 # Licensed under the Apache License, Version 2.0 (the "License");
 # you may not use this file except in compliance with the License.
 # You may obtain a copy of the License at
 #
 #     http://www.apache.org/licenses/LICENSE-2.0
 #
 # Unless required by applicable law or agreed to in writing, software
 # distributed under the License is distributed on an "AS IS" BASIS,
 # WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 # See the License for the specific language governing permissions and
 # limitations under the License.
 #
 ##############################################################################
# Safety Charging Demo – project.mk
# Targets the MAX32655FTHR (FTHR_Apps_P1).
#
# Combines:
#   - ADC voltage monitoring   (MAX32655 ADC peripheral)
#   - I2C temperature sensing  (MAX31889 sensor driver)
#   - RGB LED control via I2C  (MAX20303 PMIC on FTHR)

# Force FTHR board (overrides any command-line BOARD value)
override BOARD := FTHR_Apps_P1

ifneq ($(BOARD),FTHR_Apps_P1)
$(error ERR_NOTSUPPORTED: This project requires the MAX32655FTHR (FTHR_Apps_P1). \
        See https://analogdevicesinc.github.io/msdk/USERGUIDE/#board-support-packages)
endif

# --------------------------------------------------------------------------
# MAX31889 temperature sensor driver (from MaximSDK MiscDrivers)
# --------------------------------------------------------------------------
SRCS  += max31889_driver.c
VPATH += $(LIBS_DIR)/MiscDrivers/TempSensor
IPATH += $(LIBS_DIR)/MiscDrivers/TempSensor
