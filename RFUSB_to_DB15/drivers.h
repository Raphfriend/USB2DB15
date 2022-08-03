//
// Created by Kitsune on 8/22/2020.
//

#ifndef USB2DB15_DRIVERS_H
#define USB2DB15_DRIVERS_H

#include "HIDController.h"

// Please Keep the IDs in Alphabetical order to help
// others find the keys thank you

/****************
 * Vendor IDs
 ****************/

#define VID_8BITDO              0x2DC8 // 8BitDo Bluetooth (D-INPUT MODE)
#define VID_BROOK               0x0C12 // Brook
#define VID_BUFFALO             0x0583 // Buffalo
#define VID_DAEMON              0x2341 // DaemonBite Retro Controllers (Standard Arduino Leonardo VID)
#define VID_FIRE                0x1292 // Fire International Ltd.
#define VID_GENERIC             0x0079 // Generic
#define VID_HONCAM              0x20D6 // Honcam
#define VID_HORI                0x0F0D // HORI (Also used by Retrobit 2.4GHz)
#define VID_LOGITECH            0x046D // Logitech
#define VID_MADCATZ             0x0738 // Mad Catz
#define VID_NACON               0x146B // Nacon
#define VID_QANBA               0x2C22 // Qanba
#define VID_RAZER               0x1532 // Razer
#define VID_SEGATOYS            0x0CA3 // Sega Toys
#define VID_SHANWAN             0x2563 // Shenzhen ShanWan Technology Co., Ltd.
#define VID_SONY                0x054c // Sony
#define VID_UPCB                0x04D8 // Universal PCB Project


/****************
 * Product IDs
 ****************/

