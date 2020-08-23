//
// Created by Kitsune on 8/22/2020.
//

#ifndef USB2DB15_HIDCONTROLLER_H
#define USB2DB15_HIDCONTROLLER_H

#include <hiduniversal.h>

#include "Controller.h"
#include "device_descriptor.h"

#define MAX_HID_BUTTONS 20

#define MASK_UP         0x80000
#define MASK_UP_RIGHT   0x40000
#define MASK_RIGHT      0x20000
#define MASK_DOWN_RIGHT 0x10000
#define MASK_DOWN       0x8000
#define MASK_DOWN_LEFT  0x4000
#define MASK_LEFT       0x2000
#define MASK_UP_LEFT    0x1000
#define MASK_START      0x0800
#define MASK_COIN       0x0400
#define MASK_BUTTON_1   0x0200
#define MASK_BUTTON_2   0x0100
#define MASK_BUTTON_3   0x0080
#define MASK_BUTTON_4   0x0040
#define MASK_BUTTON_5   0x0020
#define MASK_BUTTON_6   0x0010
#define MASK_BUTTON_7   0x0008
#define MASK_BUTTON_8   0x0004
#define MASK_BUTTON_9   0x0002
#define MASK_BUTTON_10  0x0001


/**
 * Stores information on how to tell if a button is pressed
 * A Mask of 0x00 signals that the button is not set for the
 * current controller.
 */
struct Button {
  uint8_t index;
  uint8_t mask;
  uint8_t value;
}; 

// Multiple Inheritance solves data sharing problems here, but can be slow
class HIDController : public HIDUniversal, public Controller {
  Button buttons[MAX_HID_BUTTONS];
  uint32_t buttonState;

public:
  HIDController(USB *usb) : HIDUniversal(usb), Controller() {};

  void ConfigButton(uint8_t button_id, uint8_t index, uint8_t mask); // Uses mask as target value
  void ConfigButton(uint8_t button_id, uint8_t index, uint8_t mask, uint8_t value);

  bool Connected();
  bool GetButtonState(uint8_t button);

protected:
  uint8_t OnInitSuccessful();
  void ParseHIDData(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf);
  void ResetController();
};


#endif //USB2DB15_HIDCONTROLLER_H
