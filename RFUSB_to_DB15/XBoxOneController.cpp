//
// Created by Kitsune on 8/21/2020.
//

#include "XBoxOneController.h"
#include "device_descriptor.h"

XBoxOneController::XBoxOneController(XBOXONE *p) {
  xbox = p;
}

bool XBoxOneController::GetButtonState(uint8_t button) {
  switch(button) {
    case BUTTON_UP:
      return xbox->getButtonPress(UP);
    case BUTTON_DOWN:
      return xbox->getButtonPress(DOWN);
    case BUTTON_LEFT:
      return xbox->getButtonPress(LEFT);
    case BUTTON_RIGHT:
      return xbox->getButtonPress(RIGHT);
    case BUTTON_START:
      return xbox->getButtonPress(START);
    case BUTTON_COIN:
      return xbox->getButtonPress(BACK);
    case BUTTON_1:
      return xbox->getButtonPress(A);
    case BUTTON_2:
      return xbox->getButtonPress(B);
    case BUTTON_3:
      return xbox->getButtonPress(R2);
    case BUTTON_4:
      return xbox->getButtonPress(X);
    case BUTTON_5:
      return xbox->getButtonPress(Y);
    case BUTTON_6:
      return xbox->getButtonPress(R1);
    case BUTTON_7:
      return xbox->getButtonPress(L1);
    case BUTTON_8:
      return xbox->getButtonPress(L2);
    case BUTTON_9:
      return xbox->getButtonPress(R3);
    case BUTTON_10:
      return xbox->getButtonPress(L3);
  }
  return false;
}

bool XBoxOneController::Connected() {
  return xbox->XboxOneConnected;
}
