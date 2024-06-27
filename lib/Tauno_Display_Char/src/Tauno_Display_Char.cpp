/*
 * Tauno_Diaplay_Char.cpp
 *
 * Started 04.05.2024
 * Edited  27.06.2024
 * 
 * Copyright 2024 Tauno Erik
 */


#include <Arduino.h>
#include "Tauno_Display_Char.h"

Tauno_Display_Char::Tauno_Display_Char(
  uint8_t A_PIN,
  uint8_t B_PIN,
  uint8_t DP_PIN,
  uint8_t C_PIN,
  uint8_t D_PIN,
  uint8_t E_PIN,
  uint8_t G_PIN,
  uint8_t F_PIN)
  : _A_PIN(A_PIN),
    _B_PIN(B_PIN),
    _DP_PIN(DP_PIN),
    _C_PIN(C_PIN),
    _D_PIN(D_PIN),
    _E_PIN(E_PIN),
    _G_PIN(G_PIN),
    _F_PIN(F_PIN)
  {}

Tauno_Display_Char::~Tauno_Display_Char() {
    // Cleanup
}

bool Tauno_Display_Char::begin() {
  pinMode(_A_PIN, OUTPUT);
  pinMode(_B_PIN, OUTPUT);
  pinMode(_DP_PIN, OUTPUT);
  pinMode(_C_PIN, OUTPUT);
  pinMode(_D_PIN, OUTPUT);
  pinMode(_E_PIN, OUTPUT);
  pinMode(_G_PIN, OUTPUT);
  pinMode(_F_PIN, OUTPUT);

  _pins[0] = {_A_PIN};
  _pins[1] = {_B_PIN};
  _pins[2] = {_DP_PIN};
  _pins[3] = {_C_PIN};
  _pins[4] = {_D_PIN};
  _pins[5] = {_E_PIN};
  _pins[6] = {_G_PIN};
  _pins[7] = {_F_PIN};

  return true;
}

