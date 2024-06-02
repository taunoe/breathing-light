/*
 * Tauno_RGBLED_Wheel.cpp
 *
 * Started 05.05.2024
 * Edited  01.06.2024
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
        // pixels.clear();  // all off
        pixels.setPixelColor(index, color);
        pixels.show();
    }
    return true;
}

bool Tauno_RGBLED_Wheel::led_off(int index) {
    uint32_t color = 0x000000;
    if (index > WHEEL_LED_COUNT) {
        return false;
    }

    int old = pixels.getPixelColor(index);

    if (old != color) {
        // pixels.clear();  // all off
        pixels.setPixelColor(index, color);
        pixels.show();
    }
    return true;
}

bool Tauno_RGBLED_Wheel::all_off() {
    pixels.clear();
    pixels.show();
    return true;
}

bool Tauno_RGBLED_Wheel::all_on(uint32_t color) {
    for (int i = 0; i < WHEEL_LED_COUNT; i++) {
        pixels.setPixelColor(i, color);
    }
    pixels.show();
    return true;
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
// Is Adafruit Wheel()
uint32_t Tauno_RGBLED_Wheel::colours_transition(uint8_t value) {
    value = 255 - value;

    if (value < 85) {
        return pixels.Color(255 - value * 3, 0, value * 3);
    }
    if (value < 170) {
        value -= 85;
        return pixels.Color(0, value * 3, 255 - value * 3);
    }
    value -= 170;
    return pixels.Color(value * 3, 255 - value * 3, 0);
}


void Tauno_RGBLED_Wheel::rainbow(int wait) {
    static int pixel_interval = 0;
    static int pixel_cycle = 0;
    static uint32_t pixel_previous = 0;   // Previous Pixel Millis
    static uint32_t patternPrevious = 0;  // Previous Pattern Millis

    if (pixel_interval != wait) {
        pixel_interval = wait;
    }

    uint32_t currentMillis = millis();  //  Update current time

    if (currentMillis - pixel_previous >= pixel_interval) {
      pixel_previous = currentMillis;

      for (uint16_t led = 0; led < WHEEL_LED_COUNT; led++) {
        //  Update delay time
        pixels.setPixelColor(led, colours_transition((led + pixel_cycle) & 255));
      }

      pixels.show();
      pixel_cycle++;
      if (pixel_cycle >= 256) {
        pixel_cycle = 0;
      }
    }
}


uint32_t Tauno_RGBLED_Wheel::b_colours_transition(uint8_t value) {
    value = 255 - value;

    if (value < 85) {
        return pixels.Color(0, 255 - value * 3, value * 3);
    }
    if (value < 170) {
        value -= 85;
        return pixels.Color(0, value * 3, 255 - value * 3);
    }
    value -= 170;
    return pixels.Color(0, value * 3, 255 - value * 3);
}

void Tauno_RGBLED_Wheel::b_rainbow(int wait) {
    static int pixel_interval = 0;
    static int pixel_cycle = 0;
    static uint32_t pixel_previous = 0;   // Previous Pixel Millis
    static uint32_t patternPrevious = 0;  // Previous Pattern Millis

    if (pixel_interval != wait) {
        pixel_interval = wait;
    }

    uint32_t currentMillis = millis();  //  Update current time

    if (currentMillis - pixel_previous >= pixel_interval) {
      pixel_previous = currentMillis;

      for (uint16_t led = 0; led < WHEEL_LED_COUNT; led++) {
        //  Update delay time
        pixels.setPixelColor(led, colours_transition((led + pixel_cycle) & 255));
      }

      pixels.show();
      pixel_cycle++;
      if (pixel_cycle >= 256) {
        pixel_cycle = 0;
      }
    }
}

