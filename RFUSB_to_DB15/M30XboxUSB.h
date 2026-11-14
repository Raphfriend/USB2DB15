#ifndef USB2DB15_M30_XBOX_USB_H
#define USB2DB15_M30_XBOX_USB_H

#include <XBOXUSB.h>

// Keep the Xbox driver and initialize silent 045E:028E receivers when needed.
class M30XboxUSB : public XBOXUSB {
public:
  M30XboxUSB(USB *usb);
  uint8_t Init(uint8_t parent, uint8_t port, bool lowspeed);
  uint8_t Release();
  uint8_t Poll();

private:
  bool probing;
  unsigned long probeDeadline;
  unsigned long nextProbe;
  void SendM30Start();
};

#endif