void Tauno_Display_Char::display(char c) {
  switch (c) {
  case 0:
  case '0':
  case 'o':
  case 'O':
    digitalWrite(_A_PIN, HIGH);
    digitalWrite(_B_PIN, HIGH);
    digitalWrite(_C_PIN, HIGH);
    digitalWrite(_D_PIN, HIGH);
    digitalWrite(_E_PIN, HIGH);
    digitalWrite(_F_PIN, HIGH);
    digitalWrite(_G_PIN, LOW);
    digitalWrite(_DP_PIN, LOW);
    break;
  case 1:
  case '1':
  case 'i':
  case 'I':
    digitalWrite(_A_PIN, LOW);
    digitalWrite(_B_PIN, HIGH);
    digitalWrite(_C_PIN, HIGH);
    digitalWrite(_D_PIN, LOW);
    digitalWrite(_E_PIN, LOW);
    digitalWrite(_F_PIN, LOW);
    digitalWrite(_G_PIN, LOW);
    digitalWrite(_DP_PIN, LOW);
    break;
  case 2:
  case '2':
    digitalWrite(_A_PIN, HIGH);
    digitalWrite(_B_PIN, HIGH);
    digitalWrite(_C_PIN, LOW);
    digitalWrite(_D_PIN, HIGH);
    digitalWrite(_E_PIN, HIGH);
    digitalWrite(_F_PIN, LOW);
    digitalWrite(_G_PIN, HIGH);
    digitalWrite(_DP_PIN, LOW);
    break;
  case 3:
  case '3':
    digitalWrite(_A_PIN, HIGH);
    digitalWrite(_B_PIN, HIGH);
    digitalWrite(_C_PIN, HIGH);
    digitalWrite(_D_PIN, HIGH);
    digitalWrite(_E_PIN, LOW);
    digitalWrite(_F_PIN, LOW);
    digitalWrite(_G_PIN, HIGH);
    digitalWrite(_DP_PIN, LOW);
    break;
  case 4:
  case '4':
    digitalWrite(_A_PIN, LOW);
    digitalWrite(_B_PIN, HIGH);
    digitalWrite(_C_PIN, HIGH);
    digitalWrite(_D_PIN, LOW);
    digitalWrite(_E_PIN, LOW);
    digitalWrite(_F_PIN, HIGH);
    digitalWrite(_G_PIN, HIGH);
    digitalWrite(_DP_PIN, LOW);
    break;
  case 5:
  case '5':
  case 's':
  case 'S':
    digitalWrite(_A_PIN, HIGH);
    digitalWrite(_B_PIN, LOW);
    digitalWrite(_C_PIN, HIGH);
    digitalWrite(_D_PIN, HIGH);
    digitalWrite(_E_PIN, LOW);
    digitalWrite(_F_PIN, HIGH);
    digitalWrite(_G_PIN, HIGH);
    digitalWrite(_DP_PIN, LOW);
    break;
  case 6:
  case '6':
    digitalWrite(_A_PIN, HIGH);
    digitalWrite(_B_PIN, LOW);
    digitalWrite(_C_PIN, HIGH);
    digitalWrite(_D_PIN, HIGH);
    digitalWrite(_E_PIN, HIGH);
    digitalWrite(_F_PIN, HIGH);
    digitalWrite(_G_PIN, HIGH);
    digitalWrite(_DP_PIN, LOW);
    break;
  case 7:
  case '7':
    digitalWrite(_A_PIN, HIGH);
    digitalWrite(_B_PIN, HIGH);
    digitalWrite(_C_PIN, HIGH);
    digitalWrite(_D_PIN, LOW);
    digitalWrite(_E_PIN, LOW);
    digitalWrite(_F_PIN, LOW);
    digitalWrite(_G_PIN, LOW);
    digitalWrite(_DP_PIN, LOW);
    break;
  case 8:
  case '8':
    digitalWrite(_A_PIN, HIGH);
    digitalWrite(_B_PIN, HIGH);
    digitalWrite(_C_PIN, HIGH);
    digitalWrite(_D_PIN, HIGH);
    digitalWrite(_E_PIN, HIGH);
    digitalWrite(_F_PIN, HIGH);
    digitalWrite(_G_PIN, HIGH);
    digitalWrite(_DP_PIN, LOW);
    break;
  case 9:
  case '9':
    digitalWrite(_A_PIN, HIGH);
    digitalWrite(_B_PIN, HIGH);
    digitalWrite(_C_PIN, HIGH);
    digitalWrite(_D_PIN, HIGH);
    digitalWrite(_E_PIN, LOW);
    digitalWrite(_F_PIN, HIGH);
    digitalWrite(_G_PIN, HIGH);
    digitalWrite(_DP_PIN, LOW);
    break;
  case 10:
  case 'a':
  case 'A':
    digitalWrite(_A_PIN, HIGH);
    digitalWrite(_B_PIN, HIGH);
    digitalWrite(_C_PIN, HIGH);
    digitalWrite(_D_PIN, LOW);
    digitalWrite(_E_PIN, HIGH);
    digitalWrite(_F_PIN, HIGH);
    digitalWrite(_G_PIN, HIGH);
    digitalWrite(_DP_PIN, LOW);
    break;
  case 11:
  case 'b':
  case 'B':
    digitalWrite(_A_PIN, LOW);
    digitalWrite(_B_PIN, LOW);
    digitalWrite(_C_PIN, HIGH);
    digitalWrite(_D_PIN, HIGH);
    digitalWrite(_E_PIN, HIGH);
    digitalWrite(_F_PIN, HIGH);
    digitalWrite(_G_PIN, HIGH);
    digitalWrite(_DP_PIN, LOW);
    break;
  case 12:
  case 'c':
  case 'C':
    digitalWrite(_A_PIN, HIGH);
    digitalWrite(_B_PIN, LOW);
    digitalWrite(_C_PIN, LOW);
    digitalWrite(_D_PIN, HIGH);
    digitalWrite(_E_PIN, HIGH);
    digitalWrite(_F_PIN, HIGH);
    digitalWrite(_G_PIN, LOW);
    digitalWrite(_DP_PIN, LOW);
    break;
  case 13:
  case 'd':
  case 'D':
    digitalWrite(_A_PIN, LOW);
    digitalWrite(_B_PIN, HIGH);
    digitalWrite(_C_PIN, HIGH);
    digitalWrite(_D_PIN, HIGH);
    digitalWrite(_E_PIN, HIGH);
    digitalWrite(_F_PIN, LOW);
    digitalWrite(_G_PIN, HIGH);
    digitalWrite(_DP_PIN, LOW);
    break;
  case 14:
  case 'e':
  case 'E':
    digitalWrite(_A_PIN, HIGH);
    digitalWrite(_B_PIN, LOW);
    digitalWrite(_C_PIN, LOW);
    digitalWrite(_D_PIN, HIGH);
    digitalWrite(_E_PIN, HIGH);
    digitalWrite(_F_PIN, HIGH);
    digitalWrite(_G_PIN, HIGH);
    digitalWrite(_DP_PIN, LOW);
    break;
  case 15:
  case 'f':
  case 'F':
    digitalWrite(_A_PIN, HIGH);
    digitalWrite(_B_PIN, LOW);
    digitalWrite(_C_PIN, LOW);
    digitalWrite(_D_PIN, LOW);
    digitalWrite(_E_PIN, HIGH);
    digitalWrite(_F_PIN, HIGH);
    digitalWrite(_G_PIN, HIGH);
    digitalWrite(_DP_PIN, LOW);
    break;
  case 16:
  case 'h':
  case 'H':
    digitalWrite(_A_PIN, LOW);
    digitalWrite(_B_PIN, HIGH);
    digitalWrite(_C_PIN, HIGH);
    digitalWrite(_D_PIN, LOW);
    digitalWrite(_E_PIN, HIGH);
    digitalWrite(_F_PIN, HIGH);
    digitalWrite(_G_PIN, HIGH);
    digitalWrite(_DP_PIN, LOW);
    break;
  case 17:
  case '.':
    digitalWrite(_A_PIN, LOW);
    digitalWrite(_B_PIN, LOW);
    digitalWrite(_C_PIN, LOW);
    digitalWrite(_D_PIN, LOW);
    digitalWrite(_E_PIN, LOW);
    digitalWrite(_F_PIN, LOW);
    digitalWrite(_G_PIN, LOW);
    digitalWrite(_DP_PIN, HIGH);
    break;
  default:  // All off
    digitalWrite(_A_PIN, LOW);
    digitalWrite(_B_PIN, LOW);
    digitalWrite(_C_PIN, LOW);
    digitalWrite(_D_PIN, LOW);
    digitalWrite(_E_PIN, LOW);
    digitalWrite(_F_PIN, LOW);
    digitalWrite(_G_PIN, LOW);
    digitalWrite(_DP_PIN, LOW);
    break;
  }
}

