//
// Created by Kitsune on 8/21/2020.
//

#include "USB2DB15.h"

/**
 * A debug function to print out which buttons are being press.
 * TODO allow this function to be stripped out of release build.
 *
 * @param ddrc The value of DDRC
 * @param ddrd The value of DDRD
 */
void debugOutput(uint8_t ddrc, uint8_t ddrd) {
  Serial.print("Output: ");
  if (ddrc & DDRC_UP) {
    Serial.print("UP ");
  }

  if (ddrd & DDRD_DOWN) {
    Serial.print("DOWN ");
  }

  if (ddrc & DDRC_LEFT) {
    Serial.print("LEFT ");
  }

  if (ddrd & DDRD_RIGHT) {
    Serial.print("RIGHT ");
  }

  if (ddrc & DDRC_START) {
    Serial.print("START ");
  }

  if (ddrd & DDRD_COIN) {
    Serial.print("SELECT ");
  }

  if (ddrc & DDRC_1) {
    Serial.print("A ");
  }

  if (ddrd & DDRD_2) {
    Serial.print("B ");
  }

  if (ddrc & DDRC_3) {
    Serial.print("C ");
  }

  if (ddrd & DDRD_4) {
    Serial.print("X ");
  }

  if (ddrd & DDRD_5) {
    Serial.print("Y ");
  }

  if (ddrc & DDRC_6) {
    Serial.print("Z ");
  }
  Serial.println(" ");
}

/**
 * Constructor for the USB2DB15 class
 *
 * This class controls the mapping of a USB controller to the DB15 connector.
 * Contains logic around profiles.
 *
 * @param ps3 The PS3 controller to use
 * @param xbox The XBox controller to use
 * @param hid  The HID controller to use
 */
USB2DB15::USB2DB15(PS3Controller &ps3, XBoxOneController &xbox, HIDController &hid) : ps3(ps3), xbox(xbox), hid(hid) {
  GenerateBuiltinProfiles();
  curProfile = EEPROM.read(CURRENT_PROFILE_ADDR);
  if (curProfile >= 6) {
    curProfile = 0;
  }
};

/**
 * Outputs the keys pressed on the controller to the DB15 connector
 *
 * This function handles the reading of input from the controller and
 * outputs it to the DB15 connector if it has changed. It also handles
 * the control inputs for setting profiles.
 *
 * This function prioritizes the PS3 driver first, then the XBoxOne,
 * and finally falls back to raw hid if neither are present.
 */
void USB2DB15::GenerateOutput() {
  uint8_t ddrc = 0;
  uint8_t ddrd = 0;
  uint8_t button = 0;

  // Select the Controller and Generate DDRC and DDRD
  if (ps3.Connected()) { // PS3
    ddrc = GetDDRC(profiles[curProfile], ps3);
    ddrd = GetDDRD(profiles[curProfile], ps3);
  } else if (xbox.Connected()) { // XboxOne
    ddrc = GetDDRC(profiles[curProfile], xbox);
    ddrd = GetDDRD(profiles[curProfile], xbox);
  } else if (hid.Connected()) { // Generic HID Controller
    ddrc = GetDDRC(profiles[curProfile], hid);
    ddrd = GetDDRD(profiles[curProfile], hid);
  }

  if (ddrc == prevDDRC && ddrd == prevDDRD) {
    return; // Nothing has changed
  }
  Serial.println(curProfile, DEC);
  prevDDRC = ddrc;
  prevDDRD = ddrd;

  // Check Special Button Combinations
  // Change Profile
  if ((ddrd & DDRD_COIN) && (ddrc & DDRC_UP)) {
    if (ps3.Connected()) { // PS3
      SetProfile(ps3, BUILT_IN_PROFILES);
    } else if (xbox.Connected()) { // XboxOne
      SetProfile(xbox, BUILT_IN_PROFILES);
    } else if (hid.Connected()) { // Generic HID Controller
      SetProfile(hid, BUILT_IN_PROFILES);
    }
    return;
  }
  // Output
  DDRC = ddrc;
  DDRD = ddrd;

  debugOutput(ddrc, ddrd);
}

/**
 * Generates the builtin Profiles
 */
void USB2DB15::GenerateBuiltinProfiles() {
  // Profile 0 is the Default profile
  GenerateDefaultProfile(profiles[0]);
  // Profile 1 is a Row Swap of the default profile
  GenerateRowSwapProfile(profiles[1], profiles[0]);
  // Profile 2 is the Default profile but with L1 instead of R2
  GenerateSnesProfile(profiles[2], profiles[0]);
  // Profile 3 is Profile 2 with the rows swapped
  GenerateRowSwapProfile(profiles[3], profiles[2]);
}


/**
 * Creates a Default profile.
 * @param profile The profile slot to store the profile in
 */
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

/**
 * Copies the base profile into the profile and swaps the top and bottom rows
 *
 * It accomplishes this swap by swapping buttons 1 with 4, 2 with 5,
 * and 3 with 6
 *
 * @param profile The profile slot to store the profile in
 * @param base The profile slot to copy from
 */
