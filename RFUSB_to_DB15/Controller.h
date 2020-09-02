//
// Created by Kitsune on 8/21/2020.
//

#ifndef USB2DB15_CONTROLLER_H
#define USB2DB15_CONTROLLER_H

#include <stdint.h>

/**
 * A Generic Controller interface.
 *
 * This exists to abstract away the concept of a controller so that we
 * can treat all the controllers the same regardless of if we are using
 * the generic HID interface or a controller specific one
 */
class Controller {
public:
  virtual bool GetButtonClick(uint8_t button); // Only returns true once per button press
  virtual bool GetButtonState(uint8_t button); // returns true if the button is currently pressed
  virtual bool Connected();
  virtual uint16_t GetVid();
  virtual uint16_t GetPid();
};


#endif //USB2DB15_CONTROLLER_H
