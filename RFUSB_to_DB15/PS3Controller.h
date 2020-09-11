//
// Created by Kitsune on 8/21/2020.
//

#ifndef USB2DB15_PS3CONTROLLER_H
#define USB2DB15_PS3CONTROLLER_H

#include <PS3USB.h>
#include "Controller.h"

/**
 * Wrapper for the USB Host Shield PS3 Controller
 *
 * This class wraps the USB Host Shield PS3 controller. It implements
 * the Controller Interface that the USB2DB15 class expects.
 */
class PS3Controller : public Controller {
  PS3USB *ps3usb;
public:
  PS3Controller(PS3USB *p);

  bool GetButtonClick(uint8_t button);
  bool GetButtonState(uint8_t button);
  bool Connected();

  uint16_t GetVid();
  uint16_t GetPid();
};


#endif //USB2DB15_PS3CONTROLLER_H
