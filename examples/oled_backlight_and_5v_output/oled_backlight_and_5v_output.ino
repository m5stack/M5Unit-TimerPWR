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
    Serial.println("press btnA on/off oled backlight");
    Serial.println("press btnB on/off 5V output");
    Serial.println("===============================================");
    delay(1000);
}

void loop()
{
    static uint8_t btn_a_status = 1, btn_b_status = 1;
    static uint8_t last_btn_a_status = 1, last_btn_b_status = 1;
    static uint8_t oled_status, grove_status;

    timer_pwr.get_button_value(&btn_a_status, &btn_b_status);

    if (btn_a_status != last_btn_a_status) {
        if (btn_a_status == 0) {
            oled_status = timer_pwr.get_oled_backlight();
            if (oled_status) {
                timer_pwr.set_oled_backlight(false);
                Serial.println("OLED backlight off");
            } else {
                timer_pwr.set_oled_backlight(true);
                Serial.println("OLED backlight on");
            }
        }
        last_btn_a_status = btn_a_status;
    }

    if (btn_b_status != last_btn_b_status) {
        if (btn_b_status == 0) {
            grove_status = timer_pwr.get_grove_5v_output();
            if (grove_status) {
                timer_pwr.set_grove_5v_output(false);
                Serial.println("5V output off");
            } else {
                timer_pwr.set_grove_5v_output(true);
                Serial.println("5V output on");
            }
        }
        last_btn_b_status = btn_b_status;
    }

    delay(1);
}