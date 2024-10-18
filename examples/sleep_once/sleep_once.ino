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
    Serial.println("===============================================");
    Serial.println("press btnA to sleep immediately and wakeup after 30S");
    Serial.println("press btnB to sleep after 5S and wakeup after 30S");
    Serial.println("===============================================");
    delay(1000);
}

void loop()
{
    static uint8_t last_btn_a_status = 1, last_btn_b_status = 1;
    uint8_t btn_a_status = 1, btn_b_status = 1;

    timer_pwr.get_button_value(&btn_a_status, &btn_b_status);

    if (btn_a_status != last_btn_a_status) {
        if (btn_a_status == 0) {
            delay(10);
            timer_pwr.get_button_value(&btn_a_status, &btn_b_status);
            if (btn_a_status == 0) {
                timer_pwr.set_power_on_time(0, 0, 0);
                timer_pwr.set_power_off_time(0, 0, 30);
                timer_pwr.set_sleep();
                Serial.println("sleep immediately");
            }
        }
        last_btn_a_status = btn_a_status;
    }

    if (btn_b_status != last_btn_b_status) {
        if (btn_b_status == 0) {
            delay(10);
            timer_pwr.get_button_value(&btn_a_status, &btn_b_status);
            if (btn_b_status == 0) {
                timer_pwr.set_power_on_time(0, 0, 5);
                timer_pwr.set_power_off_time(0, 0, 30);
                timer_pwr.set_sleep();
                Serial.println("sleep after 5S");
            }
        }
        last_btn_b_status = btn_b_status;
    }

    delay(1);
}