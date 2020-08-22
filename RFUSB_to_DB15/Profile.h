//
// Created by Kitsune on 8/21/2020.
//

#ifndef USB2DB15_PROFILE_H
#define USB2DB15_PROFILE_H

#include <stdint.h>

#define NUM_BUTTON_BINDINGS 12

// Profile Buttons
#define PROFILE_BUTTON_UP       0
#define PROFILE_BUTTON_RIGHT    1
#define PROFILE_BUTTON_DOWN     2
#define PROFILE_BUTTON_LEFT     3
#define PROFILE_BUTTON_START    4
#define PROFILE_BUTTON_COIN     5
#define PROFILE_BUTTON_1        6
#define PROFILE_BUTTON_2        7
#define PROFILE_BUTTON_3        8
#define PROFILE_BUTTON_4        9
#define PROFILE_BUTTON_5        10
#define PROFILE_BUTTON_6        11

struct Profile {
  uint8_t bindings[NUM_BUTTON_BINDINGS];

  uint8_t SwapButtons(uint8_t a, uint8_t b);
  uint8_t SetButton(uint8_t target, uint8_t destination);
  void Copy(Profile &p);
};


#endif //USB2DB15_PROFILE_H
