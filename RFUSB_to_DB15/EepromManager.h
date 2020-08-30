//
// Created by Kitsune on 8/26/2020.
//

#ifndef USB2DB15_EEPROMMANAGER_H
#define USB2DB15_EEPROMMANAGER_H

#include "Profile.h"

#define EEPROM_BYTE_0 'U'
#define EEPROM_BYTE_1 '2'
#define EEPROM_BYTE_2 'N'
#define EEPROM_VERISON 1

/**
 * EEPROM Layout
 * bytes    Description
 * 0-2      'U2N' in ascii.  This tells us if the EEPROM has been initialized
 * 3        Version number in binary. Tells us what EEPROM layout we are using
 * 4-5      Reserved for future use
 * 6        Current Profile Number
 * 7        Current Number of Custom Device Descriptors
 * 8-19     Reserved for future use
 * 20-199   Profiles (10 profiles at 18 bytes each = 180)
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

#define CURRENT_PROFILE_ADDR 6
#define PROFILE_START_ADDR 20
#define PROFILE_SIZE 18
#define MAX_PROFILES 10

class EepromManager {
public:
  EepromManager();
  void Initialize();

  void LoadProfile(uint8_t num, Profile &profile);
  void SaveProfile(uint8_t num, Profile &profile);

  uint8_t LoadCurrentProfile();
  void SaveCurrentProfile(uint8_t num);

protected:
  void GenerateInitialProfiles();
  void GenerateDefaultProfile(Profile &profile);
  void SwapProfileRows(Profile &profile);
};


#endif //USB2DB15_EEPROMMANAGER_H
