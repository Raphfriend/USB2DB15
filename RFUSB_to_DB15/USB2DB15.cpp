//
// Created by Kitsune on 8/21/2020.
//

#include "USB2DB15.h"
#include "LED.h"

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
USB2DB15::USB2DB15(PS3Controller &ps3, XBoxOneController &xbox, XBoxUSBController &xbox360, HIDController &hid, EepromManager &eeprom) :
  ps3(ps3), xbox(xbox), xbox360(xbox360), hid(hid), eeprom(eeprom) { };

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
  Controller *controller = GetActiveController();

  if(!controller) return; // If there isn't a controller we are done here

  // Setup Profile
  if(controller->GetVid() != cur_vid || controller->GetPid() != cur_pid) {
    eeprom.SetCurrentController(controller->GetVid(), controller->GetPid());
    eeprom.LoadProfile(eeprom.LoadCurrentProfile(), profile);
    cur_vid = controller->GetVid();
    cur_pid = controller->GetPid();
  }

  ddrc = GetDDRC(*controller);
  ddrd = GetDDRD(*controller);

  // Handle SELECT/COIN held presses
  if (ddrd == DDRD_COIN) { // If only COIN is being held
    if(prevDDRD != DDRD_COIN) {  // If COIN is newly the only button pressed
      select_press_time = millis();
    } else if ((millis() - select_press_time) >= SELECT_HOLD_DURATION && input_mode != PROFILE_BIND_MODE) {
      Serial.println("Bind Mode");
      input_mode = PROFILE_BIND_MODE;
      cur_key = PROFILE_BUTTON_1;
      LED::On();
    }
  }

  if (input_mode == PROFILE_BIND_MODE) {
    HandleProfileBindMode(ddrc, ddrd, *controller);
  } else { // NORMAL_MODE or any unknown mode
    HandleNormalMode(ddrc, ddrd, *controller);
  }

  prevDDRC = ddrc;
  prevDDRD = ddrd;
}

void USB2DB15::FactoryReset() {
  eeprom.Initialize();
  cur_pid = 0;
  cur_vid = 0;
}

Controller* USB2DB15::GetActiveController() {
  if (ps3.Connected()) { // PS3
    return &ps3;
  } else if (xbox.Connected()) { // XboxOne
    return &xbox;
  } else if (xbox360.Connected()) { // Xbox360
    return &xbox360;    
  } else if (hid.Connected()) { // Generic HID Controller
    return &hid;
  }
  return NULL;
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
uint8_t USB2DB15::GetDDRC(Controller &controller) {
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
uint8_t USB2DB15::GetDDRD(Controller &controller) {
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
 * Handles Input in the Normal Mode Context
 *
 * This mode will check for profile change commands and update the DB15 pins
 *
 * @param ddrc the data to send to DDRC
 * @param ddrd the data to send to DDRD
 * @param controller The controller pull raw input from
 */
void USB2DB15::HandleNormalMode(uint8_t ddrc, uint8_t ddrd, Controller &controller) {
  if (ddrc == prevDDRC && ddrd == prevDDRD) return;
  // Check Special Button Combinations
  // Change Profile  SELECT + DIRECTION
  if (ddrd & DDRD_COIN) {
    for(uint8_t i = 0; i < 4; i++) {
      if(controller.GetButtonState(profile.bindings[PROFILE_BUTTON_UP + i])) {
        SetProfile(controller, i);
        return;
      }
    }
  }
  // Output
  DDRC = ddrc;
  DDRD = ddrd;

  debugOutput(ddrc, ddrd);
}

/**
 * Handles Inpute in the Profile Rebinding Context
 *
 * Handles setting up User Defined Profiles.
 *
 * @param ddrc the data to send to DDRC
 * @param ddrd the data to send to DDRD
 * @param controller The controller pull raw input from
 */
void USB2DB15::HandleProfileBindMode(uint8_t ddrc, uint8_t ddrd, Controller &controller) {
  // End Mode if SELECT/COIN is released
  if(!(ddrd & DDRD_COIN)) {
    Serial.println("Normal Mode");
    eeprom.SaveProfile(eeprom.LoadCurrentProfile(), profile);
    input_mode = NORMAL_MODE;
    LED::Off();
    return;
  }

  // Ignore any keys after we have bound all 6 buttons
  if(cur_key >= NUM_BUTTON_BINDINGS) return;

  // Check if any raw inputs have been clicked
  for(uint8_t i = BUTTON_1; i <= BUTTON_10; i++) {
    if(controller.GetButtonClick(i)) {
      profile.bindings[cur_key] = i;
      cur_key++;
      return;
    }
  }
}

/**
 * Sets the current Profile
 *
 * Reads the controller input as mapped by the default profile and uses that
 * to decide which profile to use.
 *
 * @param controller The controller to read from
 */
void USB2DB15::SetProfile(Controller &controller, uint8_t profile_num) {
  if(profile_num > MAX_PROFILES) return;

  Serial.print("Using Profile: ");
  Serial.println(profile_num);
  eeprom.SaveCurrentProfile(profile_num);
  eeprom.LoadProfile(profile_num, profile);
  LED::Blink(profile_num + 1, LED::BLINK_RATE_PROFILE);
}
