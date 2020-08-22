//
// Created by Kitsune on 8/21/2020.
//

#include "USB2DB15.h"

void debugOutput(uint8_t ddrc, uint8_t ddrd) {
  Serial.print("Output: ");
  if( ddrc & DDRC_UP) {
    Serial.print("UP ");
  }

  if( ddrd & DDRD_DOWN) {
    Serial.print("DOWN ");
  }

  if( ddrc & DDRC_LEFT) {
    Serial.print("LEFT ");
  }

  if( ddrd & DDRD_RIGHT) {
    Serial.print("RIGHT ");
  }

  if( ddrc & DDRC_START) {
    Serial.print("START ");
  }

  if( ddrd & DDRD_COIN) {
    Serial.print("SELECT ");
  }

  if( ddrc & DDRC_1) {
    Serial.print("A ");
  }

  if( ddrd & DDRD_2) {
    Serial.print("B ");
  }

  if( ddrc & DDRC_3) {
    Serial.print("C ");
  }

  if( ddrd & DDRD_4) {
    Serial.print("X ");
  }

  if( ddrd & DDRD_5) {
    Serial.print("Y ");
  }

  if( ddrc & DDRC_6) {
    Serial.print("Z ");
  }
  Serial.println(" ");
}

USB2DB15::USB2DB15(PS3Controller &ps3, XBoxOneController &xbox) : ps3(ps3), xbox(xbox) {
  GenerateBuiltinProfiles();
};

void USB2DB15::GenerateOutput() {
  uint8_t ddrc = 0; // getDDRC();
  uint8_t ddrd = 0; // getDDRD();
  uint8_t button = 0;

  // Select the Controller and Generate DDRC and DDRD
  if(ps3.Connected()) { // PS3
    ddrc = GetDDRC(profiles[curProfile], ps3);
    ddrd = GetDDRD(profiles[curProfile], ps3);
  } else if (xbox.Connected()) { // XboxOne
    ddrc = GetDDRC(profiles[curProfile], xbox);
    ddrd = GetDDRD(profiles[curProfile], xbox);
  }

  if(ddrc == prevDDRC && ddrd == prevDDRD) {
    return; // Nothing has changed
  }
  prevDDRC = ddrc;
  prevDDRD = ddrd;

  // Check Special Button Combinations
  // Change Profile
  if((ddrd & DDRD_COIN) && (ddrc & DDRC_UP)) {
    if(ps3.Connected()) { // PS3
      SetProfile(ps3, BUILT_IN_PROFILES);
    } else if (xbox.Connected()) { // XboxOne
      SetProfile(xbox, BUILT_IN_PROFILES);
    }
    return;
  }
  // Output
  DDRC = ddrc;
  DDRD = ddrd;

  debugOutput(ddrc, ddrd);
}

void USB2DB15::GenerateBuiltinProfiles() {
  GenerateDefaultProfile(profiles[0]);
  GenerateRowSwapProfile(profiles[1], profiles[0]);
  GenerateSnesProfile(profiles[2], profiles[0]);
  GenerateRowSwapProfile(profiles[3], profiles[2]);
}

void USB2DB15::GenerateDefaultProfile(Profile &profile) {
  profile.SetButton(BUTTON_UP, PROFILE_BUTTON_UP);
  profile.SetButton(BUTTON_RIGHT, PROFILE_BUTTON_RIGHT);
  profile.SetButton(BUTTON_DOWN, PROFILE_BUTTON_DOWN);
  profile.SetButton(BUTTON_LEFT, PROFILE_BUTTON_LEFT);
  profile.SetButton(BUTTON_START, PROFILE_BUTTON_START);
  profile.SetButton(BUTTON_COIN, PROFILE_BUTTON_COIN);
  profile.SetButton(BUTTON_1, PROFILE_BUTTON_1);
  profile.SetButton(BUTTON_2, PROFILE_BUTTON_2);
  profile.SetButton(BUTTON_3, PROFILE_BUTTON_3);
  profile.SetButton(BUTTON_4, PROFILE_BUTTON_4);
  profile.SetButton(BUTTON_5, PROFILE_BUTTON_5);
  profile.SetButton(BUTTON_6, PROFILE_BUTTON_6);
}

