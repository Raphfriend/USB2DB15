#ifndef USB2DB15_NEOGEO_XBOX_USB_H
#define USB2DB15_NEOGEO_XBOX_USB_H

#include <Usb.h>
#include <xboxEnums.h>

class NeoGeoXboxUSB : public USBDeviceConfig {
public:
  explicit NeoGeoXboxUSB(USB *usb);

  uint8_t Init(uint8_t parent, uint8_t port, bool lowspeed);
  uint8_t Release();
  uint8_t Poll();
  uint8_t GetAddress() { return address; }
  bool isReady() { return connected; }
  bool VIDPIDOK(uint16_t vid, uint16_t pid) {
    return vid == 0x2DC8 && pid == 0x3106;
  }

  uint8_t getButtonPress(ButtonEnum button);
  bool getButtonClick(ButtonEnum button);

private:
  USB *usb;
  uint8_t address;
  EpInfo endpoints[3];
  bool connected;
  unsigned long nextPoll;
  uint32_t buttonState;
  uint32_t previousButtonState;
  uint16_t buttonClickState;
  bool l2Clicked;
  bool r2Clicked;

  uint8_t fail(uint8_t result);
  void resetInput();
  uint8_t readString(uint8_t index, uint8_t length, uint8_t *buffer);
  uint8_t sendStartupPackets();
  void parseReport(const uint8_t *report, uint16_t length);
};

#endif
