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
    Serial.println("press btnA to set wake time 10S and sleep time 20S");
    Serial.println("press btnB to enable/disable cycle mode");
    Serial.println("===============================================");
    delay(1000);
}

void loop()
{
    uint8_t btn_a_status = 1, btn_b_status = 1;
    static uint8_t last_btn_a_status = 1, last_btn_b_status = 1;
    static uint8_t cycle_status;

    timer_pwr.get_button_value(&btn_a_status, &btn_b_status);

    if (btn_a_status != last_btn_a_status) {
        if (btn_a_status == 0) {
            delay(10);
            timer_pwr.get_button_value(&btn_a_status, &btn_b_status);
            if (btn_a_status == 0) {
                timer_pwr.set_power_on_time(0, 0, 10);
                timer_pwr.set_power_off_time(0, 0, 20);
                timer_pwr.save_flash();
                Serial.println("set wake time 10S and sleep time 20S");
                delay(100);
            }
        }
        last_btn_a_status = btn_a_status;
    }

    if (btn_b_status != last_btn_b_status) {
        if (btn_b_status == 0) {
            delay(10);
            timer_pwr.get_button_value(&btn_a_status, &btn_b_status);
            if (btn_b_status == 0) {
                cycle_status = timer_pwr.get_cycle_sleep();
                if (cycle_status) {
                    timer_pwr.set_cycle_sleep(false);
                    Serial.println("cycle mode off");
                } else {
                    timer_pwr.set_cycle_sleep(true);
                    Serial.println("cycle mode on");
                }
                delay(100);
            }
        }
        last_btn_b_status = btn_b_status;
    }

    delay(1);
}