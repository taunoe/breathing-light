/*
 * Tauno_RGBLED_Wheel.cpp
 *
 * Started 05.05.2024
 * Edited  07.05.2024
 * 
 * Copyright 2024 Tauno Erik
 */

#include <Arduino.h>
#include "Adafruit_GFX.h"
#include "Adafruit_NeoPixel.h"
#include "Tauno_RGBLED_Wheel.h"

Tauno_RGBLED_Wheel::Tauno_RGBLED_Wheel(
    uint8_t LED_PIN)
    : _LED_PIN(LED_PIN),
    pixels(WHEEL_LED_COUNT, _LED_PIN, NEO_GRB + NEO_KHZ800)
    {}

Tauno_RGBLED_Wheel::~Tauno_RGBLED_Wheel() {
    // Cleanup
}

bool Tauno_RGBLED_Wheel::begin() {
    //pinMode(_LED_PIN, OUTPUT);
    pixels.begin();            // INITIALIZE NeoPixel strip object (REQUIRED)
    pixels.show();             // Turn OFF all pixels ASAP
    pixels.setBrightness(WHEEL_BRIGHTNESS);  // 20 == Set BRIGHTNESS to about 1/5 (max = 255)

    return true;
}

// color: 0x7aff18
bool Tauno_RGBLED_Wheel::led_on(int index, uint32_t color) {
    if (index > WHEEL_LED_COUNT) {
        return false;
    }

    int old = pixels.getPixelColor(index);

    if (old != color) {
        pixels.clear();
        pixels.setPixelColor(index, color);
        pixels.show();
    }
    return true;
}
