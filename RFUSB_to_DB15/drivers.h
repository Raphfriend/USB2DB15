//
// Created by Kitsune on 8/22/2020.
//

#ifndef USB2DB15_DRIVERS_H
#define USB2DB15_DRIVERS_H

#include "HIDController.h"

// Please Keep the IDs in Alphabetical order to help
// other find the keys thank you

/****************
 * Vendor IDs
 ****************/

#define VID_8BITDO              0x2D08 // 8BitDo
#define VID_BROOK               0x0C12 // Brook
#define VID_BUFFALO             0x0583 // Buffalo
#define VID_HORI                0x0F0D // HORI
#define VID_MADCATZ             0x0738 // Mad Catz
#define VID_RAZER               0x1532 // Razer
#define VID_RETROBIT            0x0079 // Retrobit
#define VID_SONY                0x054c // Sony
#define VID_UPCB                0x04D8 // Universal PCB Project

/****************
 * Product IDs
 ****************/

#define PID_8BITDO_M30          0x5006 // 8BitDo M30 wired Controller
#define PID_BUFFALO_CLASSIC     0x2060 // iBUFFALO SNES CLASSIC USB GAMEPAD
#define PID_BROOK_UNIVERSAL     0x0C30 // Brook Universal Fighting PCB
#define PID_HORI_CMDR           0x0085 // HORI Fighting Commander
#define PID_HORI_HORIPAD        0x0066 // HORIPAD FPS+(PS4)
#define PID_HORI_LITE           0x00EE // HORI ワイヤードコントローラライト for PS4-102
#define PID_HORI_RAP_PS3        0x0011 // HORI Real Arcade Pro.3 SA PS3コントローラ
#define PID_HORI_RAP_PREMIUM    0x0026 // HORI Real Arcade リアルアーケードPro.3 Premium VLX
#define PID_HORI_RAP_V_PS3      0x008B // HORI RAP V HAYABUSA Controller(PS3)
#define PID_HORI_RAP_V_PS4      0x008A // HORI RAP V HAYABUSA Controller(PS4)
#define PID_MADCATZ_SF_PS3      0x8838 // Mad Catz Street Fighter IV Tournament Edition (PS3)
#define PID_RAZER_PANTHERA      0x0401 // Razer Panthera
#define PID_RETROBIT_SATURN     0x0011 // Retrobit Sega Saturn Wireless
#define PID_SONY_PS4_JP         0x09CC // PS4 Controller JP region
#define PID_SONY_PS4_NA         0x05C4 // PS4 Controller NA region
#define PID_SONY_PS4_ADAPTER    0x0BA0 // PS4 Wireless Adapter
#define PID_UPCB                0x1529 // Universal PCB Project

/****************
 * D Pad Constants
 ****************/

#define DPAD_UP         0
#define DPAD_UP_RIGHT   1
#define DPAD_RIGHT      2
#define DPAD_DOWN_RIGHT 3
#define DPAD_DOWN       4
#define DPAD_DOWN_LEFT  5
#define DPAD_LEFT       6
#define DPAD_UP_LEFT    7

#define DPAD_MASK       0x0F

void setupController(uint16_t vid, uint16_t pid, HIDController *controller);

// 8BitDo Controllers
void setup8BitDoM30(HIDController *controller);

// Buffalo Controllers
void setupBuffaloClassic(HIDController *controller);

// Hori Controllers
void setupHoriFightingCmdr(HIDController *controller);
void setupHoriRAP3(HIDController *controller);

// Sony
void setupPS4(HIDController *controller);

#endif //USB2DB15_DRIVERS_H
