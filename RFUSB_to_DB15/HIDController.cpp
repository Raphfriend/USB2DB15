//
// Created by Kitsune on 8/22/2020.
//

#include "drivers.h"
#include "HIDController.h"
/*
void HIDController::DebugStatePrint() {
  Serial.print("HID Input: ");
  if(buttonState & MASK_UP) {
    Serial.print("UP ");
  }
  if(buttonState & MASK_UP_RIGHT) {
    Serial.print("UP/RIGHT ");
  }
  if(buttonState & MASK_RIGHT) {
    Serial.print("RIGHT ");
  }
  if(buttonState & MASK_DOWN_RIGHT) {
    Serial.print("DOWN/RIGHT ");
  }
  if(buttonState & MASK_DOWN) {
    Serial.print("DOWN ");
  }
  if(buttonState & MASK_DOWN_LEFT) {
    Serial.print("DOWN/LEFT ");
  }
  if(buttonState & MASK_LEFT) {
    Serial.print("LEFT ");
  }
  if(buttonState & MASK_UP_LEFT) {
    Serial.print("UP/LEFT ");
  }
  if(buttonState & MASK_COIN) {
    Serial.print("SELECT ");
  }
  if(buttonState & MASK_START) {
    Serial.print("START ");
  }
  if(buttonState & MASK_BUTTON_1) {
    Serial.print("BUTTON 1 ");
  }
  if(buttonState & MASK_BUTTON_2) {
    Serial.print("BUTTON 2 ");
  }
  if(buttonState & MASK_BUTTON_3) {
    Serial.print("BUTTON 3 ");
  }
  if(buttonState & MASK_BUTTON_4) {
    Serial.print("BUTTON 4 ");
  }
  if(buttonState & MASK_BUTTON_5) {
    Serial.print("BUTTON 5 ");
  }
  if(buttonState & MASK_BUTTON_6) {
    Serial.print("BUTTON 6 ");
  }
  if(buttonState & MASK_BUTTON_7) {
    Serial.print("BUTTON 7 ");
  }
  if(buttonState & MASK_BUTTON_8) {
    Serial.print("BUTTON 8 ");
  }
  if(buttonState & MASK_BUTTON_9) {
    Serial.print("BUTTON 9 ");
  }
  if(buttonState & MASK_BUTTON_10) {
    Serial.print("BUTTON 10 ");
  }
  Serial.println();
} */

bool HIDController::Connected() {
  isReady();
}

bool HIDController::GetButtonState(uint8_t button) {
  switch (button) {
    case BUTTON_UP :
      return (buttonState & MASK_UP)  || (buttonState & MASK_UP_RIGHT) || (buttonState & MASK_UP_LEFT);
    case BUTTON_DOWN :
      return (buttonState & MASK_DOWN)  || (buttonState & MASK_DOWN_RIGHT) || (buttonState & MASK_DOWN_LEFT);
    case BUTTON_RIGHT :
      return (buttonState & MASK_RIGHT)  || (buttonState & MASK_UP_RIGHT) || (buttonState & MASK_DOWN_RIGHT);
    case BUTTON_LEFT :
      return (buttonState & MASK_LEFT)  || (buttonState & MASK_UP_LEFT) || (buttonState & MASK_DOWN_LEFT);
    case BUTTON_COIN :
      return buttonState & MASK_COIN;
    case BUTTON_START :
      return buttonState & MASK_START;
    case BUTTON_1 :
      return buttonState & MASK_BUTTON_1;
    case BUTTON_2 :
      return buttonState & MASK_BUTTON_2;
    case BUTTON_3 :
      return buttonState & MASK_BUTTON_3;
    case BUTTON_4 :
      return buttonState & MASK_BUTTON_4;
    case BUTTON_5 :
      return buttonState & MASK_BUTTON_5;
    case BUTTON_6 :
      return buttonState & MASK_BUTTON_6;
    case BUTTON_7 :
      return buttonState & MASK_BUTTON_7;
    case BUTTON_8 :
      return buttonState & MASK_BUTTON_8;
    case BUTTON_9 :
      return buttonState & MASK_BUTTON_9;
    case BUTTON_10 :
      return buttonState & MASK_BUTTON_10;
  }
  return 0;
}

void HIDController::ConfigButton(uint8_t button_id, uint8_t index, uint8_t mask) {
  ConfigButton(button_id, index, mask, mask);
}

void HIDController::ConfigButton(uint8_t button_id, uint8_t index, uint8_t mask, uint8_t value) {
  if (button_id >= MAX_HID_BUTTONS) return;

  Serial.print("Button ID: ");
  Serial.print(button_id, DEC);
  Serial.print(" Index: ");
  Serial.print(index, DEC);
  Serial.print(" Mask: ");
  Serial.print(mask, BIN);
  Serial.print(" Value: ");
  Serial.println(value, DEC);

  buttons[button_id].index = index;
  buttons[button_id].mask = mask;
  buttons[button_id].value = value;
}

uint8_t HIDController::OnInitSuccessful() {
  Serial.println("Detected HID device. ");
  ResetController();
  setupController(VID, PID, this);
}

void HIDController::ParseHIDData(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf) {
  buttonState = 0;
  uint8_t value = 0;

  // For each button shift left then append the button state
  // UP is the left most bit, BUTTON_10 is right most
  for(uint8_t i = 0; i < MAX_HID_BUTTONS; i++) {
    buttonState <<= 1;  // Shift to the next bit
    if (buttons[i].mask == 0) continue; // Skip this bit as there is no button associated with it

    value = buf[buttons[i].index] & buttons[i].mask;  // Get the bits we care about
    if(value == buttons[i].value) {
      buttonState |= 1;  // Set the final bit
    }
  }
}

void HIDController::ResetController() {
  for(uint8_t i = 0; i < MAX_HID_BUTTONS; i++) {
    buttons[i].mask = 0;
  }
}