void USB2DB15::GenerateRowSwapProfile(Profile &profile, Profile &base) {
  profile.Copy(base);
  profile.SwapButtons(PROFILE_BUTTON_1, PROFILE_BUTTON_4);
  profile.SwapButtons(PROFILE_BUTTON_2, PROFILE_BUTTON_5);
  profile.SwapButtons(PROFILE_BUTTON_3, PROFILE_BUTTON_6);
}

/**
 * Copies the base profile into the profile and switches to SNES style
 *
 * It accomplishes this switch but changing button 6 to button 7
 *
 * @param profile The profile slot to store the profile in
 * @param base The profile slot to copy from
 */
void USB2DB15::GenerateSnesProfile(Profile &profile, Profile &base) {
  profile.Copy(base);
  profile.SetButton(BUTTON_7, PROFILE_BUTTON_6);
}

/**
 * Generates the DDRC Data from the buttons pressed on the controller
 *
 * Uses a profile to map the raw button input from the controller to the
 * data pins in the DDRC register. Pin data masks are defined in USB2DB15.h
 *
 * Does not write to the DDRC resister directly.
 *
 * @param profile The profile to use for mapping the raw button input
 * @param controller The controller to read the raw input from
 * @return The value of DDRC that the raw input should produce
 */
uint8_t USB2DB15::GetDDRC(Profile &profile, Controller &controller) {
  uint8_t ddrc = 0;

  if (controller.GetButtonState(profile.bindings[PROFILE_BUTTON_UP])) {
    ddrc |= DDRC_UP;
  }

  if (controller.GetButtonState(profile.bindings[PROFILE_BUTTON_LEFT])) {
    ddrc |= DDRC_LEFT;
  }

  if (controller.GetButtonState(profile.bindings[PROFILE_BUTTON_1])) {
    ddrc |= DDRC_1;
  }

  if (controller.GetButtonState(profile.bindings[PROFILE_BUTTON_3])) {
    ddrc |= DDRC_3;
  }

  if (controller.GetButtonState(profile.bindings[PROFILE_BUTTON_6])) {
    ddrc |= DDRC_6;
  }

  if (controller.GetButtonState(profile.bindings[PROFILE_BUTTON_START])) {
    ddrc |= DDRC_START;
  }

  return ddrc;
}

/**
 * Generates the DDRD Data from the buttons pressed on the controller
 *
 * Uses a profile to map the raw button input from the controller to the
 * data pins in the DDRD register. Pin data masks are defined in USB2DB15.h
 *
 * Does not write to the DDRD resister directly.
 *
 * @param profile The profile to use for mapping the raw button input
 * @param controller The controller to read the raw input from
 * @return The value of DDRD that the raw input should produce
 */
uint8_t USB2DB15::GetDDRD(Profile &profile, Controller &controller) {
  uint8_t ddrd = 0;

  if (controller.GetButtonState(profile.bindings[PROFILE_BUTTON_RIGHT])) {
    ddrd |= DDRD_RIGHT;
  }

  if (controller.GetButtonState(profile.bindings[PROFILE_BUTTON_DOWN])) {
    ddrd |= DDRD_DOWN;
  }

  if (controller.GetButtonState(profile.bindings[PROFILE_BUTTON_2])) {
    ddrd |= DDRD_2;
  }

  if (controller.GetButtonState(profile.bindings[PROFILE_BUTTON_4])) {
    ddrd |= DDRD_4;
  }

  if (controller.GetButtonState(profile.bindings[PROFILE_BUTTON_5])) {
    ddrd |= DDRD_5;
  }

  if (controller.GetButtonState(profile.bindings[PROFILE_BUTTON_COIN])) {
    ddrd |= DDRD_COIN;
  }

  return ddrd;
}

/**
 * Sets the current Profile
 *
 * Reads the controller input as mapped by the default profile and uses that
 * to decide which profile to use.
 *
 * @param controller The controller to read from
 * @param page The profile page to use. 0 for builtin, 1 user defined
 */
void USB2DB15::SetProfile(Controller &controller, uint8_t page) {
  if (controller.GetButtonState(profiles[0].bindings[PROFILE_BUTTON_1])) {
    Serial.print("Using Profile: ");
    Serial.println((0 + page * 6));
    curProfile = 0 + page * 6;
    EEPROM.update(CURRENT_PROFILE_ADDR, curProfile);
  }

  if (controller.GetButtonState(profiles[0].bindings[PROFILE_BUTTON_2])) {
    Serial.print("Using Profile: ");
    Serial.println((1 + page * 6));
    curProfile = 1 + page * 6;
    EEPROM.update(CURRENT_PROFILE_ADDR, curProfile);
  }

  if (controller.GetButtonState(profiles[0].bindings[PROFILE_BUTTON_3])) {
    Serial.print("Using Profile: ");
    Serial.println((2 + page * 6));
    curProfile = 2 + page * 6;
    EEPROM.update(CURRENT_PROFILE_ADDR, curProfile);
  }

  if (controller.GetButtonState(profiles[0].bindings[PROFILE_BUTTON_4])) {
    Serial.print("Using Profile: ");
    Serial.println((3 + page * 6));
    curProfile = 3 + page * 6;
    EEPROM.update(CURRENT_PROFILE_ADDR, curProfile);
  }
  // if(ddrd & DDRD_5) return curProfile = 0;
  // if(ddrc & DDRC_6) return curProfile = 0;
}
