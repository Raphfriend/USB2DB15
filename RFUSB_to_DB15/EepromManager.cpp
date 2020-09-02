//
// Created by Kitsune on 8/26/2020.
//
#include <EEPROM.h>
#include <SPI.h>

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
  //Initialize();
}

/**
 * Initialize the EEPROM with default values
 */
void EepromManager::Initialize() {
  // Populate initial profiles
  InitializeProfileBlocks();

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
  return EEPROM.read(GetProfileBlockStartAddr() + CURRENT_PROFILE_ADDR);
}

/**
 * Saves the current profile number to EEPROM
 * @param num The Current Profile Number
 */
void EepromManager::SaveCurrentProfile(uint8_t num) {
  EEPROM.update(GetProfileBlockStartAddr() + CURRENT_PROFILE_ADDR, num);
}

/**
 * Loads the specified profile from EEPROM into the given profile object
 * @param num The number of the profile to load
 * @param profile The profile object to load the profile into
 */
void EepromManager::LoadProfile(uint8_t num, Profile &profile) {
  int start_addr = GetProfileStartAddr() + (num * PROFILE_SIZE);
  if (num >= MAX_PROFILES) return;

  for(uint8_t i;i < NUM_BUTTON_BINDINGS; i++) {
    profile.bindings[i] = EEPROM.read(start_addr + i);
  }
}

void EepromManager::SaveProfile(uint8_t num, Profile &profile) {
  int start_addr = GetProfileStartAddr() + (num * PROFILE_SIZE);
  if (num >= MAX_PROFILES) return;

  for(uint8_t i;i < NUM_BUTTON_BINDINGS; i++) {
    EEPROM.update(start_addr + i, profile.bindings[i]);
  }
}

void EepromManager::SetCurrentController(uint16_t vid, uint16_t pid) {
  uint8_t found = 0;
  uint16_t addr = PROFILE_START_ADDR;

  for(uint8_t i = 0; i < MAX_PROFILE_BLOCKS; i++) {
    if(vid == Read16Bit(addr) && pid == Read16Bit(addr + 2)) {
      Serial.print("Found Controller.  Using Profile Block: ");
      Serial.println(i);
      found = 1;
      cur_profile_block = i;
      break;
    }
    addr += PROFILE_SIZE;
  }
  if(!found) {
    cur_profile_block = EEPROM.read(NEXT_PROFILE_BLOCK_ADDR);
    Serial.print("Could Not find Controller. Generating Profile Block:");
    Serial.println(cur_profile_block);
    // Write the vid and pid to EEPROM
    Write16Bit(GetProfileBlockStartAddr(), vid);
    Write16Bit(GetProfileBlockStartAddr() + 2, pid);
    // Set the current profile to 0
    EEPROM.update(GetProfileBlockStartAddr() + 4, 0);
    // Generate Default profiles
    GenerateInitialProfiles();
    // Update Next Profile Block.  Mod block by 10 to loop around
    EEPROM.update(NEXT_PROFILE_BLOCK_ADDR, (cur_profile_block + 1) % 10);
  }
}

/**
 * Calculates the start address for profiles in the current profile block
 * @return The Address of byte 0 of profile 0
 */
inline uint16_t EepromManager::GetProfileStartAddr() {
  return GetProfileBlockStartAddr() + PROFILE_BLOCK_HEADER_SIZE;
}

/**
 * Calculates the Address of byte 0 of the current profile block
 * @return The address of byte 0 of the current profile block
 */
inline uint16_t EepromManager::GetProfileBlockStartAddr() {
  return PROFILE_START_ADDR + (cur_profile_block * PROFILE_SIZE);
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
}

void EepromManager::InitializeProfileBlocks() {
  uint16_t addr = PROFILE_START_ADDR;
  for(int i = 0; i < MAX_PROFILE_BLOCKS; i++) {
    // Set the VID, PID and Current profile of the Profile Block to Zero
    EEPROM.update(addr, 0);
    EEPROM.update(addr + 1, 0);
    EEPROM.update(addr + 2, 0);
    EEPROM.update(addr + 3, 0);
    EEPROM.update(addr + 4, 0);
    addr += PROFILE_SIZE;
  }
}

uint16_t EepromManager::Read16Bit(uint16_t addr) {
  uint16_t value;
  value = (EEPROM.read(addr + 1) << 8);
  value |= EEPROM.read(addr);
  return value;
}

void EepromManager::Write16Bit(uint16_t addr, uint16_t value) {
  EEPROM.update(addr, value);
  EEPROM.update(addr + 1, value >> 8);
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
