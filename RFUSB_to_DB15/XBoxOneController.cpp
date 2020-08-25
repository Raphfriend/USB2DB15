//
// Created by Kitsune on 8/21/2020.
//

#include "XBoxOneController.h"
#include "device_descriptor.h"

/**
 * Create a new XBoxOneController
 *
 * @param p Pointer to the XBoxOne Controller to Wrap
 */
XBoxOneController::XBoxOneController(XBOXONE *p) {
  xbox = p;
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
/**
 * Checks if an Official XBoxOne controller is connected to the system
 *
 * Checks if a XBoxOne controller is connected to the system. The Vid and Pid
 * of XboxOne Controllers is defined in the Host Shield Library.  Any
 * Controllers not defined as XBoxOne controllers in the library will be
 * treated as generic HID controllers
 *
 * @return true if an XBoxOne controller is connected to the system
 */
bool XBoxOneController::Connected() {
  if (xbox->XboxOneConnected) {
    xbox->setRumbleOff();
    return true;
  }
  return false;
}
