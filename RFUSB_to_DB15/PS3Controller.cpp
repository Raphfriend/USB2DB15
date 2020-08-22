//
// Created by Kitsune on 8/21/2020.
//

#include "PS3Controller.h"
#include "device_descriptor.h"

PS3Controller::PS3Controller(PS3USB *p) {
  ps3usb = p;
}

bool PS3Controller::GetButtonState(uint8_t button) {
  switch(button) {
    case BUTTON_UP:
      return ps3usb->getButtonPress(UP);
    case BUTTON_DOWN:
      return ps3usb->getButtonPress(DOWN);
    case BUTTON_LEFT:
      return ps3usb->getButtonPress(LEFT);
    case BUTTON_RIGHT:
      return ps3usb->getButtonPress(RIGHT);
    case BUTTON_START:
      return ps3usb->getButtonPress(START);
    case BUTTON_COIN:
      return ps3usb->getButtonPress(BACK);
    case BUTTON_1:
      return ps3usb->getButtonPress(SQUARE);
    case BUTTON_2:
      return ps3usb->getButtonPress(TRIANGLE);
    case BUTTON_3:
      return ps3usb->getButtonPress(R1);
    case BUTTON_4:
      return ps3usb->getButtonPress(X);
    case BUTTON_5:
      return ps3usb->getButtonPress(CIRCLE);
    case BUTTON_6:
      return ps3usb->getButtonPress(R2);
    case BUTTON_7:
      return ps3usb->getButtonPress(L1);
    case BUTTON_8:
      return ps3usb->getButtonPress(L2);
    case BUTTON_9:
      return ps3usb->getButtonPress(R3);
    case BUTTON_10:
      return ps3usb->getButtonPress(L3);
  }
  return false;
}

bool PS3Controller::Connected() {
  return ps3usb->PS3Connected;
}
