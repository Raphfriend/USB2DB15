//
// Created by Kitsune on 8/26/2020.
//

#ifndef USB2DB15_EEPROMMANAGER_H
#define USB2DB15_EEPROMMANAGER_H

#include "Profile.h"

#define EEPROM_BYTE_0 'U'
#define EEPROM_BYTE_1 '2'
#define EEPROM_BYTE_2 'N'
#define EEPROM_VERISON 2

/**
 * EEPROM Layout
 * bytes    Description
 * 0-2      'U2N' in ascii.  This tells us if the EEPROM has been initialized
 * 3        Version number in binary. Tells us what EEPROM layout we are using
 * 4-5      Reserved for future use
 * 6        Next Profile Block to use
 * 7-15     Reserved for future use
 * 16-735   Profiles (10 Profile Blocks at 72 bytes each)
 */

/**
 * Profile Block
 * Byte     Description
 * 0-1      VID
 * 2-3      PID
 * 4        Current Profile
 * 5-7      Reserved
 * 8-71     Profile Data
 */

/**
 * Profile Layout  (Byte Address is the same as the Profile IDs in profile.h
 * Byte     Description
 * 0        UP
 * 1        RIGHT
 * 2        DOWN
 * 3        LEFT
 * 4        START
 * 5        COIN/SELECT
 * 6        BUTTON 1
 * 7        BUTTON 2
 * 8        BUTTON 3
 * 9        BUTTON 4
 * 10       BUTTON 5
 * 11       BUTTON 6
 * 12-15    Reserved reserved for future use
 */

// Address Locations

#define NEXT_PROFILE_BLOCK_ADDR 6
#define PROFILE_START_ADDR 16
#define PROFILE_BLOCK_HEADER_SIZE 8
#define CURRENT_PROFILE_ADDR 4
#define PROFILE_SIZE 72
#define MAX_PROFILES 4
#define MAX_PROFILE_BLOCKS 10

class EepromManager {
  uint8_t cur_profile_block;
public:
  EepromManager();
  void Initialize();

  void LoadProfile(uint8_t num, Profile &profile);
  void SaveProfile(uint8_t num, Profile &profile);

  uint8_t LoadCurrentProfile();
  void SaveCurrentProfile(uint8_t num);

  void SetCurrentController(uint16_t vid, uint16_t pid);
protected:
  void InitializeProfileBlocks();

  void GenerateInitialProfiles();
  void GenerateDefaultProfile(Profile &profile);
  void SwapProfileRows(Profile &profile);

  uint16_t GetProfileStartAddr();
  uint16_t GetProfileBlockStartAddr();

  uint16_t Read16Bit(uint16_t addr);
  void Write16Bit(uint16_t addr, uint16_t value);
};


#endif //USB2DB15_EEPROMMANAGER_H
