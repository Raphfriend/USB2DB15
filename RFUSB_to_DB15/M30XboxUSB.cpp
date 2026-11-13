#include "M30XboxUSB.h"

M30XboxUSB::M30XboxUSB(USB *usb) : XBOXUSB(usb), probing(false),
  probeDeadline(0), nextProbe(0) {}

uint8_t M30XboxUSB::Init(uint8_t parent, uint8_t port, bool lowspeed) {
  const uint8_t result = XBOXUSB::Init(parent, port, lowspeed);
  probing = false;
  if (result) return result;

  // The M30 shares the wired Xbox 360 VID/PID. Probe input first so a
  // controller that already reports data stays on the original path.
  uint8_t descriptor[18];
  if (pUsb->getDevDescr(bAddress, 0, sizeof(descriptor), descriptor) == 0 &&
      descriptor[8] == 0x5E && descriptor[9] == 0x04 &&
      descriptor[10] == 0x8E && descriptor[11] == 0x02) {
    probing = true;
    nextProbe = millis();
    probeDeadline = millis() + 750;
  }
  return 0;
}

uint8_t M30XboxUSB::Release() {
  probing = false;
  return XBOXUSB::Release();
}

uint8_t M30XboxUSB::Poll() {
  if (!probing) return XBOXUSB::Poll();

  if ((long)(millis() - nextProbe) >= 0) {
    nextProbe = millis() + 4;
    uint8_t buffer[32];
    uint16_t length = sizeof(buffer);
    const uint8_t result = pUsb->inTransfer(bAddress,
      epInfo[XBOX_INPUT_PIPE].epAddr, &length, buffer);
    if (result == 0 && length != 0) {
      probing = false;
      return 0;
    }
  }

  if ((long)(millis() - probeDeadline) >= 0) {
    probing = false;
    SendM30Start();
  }
  return 0;
}

void M30XboxUSB::SendM30Start() {
  // OUT packets 56, 59, and 67 from the M30 Windows capture.
  // Interleaved vendor requests stalled and are not replayed.
  uint8_t commands[3][3] = {
    {0x01, 0x03, 0x02},
    {0x02, 0x08, 0x03},
    {0x01, 0x03, 0x02}
  };
  for (uint8_t i = 0; i < 3; ++i) {
    if (pUsb->outTransfer(bAddress, epInfo[XBOX_OUTPUT_PIPE].epAddr,
                          3, commands[i])) return;
    if (i == 1) delay(8);
  }
}
