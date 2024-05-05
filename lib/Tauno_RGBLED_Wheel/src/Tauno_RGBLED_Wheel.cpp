/*
 * Tauno_RGBLED_Wheel.cpp
 *
 * Started 05.05.2024
 * Edited  05.05.2024
 * 
 * Copyright 2024 Tauno Erik
 */

#include <Arduino.h>
#include "Tauno_RGBLED_Wheel.h"

Tauno_RGBLED_Wheel::Tauno_RGBLED_Wheel(
    uint8_t LED_PIN)
    : _LED_PIN(LED_PIN)
    {}

Tauno_RGBLED_Wheel::~Tauno_RGBLED_Wheel() {
    // Cleanup
}

void Tauno_RGBLED_Wheel::begin() {
    pinMode(_LED_PIN, OUTPUT);
}