#define PID_8BITDO_M30          0x5006 // 8BitDo M30 Bluetooth Wired Controller (D-INPUT MODE)
#define PID_8BITDO_SFC30        0xAB21 // 8BitDo SFC30 Wired Controller
#define PID_8BITDO_SNES_BT      0x3101 // 8BitDo Snes BT adapter
#define PID_BUFFALO_CLASSIC     0x2060 // iBUFFALO SNES CLASSIC USB GAMEPAD
#define PID_BROOK_UNIVERSAL     0x0C30 // Brook Universal Fighting PCB
#define PID_BROOK_FB            0x0ef7 // FIGHTING BOARD PS3/PS4 (PS4 Mode)
#define PID_DAEMON_SNES         0x8036 // DaemonBite Retro Controllers SNES (Standard Arduino Leonardo PID)
#define PID_DAEMON_SATURN       0x8030 // DaemonBite Retro Controllers Saturn (Modded PID to suit the USB2DB15, check COMPATIBILITY.md)
#define PID_FEIR_PS4            0x1E1B // Feir Wired FR-225C Controller for PlayStation 4
#define PID_FIRE_NEOGEOX_AS     0x4e47 // Fire NEOGEOX Arcade Stick
#define PID_FUSION_PS4          0x792A // PowerA FUSION Wired FightPad Gaming Controller (PS4)
#define PID_GENERIC_SNES        0x0011 // Generic SNES pad
#define PID_GENERIC_ZERO_DELAY  0x0006 // Generic Zero Delay PCB 01
#define PID_HONCAM              0xA713 // Honcam HC-J2003
#define PID_HORI_CMDR           0x0085 // HORI Fighting Commander
#define PID_HORI_FS3            0x0010 // HORI Fighting Stick 3
#define PID_HORI_FSV3           0x0027 // HORI Fighting Stick V3
#define PID_HORI_HORIPAD        0x0066 // HORIPAD FPS+(PS4)
#define PID_HORI_LITE           0x00EE // HORI ワイヤードコントローラライト for PS4-102
#define PID_HORI_RAP_PREMIUM    0x0026 // HORI Real Arcade リアルアーケードPro.3 Premium VLX
#define PID_HORI_RAP_PS3        0x0011 // HORI Real Arcade Pro.3 SA PS3コントローラ
#define PID_HORI_RAP_V_PS3      0x008B // HORI RAP V HAYABUSA Controller(PS3)
#define PID_HORI_RAP_V_PS4      0x008A // HORI RAP V HAYABUSA Controller(PS4)
#define PID_LOGITECH            0xC216 // Logitech F310
#define PID_MADCATZ_FSA_PS4     0x8180 // Mad Catz Fight Stick Alpha (PS4)
#define PID_MADCATZ_FSA_PS3     0x3180 // Mad Catz Fight Stick Alpha (PS3)
#define PID_MADCATZ_SF_PS3_RND1 0x8818 // Mad Catz Street Fighter IV Tournament Edition Round 1 (PS3)
#define PID_MADCATZ_SF_PS3_RND2 0x8838 // Mad Catz Street Fighter IV Tournament Edition Round 2 (PS3)
#define PID_MADCATZ_TE2P_PS4    0x8481 // Mad Catz SFV Arcade FightStick TE2+ (PS4 Mode)
#define PID_MADCATZ_TESP_PS4    0x8384 // Mad Catz SFV Arcade FightStick TES+ (PS4 Mode)
#define PID_NACON_DAIJA_PS3     0x0904 // Nacon Daija (PS3)
#define PID_NACON_DAIJA_PS4     0x0D09 // Nacon Daija (PS4)
#define PID_NEOGEO_MINI_PAD     0x0575 // Neo Geo Mini Pad
#define PID_QANBA_CRYSTAL_PS4   0x2200 // Qanba Crystal (PS4)
#define PID_QANBA_OBSIDIAN_PS3  0x2302 // Qanba Obsidian (PS3)
#define PID_QANBA_OBSIDIAN_PS4  0x2300 // Qanba Obsidian (PS4)
#define PID_RAZER_PANTHERA      0x0401 // Razer Panthera (PS4)
#define PID_RAZER_PANTHERA_EVO  0x1008 // Razer Panthera EVO (PS4)
#define PID_RAZER_RAIJU_ULT     0x1004 // Razer Raiju Ultimate (PS4)
#define PID_RETROBIT_SATURN     0x00C1 // Retrobit Sega Saturn Wireless 2.4G
#define PID_SEGA_ACS_1002       0x0027 // Control Pad for Astro City Mini - ACS-1002
#define PID_SEGA_ACS_1003       0x0028 // Arcade stick for Astro City Mini - ACS-1003
#define PID_SONY_PS4_ADAPTER    0x0BA0 // PS4 Wireless Adapter
#define PID_SONY_PS4_JP         0x09CC // PS4 Controller JP region
#define PID_SONY_PS4_NA         0x05C4 // PS4 Controller NA region
#define PID_SONY_PS5_NA         0x0CE6 // PS5 Controller NA region
#define PID_SONY_PSC	          0x0CDA // Playstation Classic Controller
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
void setup8BitDoSFC30(HIDController *controller);

// Buffalo Controllers
void setupBuffaloClassic(HIDController *controller);

// Generic Controllers
void setupGenericSNES(HIDController *controller);
void setupGenericZeroDelay(HIDController *controller);

// Daemon SNES
void setupDaemonSNES(HIDController *controller);

// Daemon Saturn
void setupDaemonSaturn(HIDController *controller);

// Fire
void setupFireNEOGEOXAS(HIDController *controller);

// Honcam Controllers
void setupHoncam(HIDController *controller);

// Hori Controllers
void setupHoriFightingCmdr(HIDController *controller);
void setupHoriRAP3(HIDController *controller);

// Logitech

void setupLogitech(HIDController *controller);

// MadCatz 
void setupMadCatzTE2Plus(HIDController *controller);

// NeoGeo
void setupNeoGeoMini(HIDController *controller);

// Razer 
void setupRJU(HIDController *controller);

// Retrobit Controllers
void setupRetrobit_Saturn(HIDController *controller);

// Sega
void setupSegaAstroCityMini(HIDController *controller);

// Sony
void setupPS4(HIDController *controller);
void setupPS5(HIDController *controller);
void setupPSC(HIDController *controller);

#endif //USB2DB15_DRIVERS_H
