//
// Created by Kitsune on 8/23/2020.
//

#include "drivers.h"

/**
 * Looks up a controller based on VID and PID and configures a HIDController to
 * work with it.
 *
 * This function takes in a VID and PID as well as a controller to configure.
 * It will look up the exact controller and invoke the proper setup function to
 * configure it. If it cannot find the controller it defaults to using the PS4
 * setup.
 *
 * When adding new controllers to this make sure to define the VID and PID in
 * "drivers.h". If an existing setup function works completely for the
 * controller then you should use it, but if they differ even a little bit
 * please create a new function instead of adding logic to an existing setup
 * function.
 *
 * Vendor and Product IDs are defined in drivers.h
 *
 * @param vid The Vendor ID for the controller.
 * @param pid The Product ID for the controller.
 * @param controller A pointer to the HIDController that you want to configure.
 */

void setupController(uint16_t vid, uint16_t pid, HIDController *controller) {
  switch (vid) {
    case VID_8BITDO :
      if(pid == PID_8BITDO_M30) setup8BitDoM30(controller);
      break;

    case VID_BUFFALO:
      if(pid == PID_BUFFALO_CLASSIC) setupBuffaloClassic(controller);
      break;

    case VID_BROOK:
      if(pid == PID_BROOK_UNIVERSAL) setupPS4(controller);
      break;

    case VID_HORI:
      switch(pid) {
        case PID_HORI_CMDR:
          setupHoriFightingCmdr(controller);
          break;
        case PID_HORI_HORIPAD:
        case PID_HORI_LITE:
        case PID_HORI_RAP_V_PS4:
          setupPS4(controller);
          break;
        case PID_HORI_RAP_PS3:
        case PID_HORI_RAP_PREMIUM:
        case PID_HORI_RAP_V_PS3:
          setupHoriRAP3(controller);
          break;
      }
      break;

    case VID_RAZER:
      if(pid == PID_RAZER_PANTHERA) setupPS4(controller);
      break;

    case VID_RETROBIT:
      if(pid == PID_RETROBIT_SATURN) setupHoriRAP3(controller);
      break;

    case VID_SONY:
      switch(pid) {
        case PID_SONY_PS4_JP:
        case PID_SONY_PS4_NA:
        case PID_SONY_PS4_ADAPTER:
          setupPS4(controller);
          break;
      }
      break;

    case VID_UPCB :
      if(pid == PID_UPCB) setupPS4(controller);
      break;

    default:
      Serial.println("Failed to find a driver for device. Using PS4 as fallback");
      Serial.print("VID: ");
      Serial.print(vid, HEX);
      Serial.print(" PID ");
      Serial.println(pid, HEX);
      setupPS4(controller);
      break;
  }
}

/**************************
 * Utility functions
 **************************/

/**
 * Configures a D-Pad using the lower 4 bits of the provided byte
 *
 * This function only works for DPads who are in the lower 4 bits
 * of the byte. The mask is hardcoded to 0x0F because of this
 *
 * @param index The index of the byte where the dpad is located in the USB
 *              data packet.  See HIDUniversal#ParseHIDData in the Host
 *              Shield Library for the data packet.
 * @param controller The HIDController that will be configured
 */
void generateDPad(uint8_t index, HIDController *controller) {
  controller->ConfigButton(BUTTON_UP, index, DPAD_MASK, DPAD_UP);
  controller->ConfigButton(BUTTON_UP_RIGHT, index, DPAD_MASK, DPAD_UP_RIGHT);
  controller->ConfigButton(BUTTON_RIGHT, index, DPAD_MASK, DPAD_RIGHT);
  controller->ConfigButton(BUTTON_DOWN_RIGHT, index, DPAD_MASK, DPAD_DOWN_RIGHT);
  controller->ConfigButton(BUTTON_DOWN, index, DPAD_MASK, DPAD_DOWN);
  controller->ConfigButton(BUTTON_DOWN_LEFT, index, DPAD_MASK, DPAD_DOWN_LEFT);
  controller->ConfigButton(BUTTON_LEFT, index, DPAD_MASK, DPAD_LEFT);
  controller->ConfigButton(BUTTON_UP_LEFT, index, DPAD_MASK, DPAD_UP_LEFT);
}