void Tauno_Display_Char::clear() {
  digitalWrite(_A_PIN, LOW);
  digitalWrite(_B_PIN, LOW);
  digitalWrite(_C_PIN, LOW);
  digitalWrite(_D_PIN, LOW);
  digitalWrite(_E_PIN, LOW);
  digitalWrite(_F_PIN, LOW);
  digitalWrite(_G_PIN, LOW);
  digitalWrite(_DP_PIN, LOW);
}

void Tauno_Display_Char::test(uint64_t wait) {
  static uint64_t prev_on_time = 0;
  static uint64_t prev_off_time = 0;
  static bool turn_on_ready = false;
  static bool turn_off_ready = false;
  static int i = 0;

  // Turn ON elements
  while (!turn_on_ready) {
    Serial.print(".");
    if ((millis() - prev_on_time) >= wait) {
      prev_on_time = millis();
      digitalWrite(_pins[i], HIGH);
      i++;
      if (i == 8) {
        turn_on_ready = true;
        i = 0;
      }
    }
  }
  // Turn OFF elements
  while (!turn_off_ready) {
    Serial.print(".");
    if ((millis() - prev_off_time) >= wait) {
      prev_off_time = millis();
      digitalWrite(_pins[i], LOW);
      i++;
      if (i == 8) {
        turn_off_ready = true;
        i = 0;
      }
    }
  }
  // Display numbers & letters
  const int num_of_letters = 17;
  for (int i = 0; i < num_of_letters; i++) {
    display(i);
    delay(wait);
  }
}
