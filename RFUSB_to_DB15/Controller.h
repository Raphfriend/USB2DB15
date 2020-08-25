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
  virtual bool GetButtonState(uint8_t button);
  virtual bool Connected();
};


#endif //USB2DB15_CONTROLLER_H