/**************************
 * 8BitDo GamePads
 **************************/

/**
 * Configures a 8BitDo M30 Wired Controller and Compatible devices
 *
 * 8BitDo M30 Button layout
 * Byte 0x01   0x02   0x04   0x08   0x10   0x20   0x40   0x80
 * 0:   Btn 6, Btn 3, NA,    Btn 4, Btn 1, NA,    Btn 5, Btn 4
 * 1:   NA,    NA,    COIN,  START, NA,    NA,    NA,    NA
 * 2:   DPAD,  DPAD,  DPAD,  DPAD,  NA,    NA,    NA,    NA
 *
 * @param controller The HIDController that will be configured
 */
void setup8BitDoM30(HIDController *controller) {
  // DPad setup
  generateDPad(2, controller);

  controller->ConfigButton(BUTTON_COIN, 1, 0x04);
  controller->ConfigButton(BUTTON_START, 1, 0x08);
  controller->ConfigButton(BUTTON_1, 0, 0x08);
  controller->ConfigButton(BUTTON_2, 0, 0x10);
  controller->ConfigButton(BUTTON_3, 0, 0x40);
  controller->ConfigButton(BUTTON_4, 0, 0x01);
  controller->ConfigButton(BUTTON_5, 0, 0x02);
  controller->ConfigButton(BUTTON_6, 0, 0x80);
}

/**************************
 * Buffalo GamePads
 **************************/

/**
 * Configures a iBuffalo Classic USB GamePad and Compatible devices
 *
 * 8BitDo M30 Button layout
 * Byte 0x01   0x02   0x04   0x08   0x10   0x20   0x40   0x80
 * 0:                   LEFT(0x00)/RIGHT(0xFF)
 * 1:                   UP(0x00)/DOWN(0xFF)
 * 2:   Btn 5, Btn 4, Btn 2, Btn 1, Btn 6, Btn 3, COIN,  START
 *
 * @param controller The HIDController that will be configured
 */
void setupBuffaloClassic(HIDController *controller) {
  controller->ConfigButton(BUTTON_LEFT, 0, 0xFF, 0);
  controller->ConfigButton(BUTTON_RIGHT, 0, 0xFF, 0xFF);
  controller->ConfigButton(BUTTON_UP, 1, 0xFF, 0);
  controller->ConfigButton(BUTTON_DOWN, 1, 0xFF, 0xFF);
  controller->ConfigButton(BUTTON_COIN, 2, 0x40);
  controller->ConfigButton(BUTTON_START, 2, 0x80);
  controller->ConfigButton(BUTTON_1, 2, 0x08);
  controller->ConfigButton(BUTTON_2, 2, 0x04);
  controller->ConfigButton(BUTTON_3, 2, 0x20);
  controller->ConfigButton(BUTTON_4, 2, 0x02);
  controller->ConfigButton(BUTTON_5, 2, 0x01);
  controller->ConfigButton(BUTTON_6, 2, 0x10);
}

/**************************
 * Hori GamePads
 **************************/

/**
 * Configures a Hori Fighting Commander and Compatible devices
 *
 * 8BitDo M30 Button layout
 * Byte 0x01   0x02   0x04   0x08   0x10   0x20   0x40   0x80
 * 0:   Btn 1, Btn 4, Btn 5, Btn 2, Btn 7, Btn 3, Btn 8, Btn 6
 * 1:   COIN,  START, NA,    NA,    NA,    NA,    NA,    NA
 * 2:   DPAD,  DPAD,  DPAD,  DPAD,  NA,    NA,    NA,    NA
 *
 * @param controller The HIDController that will be configured
 */
