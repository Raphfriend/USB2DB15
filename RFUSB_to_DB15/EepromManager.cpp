//
// Created by Kitsune on 8/26/2020.
//
#include <EEPROM.h>

#include "device_descriptor.h"
#include "EepromManager.h"

EepromManager::EepromManager() {
  if(EEPROM.read(0) != EEPROM_BYTE_0 ||
     EEPROM.read(1) != EEPROM_BYTE_1 ||
     EEPROM.read(2) != EEPROM_BYTE_2) {
    Initialize();
  }

  // We may want to attempt to migrate the data so new versions in the future so I have separated this check out
  if(EEPROM.read(3) != EEPROM_VERISON) {
    Initialize();
  }
}

/**
 * Initialize the EEPROM with default values
 */
void EepromManager::Initialize() {
  // Populate initial profiles
  GenerateInitialProfiles();

  //Create the header last only after everything completes
  EEPROM.update(0, EEPROM_BYTE_0);
  EEPROM.update(1, EEPROM_BYTE_1);
  EEPROM.update(2, EEPROM_BYTE_2);
  EEPROM.update(3, EEPROM_VERISON);
}

/**
 * Loads the current profile number from EEPROM
 * @return The profile number
 */
uint8_t EepromManager::LoadCurrentProfile() {
  return EEPROM.read(CURRENT_PROFILE_ADDR);
}

/**
 * Saves the current profile number to EEPROM
 * @param num The Current Profile Number
 */
void EepromManager::SaveCurrentProfile(uint8_t num) {
  EEPROM.update(CURRENT_PROFILE_ADDR, num);
}

/**
 * Loads the specified profile from EEPROM into the given profile object
 * @param num The number of the profile to load
 * @param profile The profile object to load the profile into
 */
void EepromManager::LoadProfile(uint8_t num, Profile &profile) {
  int start_addr = PROFILE_START_ADDR + (num * PROFILE_SIZE);
  if (num >= MAX_PROFILES) return;

  for(uint8_t i;i < NUM_BUTTON_BINDINGS; i++) {
    profile.bindings[i] = EEPROM.read(start_addr + i);
  }
}

void EepromManager::SaveProfile(uint8_t num, Profile &profile) {
  int start_addr = PROFILE_START_ADDR + (num * PROFILE_SIZE);
  if (num >= MAX_PROFILES) return;

  for(uint8_t i;i < NUM_BUTTON_BINDINGS; i++) {
    EEPROM.update(start_addr + i, profile.bindings[i]);
  }
}

void EepromManager::GenerateInitialProfiles() {
  Profile profile;

  // Profile 0 is the default profile
  GenerateDefaultProfile(profile);
  SaveProfile(0, profile);

  // Profile 1 is the default profile row swapped
  SwapProfileRows(profile);
  SaveProfile(1, profile);

  // Profile 2 is the Default profile but with L1 instead of R2
  GenerateDefaultProfile(profile);
  profile.SetButton(BUTTON_7, PROFILE_BUTTON_6);
  SaveProfile(2, profile);

  // Profile 3 is Profile 2 with the rows swapped
  SwapProfileRows(profile);
  SaveProfile(3, profile);

  // Profile 4 - n haven't been decided yet and are default
  GenerateDefaultProfile(profile);
  for(uint8_t i = 4; i < MAX_PROFILES; i++) {
    SaveProfile(i, profile);
  }
}

/**
 * Creates a Default profile.
 * @param profile The profile slot to store the profile in
 */
void EepromManager::GenerateDefaultProfile(Profile &profile) {
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
 * Swaps a profiles top and bottom rows
 *
 * It accomplishes this swap by swapping buttons 1 with 4, 2 with 5,
 * and 3 with 6
 *
 * @param profile The profile slot to store the profile in
 * @param base The profile slot to copy from
 */
void EepromManager::SwapProfileRows(Profile &profile) {
  profile.SwapButtons(PROFILE_BUTTON_1, PROFILE_BUTTON_4);
  profile.SwapButtons(PROFILE_BUTTON_2, PROFILE_BUTTON_5);
  profile.SwapButtons(PROFILE_BUTTON_3, PROFILE_BUTTON_6);
}
