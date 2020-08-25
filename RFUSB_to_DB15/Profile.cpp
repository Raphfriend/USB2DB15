//
// Created by Kitsune on 8/21/2020.
//

#include "Profile.h"

/**
 * Swaps the bindings of 2 buttons
 *
 * Button index numbers are defined in profile.h
 *
 * @param a The first button
 * @param b The second button
 * @return 0 if no error otherwise the error number
 */
uint8_t Profile::SwapButtons(uint8_t a, uint8_t b) {
  if(a >= NUM_BUTTON_BINDINGS || b >= NUM_BUTTON_BINDINGS) {
    return 1;
  }

  uint8_t temp = bindings[a];
  bindings[a] = bindings[b];
  bindings[b] = temp;
  return 0;
}

/**
 * Binds controller button to a profile button
 *
 * @param target Controller button.  Ids defined in "device_descriptor.h"
 * @param destination Profile Button. Ids defined in "profile.h"
 * @return 0 if no error otherwise the error number
 */
uint8_t Profile::SetButton(uint8_t target, uint8_t destination) {
  if(destination >= NUM_BUTTON_BINDINGS) {
    return 1;
  }

  bindings[destination] = target;
  return 0;
}

/**
 * Copies all the bindings from one profile to another
 *
 * @param p The profile to copy bindings from
 */
void Profile::Copy(Profile &p) {
  for(uint8_t i = 0; i < NUM_BUTTON_BINDINGS; i++) {
    bindings[i] = p.bindings[i];
  }
}