void setupHoriFightingCmdr(HIDController *controller) {
  // DPad setup
  generateDPad(2, controller);

  controller->ConfigButton(BUTTON_COIN, 1, 0x01);
  controller->ConfigButton(BUTTON_START, 1, 0x02);
  controller->ConfigButton(BUTTON_1, 0, 0x01);
  controller->ConfigButton(BUTTON_2, 0, 0x08);
  controller->ConfigButton(BUTTON_3, 0, 0x20);
  controller->ConfigButton(BUTTON_4, 0, 0x02);
  controller->ConfigButton(BUTTON_5, 0, 0x04);
  controller->ConfigButton(BUTTON_6, 0, 0x80);
  controller->ConfigButton(BUTTON_7, 0, 0x10);
  controller->ConfigButton(BUTTON_8, 0, 0x40);
}

/**************************
 * Hori GamePads
 **************************/

/**
 * Configures a Hori Real Arcade Pro and Compatible devices
 *
 * 8BitDo M30 Button layout
 * Byte 0x01   0x02   0x04   0x08   0x10   0x20   0x40   0x80
 * 0:   Btn 1, Btn 4, Btn 5, Btn 2, Btn 7, Btn 3, Btn 8, Btn 6
 * 1:   COIN,  START, Btn 9, Btn10, NA,    NA,    NA,    NA
 * 2:   DPAD,  DPAD,  DPAD,  DPAD,  NA,    NA,    NA,    NA
 *
 * @param controller The HIDController that will be configured
 */
void setupHoriRAP3(HIDController *controller) {
  generateDPad(2, controller);

  controller->ConfigButton(BUTTON_COIN, 1, 0x01);
  controller->ConfigButton(BUTTON_START, 1, 0x02);
  controller->ConfigButton(BUTTON_1, 0, 0x01);
  controller->ConfigButton(BUTTON_2, 0, 0x08);
  controller->ConfigButton(BUTTON_3, 0, 0x20);
  controller->ConfigButton(BUTTON_4, 0, 0x02);
  controller->ConfigButton(BUTTON_5, 0, 0x04);
  controller->ConfigButton(BUTTON_6, 0, 0x80);
  controller->ConfigButton(BUTTON_7, 0, 0x10);
  controller->ConfigButton(BUTTON_8, 0, 0x40);
  controller->ConfigButton(BUTTON_9, 1, 0x08);
  controller->ConfigButton(BUTTON_10, 1, 0x04);
}

/**************************
 * Sony GamePads
 **************************/

/**
 * Configures a Hori Fighting Commander and Compatible devices
 *
 * 8BitDo M30 Button layout
 * Byte 0x01   0x02   0x04   0x08   0x10   0x20   0x40   0x80
 * 0-4:                          NA
 * 5:   DPAD,  DPAD,  DPAD,  DPAD,  Btn 1, Btn 4, Btn 5, Btn 2
 * 6:   Btn 7, Btn 3, Btn 8, Btn 6, COIN,  START, NA,    NA
 *
 * @param controller The HIDController that will be configured
 */

void setupPS4(HIDController *controller) {
  generateDPad(5, controller);

  controller->ConfigButton(BUTTON_COIN, 6, 0x10);
  controller->ConfigButton(BUTTON_START, 6, 0x20);
  controller->ConfigButton(BUTTON_1, 5, 0x10);
  controller->ConfigButton(BUTTON_2, 5, 0x80);
  controller->ConfigButton(BUTTON_3, 6, 0x02);
  controller->ConfigButton(BUTTON_4, 5, 0x20);
  controller->ConfigButton(BUTTON_5, 5, 0x40);
  controller->ConfigButton(BUTTON_6, 6, 0x08);
  controller->ConfigButton(BUTTON_7, 6, 0x01);
  controller->ConfigButton(BUTTON_8, 6, 0x04);
}
