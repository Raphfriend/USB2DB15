#ifndef USB2DB15_LED_H
#define USB2DB15_LED_H

#define LED_PIN 8

#define BLINK_HALF_LENGTH 100

class LED {

    static uint8_t blinks;
    static uint8_t on;
    static unsigned long next_blink;

  public:

    static void On() {
      on = true;
    };
    static void Off() {
      on = false;
    };
    static void Blink(uint8_t num) {
      blinks = num;
      on = true;
      next_blink = millis() + BLINK_HALF_LENGTH;
    }

    static void Update() {
      if (blinks && millis() >= next_blink) {
        if (on) blinks--;
        on = !on;
        next_blink = millis() + BLINK_HALF_LENGTH;
      }
      if (on) {
        digitalWrite(LED_PIN, HIGH);
      } else {
        digitalWrite(LED_PIN, LOW);
      }
    };
};

#endif //USB2DB15_LED_H
