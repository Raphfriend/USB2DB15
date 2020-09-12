#ifndef USB2DB15_LED_H
#define USB2DB15_LED_H

#include <Arduino.h>

#define LED_PIN 8

namespace LED {
  const uint8_t BLINK_RATE_RESET = 50;
  const uint8_t BLINK_RATR_PROFILE = 150;

  void On();
  void Off();
  void Blink(uint8_t num, uint8_t rate );
  void Update();
};

#endif //USB2DB15_LED_H
