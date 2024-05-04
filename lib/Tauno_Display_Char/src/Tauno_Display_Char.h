/*
 * Tauno_Diaplay_Char.h
 * Started 04.05.2024
 * Edited 04.05.2024
 * Copyright 2024 Tauno Erik
 */

#ifndef LIB_TAUNO_DISPLAY_CHAR_SRC_TAUNO_DISPLAY_CHAR_H_
#define LIB_TAUNO_DISPLAY_CHAR_SRC_TAUNO_DISPLAY_CHAR_H_

#include <Arduino.h>

class Tauno_Display_Char {
 private:
  uint8_t _A_PIN;
  uint8_t _B_PIN;
  uint8_t _DP_PIN;
  uint8_t _C_PIN;
  uint8_t _D_PIN;
  uint8_t _E_PIN;
  uint8_t _G_PIN;
  uint8_t _F_PIN;

  uint8_t _pins[8] = {};

 public:
  Tauno_Display_Char(
    uint8_t A_PIN,
    uint8_t B_PIN,
    uint8_t DP_PIN,
    uint8_t C_PIN,
    uint8_t D_PIN,
    uint8_t E_PIN,
    uint8_t G_PIN,
    uint8_t F_PIN);

  ~Tauno_Display_Char();

  void begin();
  void display(char c);
  void clear();
  void test(uint64_t wait);
};

#endif  // LIB_TAUNO_DISPLAY_CHAR_SRC_TAUNO_DISPLAY_CHAR_H_
