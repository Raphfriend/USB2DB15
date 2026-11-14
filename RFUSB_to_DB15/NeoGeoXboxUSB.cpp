#include "NeoGeoXboxUSB.h"

NeoGeoXboxUSB::NeoGeoXboxUSB(USB *host) : usb(host), address(0), connected(false),
  nextPoll(0), buttonState(0), previousButtonState(0), buttonClickState(0),
  l2Clicked(false), r2Clicked(false) {
  for (uint8_t i = 0; i < 3; ++i) {
    endpoints[i].epAddr = 0;
    endpoints[i].maxPktSize = i ? 32 : 8;
    endpoints[i].bmSndToggle = 0;
    endpoints[i].bmRcvToggle = 0;
    endpoints[i].bmNakPower = i ? USB_NAK_NOWAIT : USB_NAK_MAX_POWER;
  }
  resetInput();
  usb->RegisterDeviceClass(this);
}

void NeoGeoXboxUSB::resetInput() {
  buttonState = 0;
  previousButtonState = 0;
  buttonClickState = 0;
  l2Clicked = false;
  r2Clicked = false;
}

uint8_t NeoGeoXboxUSB::fail(uint8_t result) {
  Release();
  return result;
}

uint8_t NeoGeoXboxUSB::readString(uint8_t index, uint8_t length, uint8_t *buffer) {
  return usb->getStrDescr(address, 0, length, index, 0x0409, buffer);
}

uint8_t NeoGeoXboxUSB::Init(uint8_t parent, uint8_t port, bool lowspeed) {
  if (address) return USB_ERROR_CLASS_INSTANCE_ALREADY_IN_USE;

  AddressPool &pool = usb->GetAddressPool();
  UsbDevice *zero = pool.GetUsbDevicePtr(0);
  if (!zero || !zero->epinfo) return USB_ERROR_EPINFO_IS_NULL;

  EpInfo *previous = zero->epinfo;
  zero->epinfo = endpoints;
  zero->lowspeed = lowspeed;
  USB_DEVICE_DESCRIPTOR descriptor;
  uint8_t result = usb->getDevDescr(0, 0, sizeof(descriptor), (uint8_t *)&descriptor);
  zero->epinfo = previous;
  if (result) return result;
  if (!VIDPIDOK(descriptor.idVendor, descriptor.idProduct))
    return USB_DEV_CONFIG_ERROR_DEVICE_NOT_SUPPORTED;

  address = pool.AllocAddress(parent, false, port);
  if (!address) return USB_ERROR_OUT_OF_ADDRESS_SPACE_IN_POOL;
  endpoints[0].maxPktSize = descriptor.bMaxPacketSize0;

  UsbDevice *device = pool.GetUsbDevicePtr(address);
  if (!device) return fail(USB_ERROR_ADDRESS_NOT_FOUND_IN_POOL);
  device->lowspeed = lowspeed;
  result = usb->setAddr(0, 0, address);
  if (result) return fail(result);
  result = usb->setEpInfoEntry(address, 1, endpoints);
  if (result) return fail(result);

  uint8_t configuration[49];
  result = usb->getConfDescr(address, 0, 9, 0, configuration);
  if (result) return fail(result);
  if ((configuration[2] | ((uint16_t)configuration[3] << 8)) != sizeof(configuration) ||
      configuration[5] != 1) return fail(USB_ERROR_INVALID_ARGUMENT);
  result = usb->getConfDescr(address, 0, sizeof(configuration), 0, configuration);
  if (result) return fail(result);

  uint8_t status[2];
  result = usb->ctrlReq(address, 0, bmREQ_GET_DESCR, USB_REQUEST_GET_STATUS,
                        0, 0, 0, 2, 2, status, NULL);
  if (result) return fail(result);

  uint8_t stringData[68];
  result = readString(3, 2, stringData);
  if (result) return fail(result);

  endpoints[1].epAddr = 4;
  endpoints[1].bmNakPower = USB_NAK_NOWAIT;
  endpoints[1].bmSndToggle = 0;
  endpoints[1].bmRcvToggle = 0;
  endpoints[2].epAddr = 5;
  endpoints[2].bmNakPower = USB_NAK_NOWAIT;
  endpoints[2].bmSndToggle = 0;
  endpoints[2].bmRcvToggle = 0;
  result = usb->setEpInfoEntry(address, 3, endpoints);
  if (result) return fail(result);

  result = usb->setConf(address, 0, configuration[5]);
  if (result) return fail(result);

  connected = true;
  resetInput();
  nextPoll = millis();
  result = sendStartupPackets();
  if (result) return fail(result);
  return 0;
}

uint8_t NeoGeoXboxUSB::Release() {
  connected = false;
  resetInput();
  if (address) usb->GetAddressPool().FreeAddress(address);
  address = 0;
  return 0;
}

uint8_t NeoGeoXboxUSB::sendStartupPackets() {
  const uint8_t commands[3][3] = {{0x01, 0x03, 0x02}, {0x02, 0x08, 0x03}, {0x01, 0x03, 0x02}};
  for (uint8_t i = 0; i < 3; ++i) {
    const uint8_t result = usb->outTransfer(address, endpoints[2].epAddr, 3, (uint8_t *)commands[i]);
    if (result) return result;
    if (i == 1) delay(8);
  }
  return 0;
}

uint8_t NeoGeoXboxUSB::Poll() {
  if (!connected || (long)(millis() - nextPoll) < 0) return 0;
  nextPoll = millis() + 1;
  uint8_t report[20];
  uint16_t length = sizeof(report);
  const uint8_t result = usb->inTransfer(address, endpoints[1].epAddr, &length, report);
  if (result == 0 && length) parseReport(report, length);
  return result;
}

void NeoGeoXboxUSB::parseReport(const uint8_t *report, uint16_t length) {
  if (length < 14 || report[0] != 0x00 || report[1] != 0x14) return;
  buttonState = (uint32_t)(report[5] | ((uint16_t)report[4] << 8) |
                 ((uint32_t)report[3] << 16) | ((uint32_t)report[2] << 24));
  if (buttonState != previousButtonState) {
    buttonClickState = (buttonState >> 16) & ((~previousButtonState) >> 16);
    if ((uint8_t)previousButtonState == 0 && (uint8_t)buttonState != 0) r2Clicked = true;
    if ((uint8_t)(previousButtonState >> 8) == 0 && (uint8_t)(buttonState >> 8) != 0) l2Clicked = true;
    previousButtonState = buttonState;
  }
}

uint8_t NeoGeoXboxUSB::getButtonPress(ButtonEnum button) {
  if (button == L2) return (uint8_t)(buttonState >> 8);
  if (button == R2) return (uint8_t)buttonState;
  return (bool)(buttonState & ((uint32_t)pgm_read_word(&XBOX_BUTTONS[(uint8_t)button]) << 16));
}

bool NeoGeoXboxUSB::getButtonClick(ButtonEnum button) {
  if (button == L2) { if (l2Clicked) { l2Clicked = false; return true; } return false; }
  if (button == R2) { if (r2Clicked) { r2Clicked = false; return true; } return false; }
  const uint16_t mask = pgm_read_word(&XBOX_BUTTONS[(uint8_t)button]);
  const bool click = buttonClickState & mask;
  buttonClickState &= ~mask;
  return click;
}

