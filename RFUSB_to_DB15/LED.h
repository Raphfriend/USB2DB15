#ifndef USB2DB15_LED_H
#define USB2DB15_LED_H

#include <Arduino.h>

#define LED_PIN 8

#define BLINK_HALF_LENGTH 150

namespace LED {
  void On();
  void Off();
  void Blink(uint8_t num);
  void Update();
};

#endif //USB2DB15_LED_H
