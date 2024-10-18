/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#include "Arduino.h"
#include "Wire.h"
#include "m5_unit_timerpwr.hpp"

M5UnitTimerPWR timer_pwr;

void setup()
{
    Serial.begin(115200);
    timer_pwr.begin(&Wire, TIMERPWR_ADDR, 32, 33, 400000U);
    Serial.printf("firmware version:%d, bootloader version:%d\n", timer_pwr.get_firmware_version(),
                  timer_pwr.get_bootloader_version());
    Serial.printf("i2c addr:0x%X\n", timer_pwr.get_i2c_address());
    delay(1000);
}

void loop()
{
    uint8_t charge_status = timer_pwr.get_battery_charge_status();
    if (charge_status)
        Serial.println("charging");
    else
        Serial.println("no charging");
    Serial.printf("usb voltage:%.2fV, usb current:%.2fmA\n", (float)timer_pwr.get_usb_voltage() / 100.0f,
                  (float)timer_pwr.get_usb_current() / 100.0f);
    Serial.printf("battery voltage:%.2fV, battery current:%.2fmA\n", (float)timer_pwr.get_battery_voltage() / 100.0f,
                  (float)timer_pwr.get_battery_current() / 100.0f);
    Serial.printf("grove voltage:%.2fV, grove current:%.2fmA\n", (float)timer_pwr.get_grove_voltage() / 100.0f,
                  (float)timer_pwr.get_grove_current() / 100.0f);
    delay(100);
}