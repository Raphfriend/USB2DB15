//
// Created by Kitsune on 8/21/2020.
//

#ifndef USB2DB15_CONTROLLER_H
#define USB2DB15_CONTROLLER_H

#include <stdint.h>

class Controller {
public:
  virtual bool GetButtonState(uint8_t button);
  virtual bool Connected();
};


#endif //USB2DB15_CONTROLLER_H
