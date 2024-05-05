/*
 * Tauno_RGBLED_Wheel.h
 *
 * Started 05.05.2024
 * Edited  05.05.2024
 * 
 * Copyright 2024 Tauno Erik
 */

#ifndef LIB_TAUNO_RGBLED_WHEEL_SRC_TAUNO_RGBLED_WHEEL_H_
#define LIB_TAUNO_RGBLED_WHEEL_SRC_TAUNO_RGBLED_WHEEL_H_

#include <Arduino.h>

class Tauno_RGBLED_Wheel {
 private:
    uint8_t _LED_PIN;

 public:
    Tauno_RGBLED_Wheel(
        uint8_t LED_PIN);

    ~Tauno_RGBLED_Wheel();

    void begin();
};

#endif  // LIB_TAUNO_RGBLED_WHEEL_SRC_TAUNO_RGBLED_WHEEL_H_
