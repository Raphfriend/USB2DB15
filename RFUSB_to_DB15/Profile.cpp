//
// Created by Kitsune on 8/21/2020.
//

#include "Profile.h"

uint8_t Profile::SwapButtons(uint8_t a, uint8_t b) {
  if(a >= NUM_BUTTON_BINDINGS || b >= NUM_BUTTON_BINDINGS) {
    return 1;
  }

  uint8_t temp = bindings[a];
  bindings[a] = bindings[b];
  bindings[b] = temp;
  return 0;
}

uint8_t Profile::SetButton(uint8_t target, uint8_t destination) {
  if(destination >= NUM_BUTTON_BINDINGS) {
    return 1;
  }

  bindings[destination] = target;
  return 0;
}

void Profile::Copy(Profile &p) {
  for(uint8_t i = 0; i < NUM_BUTTON_BINDINGS; i++) {
    bindings[i] = p.bindings[i];
  }
}