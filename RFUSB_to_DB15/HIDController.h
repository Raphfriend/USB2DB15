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
  bool analogEnabled = false;
  uint8_t analogX, analogY, analogBits;
  int32_t lowX, highX, lowY, highY;
  bool analogSigned, analogBigEndian;
  bool invertX, invertY;
  bool analogSocdEnabled = false;
  uint8_t inputReportLength = 0;
  int16_t inputReportId = -1;

public:
  HIDController(USB *usb) : HIDUniversal(usb), Controller() {};

  void ConfigButton(uint8_t button_id, uint8_t index, uint8_t mask); // Uses mask as target value
  void ConfigButton(uint8_t button_id, uint8_t index, uint8_t mask, uint8_t value);
  // Unsigned 8-bit axes. ID=-1 accepts any ID; length=0 accepts any length.
  // Offsets include the report ID byte, when present.
  bool ConfigAnalog(uint8_t x, uint8_t y, uint8_t lx, uint8_t hx,
                    uint8_t ly, uint8_t hy, bool ix = false, bool iy = false,
                    uint8_t reportLength = 0, int16_t reportId = -1);
  bool ConfigReport(uint8_t length, int16_t id = -1);
  // Formatos inteiros de 8/16 bits, com ou sem sinal, alinhados em bytes.
  bool ConfigAnalogAxes(uint8_t x, uint8_t y, uint8_t bits, bool signedValue,
                        bool bigEndian, int32_t lx, int32_t hx, int32_t ly,
                        int32_t hy, bool ix, bool iy, uint8_t length, int16_t id);
  void EnableHitBoxSocd(bool enabled = true);

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
