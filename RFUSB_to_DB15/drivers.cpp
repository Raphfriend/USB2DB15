//
// Created by Kitsune on 8/23/2020.
//

#include "drivers.h"

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

void generateDPad(uint8_t index, uint8_t mask, HIDController *controller) {
  controller->ConfigButton(BUTTON_UP, index, mask, 0);
  controller->ConfigButton(BUTTON_UP_RIGHT, index, mask, 1);
  controller->ConfigButton(BUTTON_RIGHT, index, mask, 2);
  controller->ConfigButton(BUTTON_DOWN_RIGHT, index, mask, 3);
  controller->ConfigButton(BUTTON_DOWN, index, mask, 4);
  controller->ConfigButton(BUTTON_DOWN_LEFT, index, mask, 5);
  controller->ConfigButton(BUTTON_LEFT, index, mask, 6);
  controller->ConfigButton(BUTTON_UP_LEFT, index, mask, 7);
}

/**************************
 * 8BitDo GamePads
 **************************/

void setup8BitDoM30(HIDController *controller) {
  // DPad setup
  generateDPad(2, 0x0F, controller);

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

void setupHoriFightingCmdr(HIDController *controller) {
  // DPad setup
  generateDPad(2, 0x0F, controller);

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

void setupHoriRAP3(HIDController *controller) {
  generateDPad(2, 0x0F, controller);

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

void setupPS4(HIDController *controller) {
  generateDPad(5, 0x0F, controller);

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