void USB2DB15::GenerateRowSwapProfile(Profile &profile, Profile &base) {
  profile.Copy(base);
  profile.SwapButtons(PROFILE_BUTTON_1, PROFILE_BUTTON_4);
  profile.SwapButtons(PROFILE_BUTTON_2, PROFILE_BUTTON_5);
  profile.SwapButtons(PROFILE_BUTTON_3, PROFILE_BUTTON_6);
}

void USB2DB15::GenerateSnesProfile(Profile &profile, Profile &base) {
  profile.Copy(base);
  profile.SetButton(BUTTON_7, PROFILE_BUTTON_6);
}

uint8_t USB2DB15::GetDDRC(Profile &profile, Controller &controller) {
  uint8_t ddrc = 0;

  if(controller.GetButtonState(profile.bindings[PROFILE_BUTTON_UP])) {
    ddrc |= DDRC_UP;
  }

  if(controller.GetButtonState(profile.bindings[PROFILE_BUTTON_LEFT])) {
    ddrc |= DDRC_LEFT;
  }

  if(controller.GetButtonState(profile.bindings[PROFILE_BUTTON_1])) {
    ddrc |= DDRC_1;
  }

  if(controller.GetButtonState(profile.bindings[PROFILE_BUTTON_3])) {
    ddrc |= DDRC_3;
  }

  if(controller.GetButtonState(profile.bindings[PROFILE_BUTTON_6])) {
    ddrc |= DDRC_6;
  }

  if(controller.GetButtonState(profile.bindings[PROFILE_BUTTON_START])) {
    ddrc |= DDRC_START;
  }

  return ddrc;
}

uint8_t USB2DB15::GetDDRD(Profile &profile, Controller &controller) {
  uint8_t ddrd = 0;

  if(controller.GetButtonState(profile.bindings[PROFILE_BUTTON_RIGHT])) {
    ddrd |= DDRD_RIGHT;
  }

  if(controller.GetButtonState(profile.bindings[PROFILE_BUTTON_DOWN])) {
    ddrd |= DDRD_DOWN;
  }

  if(controller.GetButtonState(profile.bindings[PROFILE_BUTTON_2])) {
    ddrd |= DDRD_2;
  }

  if(controller.GetButtonState(profile.bindings[PROFILE_BUTTON_4])) {
    ddrd |= DDRD_4;
  }

  if(controller.GetButtonState(profile.bindings[PROFILE_BUTTON_5])) {
    ddrd |= DDRD_5;
  }

  if(controller.GetButtonState(profile.bindings[PROFILE_BUTTON_COIN])) {
    ddrd |= DDRD_COIN;
  }

  return ddrd;
}

// Gets the number of the lowest index button pressed
void USB2DB15::SetProfile(Controller &controller, uint8_t page) {
  if(controller.GetButtonState(profiles[0].bindings[PROFILE_BUTTON_1])) {
    Serial.print("Using Profile: ");
    Serial.println((0 + page * 6));
    curProfile = 0 + page * 6;
  }

  if(controller.GetButtonState(profiles[0].bindings[PROFILE_BUTTON_2])) {
    Serial.print("Using Profile: ");
    Serial.println((1 + page * 6));
    curProfile = 1 + page * 6;
  }

  if(controller.GetButtonState(profiles[0].bindings[PROFILE_BUTTON_3])) {
    Serial.print("Using Profile: ");
    Serial.println((2 + page * 6));
    curProfile = 2 + page * 6;
  }

  if(controller.GetButtonState(profiles[0].bindings[PROFILE_BUTTON_4])) {
    Serial.print("Using Profile: ");
    Serial.println((3 + page * 6));
    curProfile = 3 + page * 6;
  }
  // if(ddrd & DDRD_5) return curProfile = 0;
  // if(ddrc & DDRC_6) return curProfile = 0;
}
