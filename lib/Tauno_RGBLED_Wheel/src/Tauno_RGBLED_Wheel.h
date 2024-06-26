/*
 * Tauno_RGBLED_Wheel.h
 *
 * Started 05.05.2024
 * Edited  22.06.2024
 * 
 * Copyright 2024 Tauno Erik
 */

#ifndef LIB_TAUNO_RGBLED_WHEEL_SRC_TAUNO_RGBLED_WHEEL_H_
#define LIB_TAUNO_RGBLED_WHEEL_SRC_TAUNO_RGBLED_WHEEL_H_

#include <Arduino.h>
#include "Adafruit_GFX.h"
#include "Adafruit_NeoPixel.h"

#define WHEEL_NUM_OF_CIRCLES  9
// LEDs from outside to inside
#define WHEEL_CIRCLE_1_SIZE  35
#define WHEEL_CIRCLE_2_SIZE  35
#define WHEEL_CIRCLE_3_SIZE  35
#define WHEEL_CIRCLE_4_SIZE  28
#define WHEEL_CIRCLE_5_SIZE  28
#define WHEEL_CIRCLE_6_SIZE  21
#define WHEEL_CIRCLE_7_SIZE  21
#define WHEEL_CIRCLE_8_SIZE  14
#define WHEEL_CIRCLE_9_SIZE   7
#define WHEEL_LED_COUNT     224  // Total

#define WHEEL_BRIGHTNESS     40  // 0-255

class Tauno_RGBLED_Wheel {
 private:
    uint8_t _LED_PIN;
    Adafruit_NeoPixel pixels;
    uint32_t colours_transition(uint8_t value);

 public:
    Tauno_RGBLED_Wheel(uint8_t LED_PIN);

    ~Tauno_RGBLED_Wheel();

    bool begin();
    bool led_on(int index, uint32_t color);
    bool led_on_hsv(int index, uint16_t hue, uint8_t sat, uint8_t val);
    bool led_off(int index);
    bool all_off();
    bool all_on(uint32_t color);

    void rainbow(int wait);
    void arches_bottom_to_up(uint32_t color, int wait);
    void circles(uint32_t color, int wait);
    void circle_on(int index, uint32_t color);
    void katsetus();
};

#endif  // LIB_TAUNO_RGBLED_WHEEL_SRC_TAUNO_RGBLED_WHEEL_H_
