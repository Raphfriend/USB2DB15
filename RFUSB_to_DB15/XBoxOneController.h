//
// Created by Kitsune on 8/21/2020.
//

#ifndef USB2DB15_XBOXONECONTROLLER_H
#define USB2DB15_XBOXONECONTROLLER_H

#include <XBOXONE.h>
#include "Controller.h"

class XBoxOneController : public Controller {
  XBOXONE *xbox;

public:
  XBoxOneController(XBOXONE *p);

  bool GetButtonState(uint8_t button);
  bool Connected();
};

#endif //USB2DB15_XBOXONECONTROLLER_H
