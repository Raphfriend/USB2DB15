#ifndef USB2DB15_ANALOG_MAPPINGS_H
#define USB2DB15_ANALOG_MAPPINGS_H

#include "HIDController.h"

// Register only layouts measured with AnalogMapper. Offsets include the report ID.
// Called after setupController so D-pad and button mappings remain configured.
inline void setupAnalogMapping(uint16_t vid, uint16_t pid, HIDController *controller) {
  // Sony DualShock 4 and DualSense USB reports use bytes 1 and 2 for the
  // left analog stick. Byte 0 is report ID 0x01.
  if (vid == 0x054C &&
      (pid == 0x05C4 || pid == 0x09CC || pid == 0x0BA0 || pid == 0x0CE6)) {
    controller->ConfigAnalogStick(1, 2, 96, 159, 96, 159, 64, 1);
  }
}

#endif
