//
// Created by Kitsune on 8/21/2020.
//

#ifndef USB2DB15_XBOXUSBCONTROLLER_H
#define USB2DB15_XBOXUSBCONTROLLER_H

#include <XBOXUSB.h>
#include "Controller.h"

/**
 * Wrapper for the USB Host Shield XBoxOne Controller
 *
 * This class wraps the USB Host Shield XBoxUSB controller. It implements
 * the Controller Interface that the USB2DB15 class expects.
 */
class XBoxUSBController : public Controller {
  XBOXUSB *xbox360;

public:
  XBoxUSBController(XBOXUSB *p);

  bool GetButtonClick(uint8_t button);
  bool GetButtonState(uint8_t button);
  bool Connected();

  uint16_t GetVid();
  uint16_t GetPid();
};

#endif //USB2DB15_XBOXUSBCONTROLLER_H
