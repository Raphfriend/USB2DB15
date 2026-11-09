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
#define MASK_DIRECTIONS (MASK_UP | MASK_UP_RIGHT | MASK_RIGHT | MASK_DOWN_RIGHT | \
                         MASK_DOWN | MASK_DOWN_LEFT | MASK_LEFT | MASK_UP_LEFT)

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
  uint32_t buttonState, lastButtonState, clickState;
  bool analogEnabled;
  uint8_t analogX, analogY;
  uint8_t analogLowX, analogHighX, analogLowY, analogHighY;
  uint8_t inputReportLength;
  int16_t inputReportId;

public:
  HIDController(USB *usb) : HIDUniversal(usb), Controller(),
    buttonState(0), lastButtonState(0), clickState(0), analogEnabled(false),
    analogX(0), analogY(0), analogLowX(0), analogHighX(0),
    analogLowY(0), analogHighY(0), inputReportLength(0), inputReportId(-1) {
    ResetController();
  }

  void ConfigButton(uint8_t button_id, uint8_t index, uint8_t mask); // Uses mask as target value
  void ConfigButton(uint8_t button_id, uint8_t index, uint8_t mask, uint8_t value);
  // Configures unsigned 8-bit analog axes. Offsets include the report ID byte.
  // ID=-1 accepts any report ID; length=0 accepts any report length.
  bool ConfigAnalogStick(uint8_t x, uint8_t y, uint8_t lowX, uint8_t highX,
                         uint8_t lowY, uint8_t highY, uint8_t reportLength = 0,
                         int16_t reportId = -1);
  bool ConfigReport(uint8_t length, int16_t id = -1);

  bool Connected();
  bool GetButtonClick(uint8_t button);
  bool GetButtonState(uint8_t button);

  uint16_t GetVid();
  uint16_t GetPid();
protected:
  uint8_t OnInitSuccessful();
  void ParseHIDData(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf);
  void ResetController();
};


#endif //USB2DB15_HIDCONTROLLER_H
