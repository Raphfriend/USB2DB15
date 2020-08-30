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
 * 4-5      Reserved
 * 6        Current Profile Number
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
