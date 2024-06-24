/*
 * Tauno_RGBLED_Wheel.cpp
 *
 * Started 05.05.2024
 * Edited  22.06.2024
 * 
 * Copyright 2024 Tauno Erik
 */

#include <Arduino.h>
#include "Adafruit_GFX.h"
#include "Adafruit_NeoPixel.h"
#include "Tauno_RGBLED_Wheel.h"

// Constructor
Tauno_RGBLED_Wheel::Tauno_RGBLED_Wheel(
    uint8_t LED_PIN)
    : _LED_PIN(LED_PIN),
    pixels(WHEEL_LED_COUNT, _LED_PIN, NEO_GRB + NEO_KHZ800)
    {}

// Deconstructor
Tauno_RGBLED_Wheel::~Tauno_RGBLED_Wheel() {
    // Cleanup
}

bool Tauno_RGBLED_Wheel::begin() {
    pixels.begin();                          // INITIALIZE NeoPixel strip object
    pixels.show();                           // Turn OFF all pixels
    pixels.setBrightness(WHEEL_BRIGHTNESS);  // max = 255

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
        // uint32_t gamma_color = pixels.gamma32(color);
        pixels.setPixelColor(index, color);
        // pixels.show();
    }
    return true;
}

// hue  An unsigned 16-bit value, 0 to 65535, representing one full
//      loop of the color wheel, which allows 16-bit hues to "roll over"
// sat  Saturation, 8-bit value, 0 (min or pure grayscale) to 255
//      (max or pure hue). Default of 255 if unspecified.
// val  Value (brightness), 8-bit value, 0 (min / black / off) to
//      255 (max or full brightness). Default of 255 if unspecified.
bool Tauno_RGBLED_Wheel::led_on_hsv(int index,
                                    uint16_t hue,
                                    uint8_t sat,
                                    uint8_t val)
{
    if (index > WHEEL_LED_COUNT) {
        return false;
    }
    uint32_t color = pixels.ColorHSV(hue, sat, val);
    pixels.setPixelColor(index, color);

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

// Rainbow kogu ketta peal
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

// Kaared liiguvad alt keskelt ülesse
// Color: 0x000cff
// Wait ms
void Tauno_RGBLED_Wheel::arches_bottom_to_up(uint32_t color, int wait) {
    uint32_t now = millis();
    static uint32_t prev = 0;

    static bool go_up = true;
    static bool is_paus = false;
    const int paus_time = 2000;
    static int index = 0;
    const int size = 16;
    // LED ringid väljast sisse poole
    int ledid_1_r[size] = {25, 24, 23,  22,  21,  20,  19,  18, 17, 16, 15, 14, 13, 12, 11, 10};
    int ledid_1_l[size] = {27, 28, 29,  30,  31,  32,  33,  34,  0,  1,  2,  3,  4,  5,  6,  7};

    int ledid_2_r[size] = {60, 59, 58,  57,  56,  55,  54,  53, 52, 51, 50, 49, 48, 47, 46, 45};
    int ledid_2_l[size] = {62, 63, 64,  65,  66,  67,  68,  69, 35, 36, 37, 38, 39, 40, 41, 42};

    int ledid_3_r[size] = {95, 94, 93,  92,  91,  90,  89,  88, 87, 86, 85, 84, 83, 82, 81, 80};
    int ledid_3_l[size] = {97, 98, 99, 100, 101, 102, 103, 104, 70, 71, 72, 73, 74, 75, 76, 77};

    // Always ON
    for (int i = 217; i < 224; i++) {
        // inner circle
        led_on(i, color);
    }
    
    led_on(26, color);
    led_on(61, color);
    led_on(96, color);

    if (is_paus) {
        if ((millis() - prev) >= paus_time) {
            is_paus = false;
        }
        if (index == 0) {
            if ((millis() - prev) >= wait/2) {
                led_on(26, 0);
                led_on(61, 0);
                led_on(96, 0);
                pixels.show();
            }
        }
    } else {
        led_on(26, color);
        led_on(61, color);
        led_on(96, color);
    }

    if ((millis() - prev) >= wait && !is_paus) {
        Serial.print("go_up "); Serial.println(go_up);
        prev = millis();
        // Up
        if (go_up) {
            Serial.print("if index ");
            Serial.println(index);
            led_on(ledid_1_r[index], color);
            led_on(ledid_1_l[index], color);
            led_on(ledid_2_r[index], color);
            led_on(ledid_2_l[index], color);
            led_on(ledid_3_r[index], color);
            led_on(ledid_3_l[index], color);
            pixels.show();
            index++;
            if (index == size) {
                is_paus = true;
                go_up = false;
                index = size -1;
            }
        // Down
        } else {
            Serial.print("else index ");
            Serial.println(index);
            led_on(ledid_1_r[index], 0);
            led_on(ledid_1_l[index], 0);
            led_on(ledid_2_r[index], 0);
            led_on(ledid_2_l[index], 0);
            led_on(ledid_3_r[index], 0);
            led_on(ledid_3_l[index], 0);
            pixels.show();
            index--;
            if (index < 0) {
                is_paus = true;
                go_up = true;
                index = 0;
            }
        }
    }
}

// Circles move in and out
void Tauno_RGBLED_Wheel::circles(uint32_t color, int wait) {
    // static uint32_t all_colors[WHEEL_LED_COUNT] = {0};
    static uint32_t prev = 0;
    static int ring = 1;  // 1-9
    static bool out = false;

    if ((millis() - prev) >= wait) {
        if (out) {
            ring--;
        } else {
            ring++;
        }
        prev = millis();
    }

    if (ring <= 0) {
        out = false;
    }

    if (ring > WHEEL_NUM_OF_CIRCLES) {
        ring = 8;
        out = true;
    }

    switch (ring) {
    case 1:
        circle_on(2, 0);  // OFF
        circle_on(1, color);
        pixels.show();
        break;
    case 2:
        circle_on(1, 0);  // OFF
        circle_on(3, 0);  // OFF
        circle_on(2, color);
        pixels.show();
        break;
    case 3:
        circle_on(2, 0);  // OFF
        circle_on(4, 0);  // OFF
        circle_on(3, color);
        pixels.show();
        break;
    case 4:
        circle_on(3, 0);  // OFF
        circle_on(4, color);
        circle_on(5, 0);  // OFF
        pixels.show();
        break;
    case 5:
        circle_on(4, 0);  // OFF
        circle_on(5, color);
        circle_on(6, 0);  // OFF
        pixels.show();
        break;
    case 6:
        circle_on(5, 0);  // OFF
        circle_on(6, color);
        circle_on(7, 0);  // OFF
        pixels.show();
        break;
    case 7:
        circle_on(6, 0);  // OFF
        circle_on(7, color);
        circle_on(8, 0);  // OFF
        pixels.show();
        break;
    case 8:
        circle_on(7, 0);  // OFF
        circle_on(8, color);
        circle_on(9, 0);  // OFF
        pixels.show();
        break;
    case 9:
        circle_on(8, 0);  // OFF
        circle_on(9, color);
        pixels.show();
        break;
    default:
        break;
    }

}


void Tauno_RGBLED_Wheel::circle_on(int index, uint32_t color) {
    // delay(1);
    switch (index) {
    case 1:
        for (int led = 0; led < 35; led++) {
            led_on(led, color);
        }
        break;
    case 2:
        for (int led = 35; led < 35+35; led++) {
            led_on(led, color);
        }
        break;
    case 3:
        for (int led = 35+35; led < 35+35+35; led++) {
            led_on(led, color);
        }
        break;
    case 4:
        for (int led = 35+35+35; led < 35+35+35+28; led++) {
            led_on(led, color);
        }
        break;
    case 5:
        for (int led = 35+35+35+28; led < 35+35+35+28+28; led++) {
            led_on(led, color);
        }
        break;
    case 6:
        for (int led = 35+35+35+28+28; led < 35+35+35+28+28+21; led++) {
            led_on(led, color);
        }
        break;
    case 7:
        for (int led = 35+35+35+28+28+21; led < 35+35+35+28+28+21+21; led++) {
            led_on(led, color);
        }
        break;
    case 8:
        for (int led = 35+35+35+28+28+21+21; led < 35+35+35+28+28+21+21+14; led++) {
            led_on(led, color);
        }
        break;
    case 9:
        for (int led = 35+35+35+28+28+21+21+14; led < 35+35+35+28+28+21+21+14+7; led++) {
            led_on(led, color);
        }
        break;
    default:
        break;
    }
}


void Tauno_RGBLED_Wheel::katsetus() {
    // hue 0 to 65535
    for (int value = 0; value < 255; value++) {
        led_on_hsv(1, 10, 200, value);
        led_on_hsv(2, 10, 200, value);
        led_on_hsv(3, 10, 200, value);
        led_on_hsv(4, 10, 200, value);
        led_on_hsv(5, 10, 200, value);
        pixels.show();
        delay(1);
    }
    delay(500);
    for (int value = 254; value > 0; value--) {
        led_on_hsv(1, 10, 200, value);
        led_on_hsv(2, 10, 200, value);
        led_on_hsv(3, 10, 200, value);
        led_on_hsv(4, 10, 200, value);
        led_on_hsv(5, 10, 200, value);
        pixels.show();
        delay(1);
    }
    delay(500);
}
