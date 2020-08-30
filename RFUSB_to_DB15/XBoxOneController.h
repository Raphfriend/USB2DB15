//
// Created by Kitsune on 8/21/2020.
//

#ifndef USB2DB15_XBOXONECONTROLLER_H
#define USB2DB15_XBOXONECONTROLLER_H

#include <XBOXONE.h>
#include "Controller.h"

/**
 * Wrapper for the USB Host Shield XBoxOne Controller
 *
 * This class wraps the USB Host Shield XBoxOne controller. It implements
 * the Controller Interface that the USB2DB15 class expects.
 */
class XBoxOneController : public Controller {
  XBOXONE *xbox;

public:
  XBoxOneController(XBOXONE *p);

  bool GetButtonClick(uint8_t button);
  bool GetButtonState(uint8_t button);
  bool Connected();
};

#endif //USB2DB15_XBOXONECONTROLLER_H
