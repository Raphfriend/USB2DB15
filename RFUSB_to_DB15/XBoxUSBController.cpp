//
// Created by Kitsune on 8/21/2020.
// Modified by Frank_fjs on 10/9/2020.
//

#include "XBoxUSBController.h"
#include "NeoGeoXboxUSB.h"
#include "device_descriptor.h"

/**
 * Create a new XBoxUSBController
 *
 * @param p Pointer to the XBoxUSB Controller to Wrap
 */
XBoxUSBController::XBoxUSBController(XBOXUSB *p, NeoGeoXboxUSB *neo) {
  xbox360 = p;
  neoGeo = neo;
}

/**
 * Gets if the button has been clicked since the last check
 *
 * This function uses the aliases provide by the Host Shield Library
 * to determine if a given button has been clicked. Only returns true
 * once per button press
 *
 * @param button The button to check. Defined in "device_description.h"
 * @return If the button has been clicked
 */
bool XBoxUSBController::GetButtonClick(uint8_t button) {
  // Added to fix an issue regarding the USB Host Shield Xbox library
  // XBOXUSB.cpp | Line 291 | bool XBOXUSB::getButtonClick(ButtonEnum b)
  // Creates an issue surrounding L2 & R2 where a false GetButtonClick is registered 
  // upon holding SELECT to enter BIND mode
  // TODO Find the source of the issue and fix properly
  // Frank_fjs 9/10/2020
  if(!GetButtonState(button)) {
    return false;
  }
  if (neoGeo && neoGeo->isReady()) {
    switch(button) {
      case BUTTON_UP: return neoGeo->getButtonClick(UP);
      case BUTTON_DOWN: return neoGeo->getButtonClick(DOWN);
      case BUTTON_LEFT: return neoGeo->getButtonClick(LEFT);
      case BUTTON_RIGHT: return neoGeo->getButtonClick(RIGHT);
      case BUTTON_START: return neoGeo->getButtonClick(START);
      case BUTTON_COIN: return neoGeo->getButtonClick(BACK);
      case BUTTON_1: return neoGeo->getButtonClick(A);
      case BUTTON_2: return neoGeo->getButtonClick(B);
      case BUTTON_3: return neoGeo->getButtonClick(R2);
      case BUTTON_4: return neoGeo->getButtonClick(X);
      case BUTTON_5: return neoGeo->getButtonClick(Y);
      case BUTTON_6: return neoGeo->getButtonClick(R1);
      case BUTTON_7: return neoGeo->getButtonClick(L1);
      case BUTTON_8: return neoGeo->getButtonClick(L2);
      case BUTTON_9: return neoGeo->getButtonClick(R3);
      case BUTTON_10: return neoGeo->getButtonClick(L3);
    }
  }
  switch(button) {
    case BUTTON_UP:
      return xbox360->getButtonClick(UP) || xbox360->getAnalogHat(LeftHatY) <= 32767 && xbox360->getAnalogHat(LeftHatY) >= 16384;   
    case BUTTON_DOWN:
      return xbox360->getButtonClick(DOWN) || xbox360->getAnalogHat(LeftHatY) >= -32768 && xbox360->getAnalogHat(LeftHatY) <= -16384;
    case BUTTON_LEFT:
      return xbox360->getButtonClick(LEFT) || xbox360->getAnalogHat(LeftHatX) >= -32768 && xbox360->getAnalogHat(LeftHatX) <= -16384;
    case BUTTON_RIGHT:
      return xbox360->getButtonClick(RIGHT) || xbox360->getAnalogHat(LeftHatX) <= 32767 && xbox360->getAnalogHat(LeftHatX) >= 16384;
    case BUTTON_START:
      return xbox360->getButtonClick(START);
    case BUTTON_COIN:
      return xbox360->getButtonClick(BACK);
    case BUTTON_1:
      return xbox360->getButtonClick(A);
    case BUTTON_2:
      return xbox360->getButtonClick(B);
    case BUTTON_3:
      return xbox360->getButtonClick(R2);
    case BUTTON_4:
      return xbox360->getButtonClick(X);
    case BUTTON_5:
      return xbox360->getButtonClick(Y);
    case BUTTON_6:
      return xbox360->getButtonClick(R1);
    case BUTTON_7:
      return xbox360->getButtonClick(L1);
    case BUTTON_8:
      return xbox360->getButtonClick(L2);
    case BUTTON_9:
      return xbox360->getButtonClick(R3);
    case BUTTON_10:
      return xbox360->getButtonClick(L3);
  }
  return false;
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
bool XBoxUSBController::GetButtonState(uint8_t button) {
  if (neoGeo && neoGeo->isReady()) {
    switch(button) {
      case BUTTON_UP: return neoGeo->getButtonPress(UP);
      case BUTTON_DOWN: return neoGeo->getButtonPress(DOWN);
      case BUTTON_LEFT: return neoGeo->getButtonPress(LEFT);
      case BUTTON_RIGHT: return neoGeo->getButtonPress(RIGHT);
      case BUTTON_START: return neoGeo->getButtonPress(START);
      case BUTTON_COIN: return neoGeo->getButtonPress(BACK);
      case BUTTON_1: return neoGeo->getButtonPress(A);
      case BUTTON_2: return neoGeo->getButtonPress(B);
      case BUTTON_3: return neoGeo->getButtonPress(R2);
      case BUTTON_4: return neoGeo->getButtonPress(X);
      case BUTTON_5: return neoGeo->getButtonPress(Y);
      case BUTTON_6: return neoGeo->getButtonPress(R1);
      case BUTTON_7: return neoGeo->getButtonPress(L1);
      case BUTTON_8: return neoGeo->getButtonPress(L2);
      case BUTTON_9: return neoGeo->getButtonPress(R3);
      case BUTTON_10: return neoGeo->getButtonPress(L3);
    }
  }
  switch(button) {
    case BUTTON_UP:
      return xbox360->getButtonPress(UP) || xbox360->getAnalogHat(LeftHatY) <= 32767 && xbox360->getAnalogHat(LeftHatY) >= 16384; 
    case BUTTON_DOWN:
      return xbox360->getButtonPress(DOWN) || xbox360->getAnalogHat(LeftHatY) >= -32768 && xbox360->getAnalogHat(LeftHatY) <= -16384;
    case BUTTON_LEFT:
      return xbox360->getButtonPress(LEFT) || xbox360->getAnalogHat(LeftHatX) >= -32768 && xbox360->getAnalogHat(LeftHatX) <= -16384;
    case BUTTON_RIGHT:
      return xbox360->getButtonPress(RIGHT) || xbox360->getAnalogHat(LeftHatX) <= 32767 && xbox360->getAnalogHat(LeftHatX) >= 16384;
    case BUTTON_START:
      return xbox360->getButtonPress(START);
    case BUTTON_COIN:
      return xbox360->getButtonPress(BACK);
    case BUTTON_1:
      return xbox360->getButtonPress(A);
    case BUTTON_2:
      return xbox360->getButtonPress(B);
    case BUTTON_3:
      return xbox360->getButtonPress(R2);
    case BUTTON_4:
      return xbox360->getButtonPress(X);
    case BUTTON_5:
      return xbox360->getButtonPress(Y);
    case BUTTON_6:
      return xbox360->getButtonPress(R1);
    case BUTTON_7:
      return xbox360->getButtonPress(L1);
    case BUTTON_8:
      return xbox360->getButtonPress(L2);
    case BUTTON_9:
      return xbox360->getButtonPress(R3);
    case BUTTON_10:
      return xbox360->getButtonPress(L3);
  }
  return false;
}

/**
 * Checks if an Official XBoxUSB controller is connected to the system
 *
 * Checks if a XBoxUSB controller is connected to the system. The Vid and Pid
 * of XboxUSB Controllers is defined in the Host Shield Library.  Any
 * Controllers not defined as XBoxUSB controllers in the library will be
 * treated as generic HID controllers
 *
 * @return true if an XBoxUSB controller is connected to the system
 */
bool XBoxUSBController::Connected() {
  // A connection check must not send a control request on every loop.
  return (neoGeo && neoGeo->isReady()) || xbox360->Xbox360Connected;
}

/**
 * Gets the Vendor ID for the controller
 *
 * As there is no way to currently get the exact vid out of the XBoxUSB
 * Controller Class we will use a hardcoded value. This is one of the possible
 * VIDs for an XBoxUSB controller
 *
 * @return The Controller's Vendor ID
 */
uint16_t XBoxUSBController::GetVid() {
  if (neoGeo && neoGeo->isReady()) return 0x2DC8;
  return XBOX_VID;
}

/**
 * Gets the Product ID for the controller
 *
 * As there is no way to currently get the exact vid out of the XBoxUSB
 * Controller Class we will use a hardcoded value. This is one of the possible
 * VIDs for an XBoxUSB controller
 *
 * @return The Controller's Product ID
 */
uint16_t XBoxUSBController::GetPid() {
  if (neoGeo && neoGeo->isReady()) return 0x3106;
  return XBOX_WIRED_PID;
}
