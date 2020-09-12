//
// Created by Kitsune on 8/21/2020.
//
#ifndef USB2DB15_USB2DB15_H
#define USB2DB15_USB2DB15_H

#include <stdint.h>

#include "controller.h"
#include "device_descriptor.h"
#include "EepromManager.h"
#include "Profile.h"
#include "PS3Controller.h"
#include "XBoxOneController.h"
#include "HIDController.h"

// Output Constants
/* DDRC */
#define DDRC_3      1   //A0
#define DDRC_1      2   //A1
#define DDRC_LEFT   4   //A2
#define DDRC_UP     8   //A3
#define DDRC_6      16  //A4
#define DDRC_START  32  //A5

/* DDRD */
#define DDRD_DOWN   4   //D2
#define DDRD_RIGHT  8   //D3
#define DDRD_2      16  //D4
#define DDRD_4      32  //D5
#define DDRD_COIN   64  //D6
#define DDRD_5      128 //D7

/* Durations */
#define SELECT_HOLD_DURATION 3000

/* Input Modes */
#define NORMAL_MODE 0
#define PROFILE_BIND_MODE 1

/**
 * USB To DB15
 *
 * Class which controls the mapping for input from USB controllers to
 * the DB15 output. It contains support for Profiles as well as trapping
 * combinations of inputs to allow control of the adapter.
 */
class USB2DB15 {
    PS3Controller &ps3;
    XBoxOneController &xbox;
    HIDController &hid;
    EepromManager &eeprom;
    Profile profile;
    uint8_t input_mode = NORMAL_MODE;

    // Controller
    uint16_t cur_vid, cur_pid;

    // Output
    uint8_t prevDDRC = 0;
    uint8_t prevDDRD = 0;

    // Binding Mode
    uint8_t cur_key = 0;
    unsigned long select_press_time = 0;

  public:
    USB2DB15(PS3Controller &ps3, XBoxOneController &xbox, HIDController &hid, EepromManager &eeprom);
    void GenerateOutput();
    void FactoryReset();

  protected:
    Controller* GetActiveController();
    uint8_t GetDDRC(Controller &controller);
    uint8_t GetDDRD(Controller &controller);
    void SetProfile(Controller &controller, uint8_t profile_num);
    void HandleNormalMode(uint8_t ddrc, uint8_t ddrd, Controller &controller);
    void HandleProfileBindMode(uint8_t ddrc, uint8_t ddrd, Controller &controller);
};


#endif //USB2DB15_USB2DB15_H
