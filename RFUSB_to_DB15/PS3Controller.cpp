//
// Created by Kitsune on 8/21/2020.
//

#include "PS3Controller.h"
#include "device_descriptor.h"

/**
 * Create a new PS3Controller
 *
 * @param p Pointer to the PS3 Controller to Wrap
 */
PS3Controller::PS3Controller(PS3USB *p) {
  ps3usb = p;
}
/**
 * Gets the State(pressed or not) of a given button
 *
 * This function uses the aliases provide by the Host Shield Library
 * to determine if a given button is pressed on the controller
 *
 * @param button The button to check. Defined in "device_description.h"
 * @return If the requested button is pressed
 */
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

/**
 * Checks if an Official PS3 controller is connected to the system
 *
 * Checks if a PS3 controller is connected to the system. The Vid and Pid
 * of PS3 Controllers is defined in the Host Shield Library.  Any
 * Controllers not defined as PS3 controllers in the library will be
 * treated as generic HID controllers. See the RAP3 PS3 driver for
 * HID support
 *
 * @return true if an PS3 controller is connected to the system
 */
bool PS3Controller::Connected() {
  return ps3usb->PS3Connected;
}
