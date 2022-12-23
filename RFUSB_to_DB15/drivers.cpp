//
// Created by Kitsune on 8/23/2020.
//

#include "drivers.h"
#include "debug.h"

/**
 * Looks up a controller based on VID and PID and configures a HIDController to
 * work with it.
 *
 * This function takes in a VID and PID as well as a controller to configure.
 * It will look up the exact controller and invoke the proper setup function to
 * configure it. If it cannot find the controller it defaults to using the
 * HoriRAP3 setup.
 *
 * When adding new controllers to this make sure to define the VID and PID in
 * "drivers.h". If an existing setup function works completely for the
 * controller then you should use it, but if they differ even a little bit
 * please create a new function instead of adding logic to an existing setup
 * function.
 *
 * Vendor and Product IDs are defined in drivers.h
 *
 * @param vid The Vendor ID for the controller.
 * @param pid The Product ID for the controller.
 * @param controller A pointer to the HIDController that you want to configure.
 */

void setupController(uint16_t vid, uint16_t pid, HIDController *controller) {
  switch (vid) {

    case VID_8BITDO:
      switch (pid) {
       case PID_8BITDO_M30:
          setup8BitDoM30(controller);
          break;

       case PID_8BITDO_SFC30:
          setup8BitDoSFC30(controller);
          break;
             
      }
      break;

    case VID_BUFFALO:
      if (pid == PID_BUFFALO_CLASSIC) setupBuffaloClassic(controller);
      break;

    case VID_BROOK:
      if ((pid == PID_BROOK_UNIVERSAL) || (pid == PID_BROOK_FB)) setupPS4(controller);
      if (pid == PID_FEIR_PS4) setupPS4(controller);
      break;

    case VID_FIRE:
      if (pid == PID_FIRE_NEOGEOX_AS) setupFireNEOGEOXAS(controller);
      break;

    case VID_GENERIC:
      if (pid == PID_GENERIC_SNES) setupGenericSNES(controller);
      if (pid == PID_GENERIC_ZERO_DELAY) setupGenericZeroDelay(controller);
      break;

    case VID_DAEMON:
      switch(pid) {
        case PID_DAEMON_SNES:
          setupDaemonSNES(controller);
          break;
        case PID_DAEMON_SATURN:
          setupDaemonSaturn(controller);
          break;
        case PID_DAEMON_MD:
          setupDaemonMD(controller);
          break;  
      }
      break;

    case VID_HONCAM:
      if (pid == PID_HONCAM) setupHoncam(controller);
      if (pid == PID_FUSION_PS4) setupPS4(controller);
      break;

    case VID_HORI:
      switch (pid) {
        case PID_RETROBIT_SATURN:
          setupRetrobit_Saturn(controller);
          break;
        case PID_HORI_CMDR:
          setupHoriFightingCmdr(controller);
          break;
        case PID_HORI_HORIPAD:
        case PID_HORI_LITE:
        case PID_HORI_RAP_V_PS4:
          setupPS4(controller);
          break;
        case PID_HORI_RAP_PS3:
        case PID_HORI_RAP_PREMIUM:
        case PID_HORI_RAP_V_PS3:
        case PID_HORI_FS3:
        case PID_HORI_FSV3:
        default:
          setupHoriRAP3(controller);
          break;
      }
      break;
		  
	case VID_LOGITECH:
      if (pid == PID_LOGITECH) setupLogitech(controller);
      break;

    case VID_MADCATZ:
      switch (pid) {
        case PID_MADCATZ_FSA_PS4:
        case PID_MADCATZ_TESP_PS4:
          setupPS4(controller);
          break;
        case PID_MADCATZ_TE2P_PS4:
          setupMadCatzTE2Plus(controller);
          break;
        case PID_MADCATZ_FSA_PS3:
        case PID_MADCATZ_SF_PS3_RND1:
        case PID_MADCATZ_SF_PS3_RND2:
        default:
          setupHoriRAP3(controller);
          break;
      }
      break;

    case VID_NACON:
      switch (pid) {
        case PID_NACON_DAIJA_PS4:
          setupPS4(controller);
          break;
        case PID_NACON_DAIJA_PS3:
        default:
          setupHoriRAP3(controller);
      }
      break;

    case VID_QANBA:
      switch (pid) {
        case PID_QANBA_CRYSTAL_PS4:
        case PID_QANBA_DRONE_PS4:
        case PID_QANBA_OBSIDIAN_PS4:
          setupPS4(controller);
          break;
        case PID_QANBA_OBSIDIAN_PS3:
        default:
          setupHoriRAP3(controller);
      }
      break;

    case VID_RAZER:
      switch (pid) {
        case PID_RAZER_PANTHERA:
        case PID_RAZER_PANTHERA_EVO:
          setupPS4(controller);
          break;
        case PID_RAZER_RAIJU_ULT:
          setupRJU(controller);
          break;
      }
      break;
		  
    case VID_SEGATOYS:
       switch (pid) {
        case PID_SEGA_ACS_1002:
        case PID_SEGA_ACS_1003:
          setupSegaAstroCityMini(controller);
          break;
      }

    case VID_SONY:
      switch (pid) { 
		    case PID_SONY_PSC:
          setupPSC(controller);
          break;      
        case PID_SONY_PS5_NA:
          setupPS5(controller);
          break;
        case PID_SONY_PS4_JP:
        case PID_SONY_PS4_NA:
        case PID_SONY_PS4_ADAPTER:
        default:
          setupPS4(controller);
          break;
      }
      break;

    case VID_SHANWAN:
      if (pid == PID_NEOGEO_MINI_PAD) setupNeoGeoMini(controller);
      break;

    case VID_UPCB:
      if (pid == PID_UPCB) setupPS4(controller);
      break;

    default:
		  
#ifndef RELEASE_MODE
      Serial.println("Failed to find a driver for device. Using RAP PS3 as fallback");
#endif
#ifndef REDUCED_SERIAL_OUTPUT
      Serial.print("VID: ");
      Serial.print(vid, HEX);
      Serial.print(" PID ");
      Serial.println(pid, HEX);
#endif
      setupHoriRAP3(controller);
      break;
  }
}

/*
/**************************
 * Utility functions
 **************************/

/**
 * Configures a D-Pad using the lower 4 bits of the provided byte
 *
 * This function only works for DPads who are in the lower 4 bits
 * of the byte. The mask is hardcoded to 0x0F because of this
 *
 * @param index The index of the byte where the dpad is located in the USB
 *              data packet.  See HIDUniversal#ParseHIDData in the Host
 *              Shield Library for the data packet.
 * @param controller The HIDController that will be configured
 */
void generateDPad(uint8_t index, HIDController *controller) {
  controller->ConfigButton(BUTTON_UP, index, DPAD_MASK, DPAD_UP);
  controller->ConfigButton(BUTTON_UP_RIGHT, index, DPAD_MASK, DPAD_UP_RIGHT);
  controller->ConfigButton(BUTTON_RIGHT, index, DPAD_MASK, DPAD_RIGHT);
  controller->ConfigButton(BUTTON_DOWN_RIGHT, index, DPAD_MASK, DPAD_DOWN_RIGHT);
  controller->ConfigButton(BUTTON_DOWN, index, DPAD_MASK, DPAD_DOWN);
  controller->ConfigButton(BUTTON_DOWN_LEFT, index, DPAD_MASK, DPAD_DOWN_LEFT);
  controller->ConfigButton(BUTTON_LEFT, index, DPAD_MASK, DPAD_LEFT);
  controller->ConfigButton(BUTTON_UP_LEFT, index, DPAD_MASK, DPAD_UP_LEFT);
}

/**************************
 * 8BitDo GamePads
 **************************/

/**
 * Configures an 8BitDo M30 Wired Controller and Compatible devices
 *
 * 8BitDo M30 Button layout
 * Byte 0x01   0x02   0x04   0x08   0x10   0x20   0x40   0x80
 * 0:   Btn 4, Btn 5, NA,    Btn 1, Btn 2, NA,    Btn 3, Btn 6
 * 1:   Btn 7, Btn 8, COIN,  START, NA,    NA,    NA,    NA
 * 2:   DPAD,  DPAD,  DPAD,  DPAD,  NA,    NA,    NA,    NA
 *
 * @param controller The HIDController that will be configured
 */
void setup8BitDoM30(HIDController *controller) {
  // DPad setup
  generateDPad(2, controller);
  // Button setup
  controller->ConfigButton(BUTTON_COIN, 1, 0x04);
  controller->ConfigButton(BUTTON_START, 1, 0x08);
  controller->ConfigButton(BUTTON_1, 0, 0x08);
  controller->ConfigButton(BUTTON_2, 0, 0x10);
  controller->ConfigButton(BUTTON_3, 0, 0x40);
  controller->ConfigButton(BUTTON_4, 0, 0x01);
  controller->ConfigButton(BUTTON_5, 0, 0x02);
  controller->ConfigButton(BUTTON_6, 0, 0x80);
  controller->ConfigButton(BUTTON_7, 1, 0x01);
  controller->ConfigButton(BUTTON_8, 1, 0x02);
}

/**
 * Configures an 8BitDo SFC30 Wired Controller and Compatible devices
 *
 * 8BitDo SFC30 Button layout
 * Byte 0x01   0x02   0x04   0x08   0x10   0x20   0x40   0x80
 * 0:                   LEFT(0x00)/RIGHT(0xFF)
 * 1:                   UP(0x00)/DOWN(0xFF)
 * 2-3:                          NA
 * 4:   NA,    NA,    NA,    NA,    Btn 5, Btn 4, NA,    Btn 2
 * 5:   Btn 1, NA,    Btn 6, Btn 3, NA,    NA,    COIN,  START
 *
 * @param controller The HIDController that will be configured
 */
void setup8BitDoSFC30(HIDController *controller) {
  // DPad setup
  controller->ConfigButton(BUTTON_LEFT, 0, 0xFF, 0);
  controller->ConfigButton(BUTTON_RIGHT, 0, 0xFF, 0xFF);
  controller->ConfigButton(BUTTON_UP, 1, 0xFF, 0);
  controller->ConfigButton(BUTTON_DOWN, 1, 0xFF, 0xFF);
  // Button setup
  controller->ConfigButton(BUTTON_COIN, 5, 0x40);
  controller->ConfigButton(BUTTON_START, 5, 0x80);
  controller->ConfigButton(BUTTON_1, 5, 0x01);
  controller->ConfigButton(BUTTON_2, 4, 0x80);
  controller->ConfigButton(BUTTON_3, 5, 0x08);
  controller->ConfigButton(BUTTON_4, 4, 0x20);
  controller->ConfigButton(BUTTON_5, 4, 0x10);
  controller->ConfigButton(BUTTON_6, 5, 0x04);
}

/**************************
 * Buffalo GamePads
 **************************/

/**
 * Configures an iBuffalo Classic USB GamePad and Compatible devices
 *
 * iBuffalo SNES Button layout
 * Byte 0x01   0x02   0x04   0x08   0x10   0x20   0x40   0x80
 * 0:                   LEFT(0x00)/RIGHT(0xFF)
 * 1:                   UP(0x00)/DOWN(0xFF)
 * 2:   Btn 5, Btn 4, Btn 2, Btn 1, Btn 6, Btn 3, COIN,  START
 *
 * @param controller The HIDController that will be configured
 */
void setupBuffaloClassic(HIDController *controller) {
  // DPad setup
  controller->ConfigButton(BUTTON_LEFT, 0, 0xFF, 0);
  controller->ConfigButton(BUTTON_RIGHT, 0, 0xFF, 0xFF);
  controller->ConfigButton(BUTTON_UP, 1, 0xFF, 0);
  controller->ConfigButton(BUTTON_DOWN, 1, 0xFF, 0xFF);
  // Button setup
  controller->ConfigButton(BUTTON_COIN, 2, 0x40);
  controller->ConfigButton(BUTTON_START, 2, 0x80);
  controller->ConfigButton(BUTTON_1, 2, 0x08);
  controller->ConfigButton(BUTTON_2, 2, 0x04);
  controller->ConfigButton(BUTTON_3, 2, 0x20);
  controller->ConfigButton(BUTTON_4, 2, 0x02);
  controller->ConfigButton(BUTTON_5, 2, 0x01);
  controller->ConfigButton(BUTTON_6, 2, 0x10);
}

/**************************
 * Fire GamePads
 **************************/

/**
 * Configures a Fire NEOGEOX Arcade Stick and Compatible devices
 *
 * NEOGEOX Arcade Stick Button layout
 * Byte 0x01   0x02   0x04   0x08   0x10   0x20   0x40   0x80
 * 0:   Btn 4, Btn 5, Btn 2, Btn 1, NA,    NA,    NA,    NA
 * 1:   COIN,  START, NA,    NA,    NA,    NA,    NA,    NA
 * 2:   DPAD,  DPAD,  DPAD,  DPAD,  NA,    NA,    NA,    NA
 *
 * @param controller The HIDController that will be configured
 */
void setupFireNEOGEOXAS(HIDController *controller) {
  // DPad setup
  generateDPad(2, controller);
  // Button setup
  controller->ConfigButton(BUTTON_COIN,  1, 0x01);
  controller->ConfigButton(BUTTON_START, 1, 0x02);
  controller->ConfigButton(BUTTON_1,  0, 0x08);
  controller->ConfigButton(BUTTON_2,  0, 0x04);
  controller->ConfigButton(BUTTON_4,  0, 0x01);
  controller->ConfigButton(BUTTON_5,  0, 0x02);
}

/**************************
 * Generic GamePads
 **************************/

/**
 * Configures a Generic SNES pad and Compatible devices
 *
 * Generic SNES Button layout
 * Byte 0x01   0x02   0x04   0x08   0x10   0x20   0x40   0x80
 * 0-2:                          NA
 * 3:                   LEFT(0x00)/RIGHT(0xFF)
 * 4:                   UP(0x00)/DOWN(0xFF)
 * 5:   NA,    NA,    NA,    NA,    Btn 2, Btn 5, Btn 4, Btn 1
 * 6:   Btn 6, Btn 3, NA,    NA,    COIN,  START, NA,    NA
 *
 * @param controller The HIDController that will be configured
 */
void setupGenericSNES(HIDController *controller) {
  // DPad setup
  controller->ConfigButton(BUTTON_LEFT, 3, 0xFF, 0);
  controller->ConfigButton(BUTTON_RIGHT, 3, 0xFF, 0xFF);
  controller->ConfigButton(BUTTON_UP, 4, 0xFF, 0);
  controller->ConfigButton(BUTTON_DOWN, 4, 0xFF, 0xFF);
  // Button setup
  controller->ConfigButton(BUTTON_COIN, 6, 0x10);
  controller->ConfigButton(BUTTON_START, 6, 0x20);
  controller->ConfigButton(BUTTON_1, 5, 0x80);
  controller->ConfigButton(BUTTON_2, 5, 0x10);
  controller->ConfigButton(BUTTON_3, 6, 0x02);
  controller->ConfigButton(BUTTON_4, 5, 0x40);
  controller->ConfigButton(BUTTON_5, 5, 0x20);
  controller->ConfigButton(BUTTON_6, 6, 0x01);
}

/**
 * Configures a Generic Zero Delay Board - 0x0006 PID
 *
 * Generic Generic Zero Delay Button layout
 * Byte 0x01   0x02   0x04   0x08   0x10   0x20   0x40   0x80
 * 0:                   LEFT(0x00)/RIGHT(0xFF)
 * 1:                   UP(0x00)/DOWN(0xFF)
 * 2-4:                          NA
 * 5:   NA,    NA,    NA,    NA,    Btn 1, Btn 2, Btn 5, Btn 6
 * 6:   Btn 3, NA,    Btn 4, NA,    START, NA,    COIN,  NA
 *
 * @param controller The HIDController that will be configured
 */
void setupGenericZeroDelay(HIDController *controller) {
  // DPad setup
  controller->ConfigButton(BUTTON_LEFT, 0, 0xFF, 0);
  controller->ConfigButton(BUTTON_RIGHT, 0, 0xFF, 0xFF);
  controller->ConfigButton(BUTTON_UP, 1, 0xFF, 0);
  controller->ConfigButton(BUTTON_DOWN, 1, 0xFF, 0xFF);
  // Button setup
  controller->ConfigButton(BUTTON_COIN, 6, 0x40);
  controller->ConfigButton(BUTTON_START, 6, 0x10);
  controller->ConfigButton(BUTTON_1, 5, 0x10);
  controller->ConfigButton(BUTTON_2, 5, 0x20);
  controller->ConfigButton(BUTTON_3, 6, 0x01);
  controller->ConfigButton(BUTTON_4, 6, 0x04);
  controller->ConfigButton(BUTTON_5, 5, 0x40);
  controller->ConfigButton(BUTTON_6, 5, 0x80);
}

/**************************
 * DaemonBite Retro Controllers
 **************************/

/**
 * Configures a Generic SNES pad and Compatible devices
 *
 * Generic SNES Button layout
 * Byte 0x01   0x02   0x04   0x08   0x10   0x20   0x40   0x80
 * 0-2:                          NA
 * 3:                   LEFT(0x00)/RIGHT(0xFF)
 * 4:                   UP(0x00)/DOWN(0xFF)
 * 5:   NA,    NA,    NA,    NA,    Btn 2, Btn 5, Btn 4, Btn 1
 * 6:   Btn 6, Btn 3, NA,    NA,    COIN,  START, NA,    NA
 *
 * @param controller The HIDController that will be configured
 */

void setupDaemonMD(HIDController *controller) {
  // DPad setup
  controller->ConfigButton(BUTTON_LEFT,  2, 0xFF, 0xFF);
  controller->ConfigButton(BUTTON_RIGHT, 2, 0xFF, 0x01);
  controller->ConfigButton(BUTTON_UP, 3, 0xFF, 0xFF);
  controller->ConfigButton(BUTTON_DOWN, 3, 0xFF, 0x01);
  // Button setup
  controller->ConfigButton(BUTTON_COIN, 0, 0x80); 
  controller->ConfigButton(BUTTON_START, 0, 0x40); 
  controller->ConfigButton(BUTTON_1, 0, 0x08);
  controller->ConfigButton(BUTTON_2, 0, 0x10);
  controller->ConfigButton(BUTTON_3, 0, 0x20);
  controller->ConfigButton(BUTTON_4, 0, 0x01);
  controller->ConfigButton(BUTTON_5, 0, 0x02);
  controller->ConfigButton(BUTTON_6, 0, 0x04);
  //controller->ConfigButton(BUTTON_7, 1, 0x01); home button

}

void setupDaemonSNES(HIDController *controller) {
  // DPad setup
  controller->ConfigButton(BUTTON_LEFT, 1, 0xFF, 0xFF);
  controller->ConfigButton(BUTTON_RIGHT, 1, 0xFF, 0x01);
  controller->ConfigButton(BUTTON_UP, 2, 0xFF, 0xFF);
  controller->ConfigButton(BUTTON_DOWN, 2, 0xFF, 0x01);
  // Button setup
  controller->ConfigButton(BUTTON_COIN, 0, 0x40);
  controller->ConfigButton(BUTTON_START, 0, 0x80);
  controller->ConfigButton(BUTTON_1, 0, 0x04);
  controller->ConfigButton(BUTTON_2, 0, 0x08);
  controller->ConfigButton(BUTTON_3, 0, 0x10);
  controller->ConfigButton(BUTTON_4, 0, 0x01);
  controller->ConfigButton(BUTTON_5, 0, 0x02);
  controller->ConfigButton(BUTTON_6, 0, 0x20);

}


void setupDaemonSaturn(HIDController *controller) {
  // DPad setup
  controller->ConfigButton(BUTTON_LEFT,  2, 0xFF, 0xFF);
  controller->ConfigButton(BUTTON_RIGHT, 2, 0xFF, 0x01);
  controller->ConfigButton(BUTTON_UP, 3, 0xFF, 0xFF);
  controller->ConfigButton(BUTTON_DOWN, 3, 0xFF, 0x01);
  // Button setup
  controller->ConfigButton(BUTTON_COIN, 0, 0x08 );
  controller->ConfigButton(BUTTON_START, 0, 0x80 );
  controller->ConfigButton(BUTTON_1, 0, 0x40);
  controller->ConfigButton(BUTTON_2, 0, 0x20);
  controller->ConfigButton(BUTTON_3, 0, 0x10);
  controller->ConfigButton(BUTTON_4, 0, 0x04);
  controller->ConfigButton(BUTTON_5, 0, 0x01);
  controller->ConfigButton(BUTTON_6, 0, 0x02);
  controller->ConfigButton(BUTTON_7, 1, 0x01);


}



/**************************
 * Honcam GamePads
 **************************/

/**
 * Configures a Honcam HC-J2003 and Compatible devices
 *
 * Honcam Button layout
 * Byte 0x01   0x02   0x04   0x08   0x10   0x20   0x40   0x80
 * 0:   Btn 1, Btn 4, Btn 5, Btn 2, Btn 7, Btn 3, Btn 8, Btn 6
 * 1:   COIN,  START, Btn 9, Btn10, Btn11, NA,    NA,    NA
 * 2:   NA,    NA,    NA,    NA,    NA,    NA,    NA,    NA
 * 3:                   LEFT(0x00)/RIGHT(0xFF)
 * 4:                   UP(0x00)/DOWN(0xFF)
 *
 * @param controller The HIDController that will be configured
 */

void setupHoncam(HIDController *controller) {
  // DPad setup
  controller->ConfigButton(BUTTON_LEFT, 3, 0xFF, 0);
  controller->ConfigButton(BUTTON_RIGHT, 3, 0xFF, 0xFF);
  controller->ConfigButton(BUTTON_UP, 4, 0xFF, 0);
  controller->ConfigButton(BUTTON_DOWN, 4, 0xFF, 0xFF);
  // Button setup
  controller->ConfigButton(BUTTON_COIN, 1, 0x01);
  controller->ConfigButton(BUTTON_START, 1, 0x02);
  controller->ConfigButton(BUTTON_1, 0, 0x01);
  controller->ConfigButton(BUTTON_2, 0, 0x08);
  controller->ConfigButton(BUTTON_3, 0, 0x20);
  controller->ConfigButton(BUTTON_4, 0, 0x02);
  controller->ConfigButton(BUTTON_5, 0, 0x04);
  controller->ConfigButton(BUTTON_6, 0, 0x80);
  controller->ConfigButton(BUTTON_7, 0, 0x10);
  controller->ConfigButton(BUTTON_8, 0, 0x40);
  controller->ConfigButton(BUTTON_9, 1, 0x04);
  controller->ConfigButton(BUTTON_10, 1, 0x08);
}



/**************************
 * Hori GamePads
 **************************/

/**
 * Configures a Hori Fighting Commander and Compatible devices
 *
 * Fighting Commander Button layout
 * Byte 0x01   0x02   0x04   0x08   0x10   0x20   0x40   0x80
 * 0:   Btn 1, Btn 4, Btn 5, Btn 2, Btn 7, Btn 3, Btn 8, Btn 6
 * 1:   COIN,  START, NA,    NA,    NA,    NA,    NA,    NA
 * 2:   DPAD,  DPAD,  DPAD,  DPAD,  NA,    NA,    NA,    NA
 *
 * @param controller The HIDController that will be configured
 */
void setupHoriFightingCmdr(HIDController *controller) {
  // DPad setup
  generateDPad(2, controller);
  // Button setup
  controller->ConfigButton(BUTTON_COIN, 1, 0x01);
  controller->ConfigButton(BUTTON_START, 1, 0x02);
  controller->ConfigButton(BUTTON_1, 0, 0x01);
  controller->ConfigButton(BUTTON_2, 0, 0x08);
  controller->ConfigButton(BUTTON_3, 0, 0x20);
  controller->ConfigButton(BUTTON_4, 0, 0x02);
  controller->ConfigButton(BUTTON_5, 0, 0x04);
  controller->ConfigButton(BUTTON_6, 0, 0x80);
  controller->ConfigButton(BUTTON_7, 0, 0x10);
  controller->ConfigButton(BUTTON_8, 0, 0x40);
}

/**************************
 * Hori GamePads
 **************************/

/**
 * Configures a Hori Real Arcade Pro and Compatible devices
 *
 * RAP Button layout
 * Byte 0x01   0x02   0x04   0x08   0x10   0x20   0x40   0x80
 * 0:   Btn 1, Btn 4, Btn 5, Btn 2, Btn 7, Btn 3, Btn 8, Btn 6
 * 1:   COIN,  START, Btn 9, Btn10, NA,    NA,    NA,    NA
 * 2:   DPAD,  DPAD,  DPAD,  DPAD,  NA,    NA,    NA,    NA
 *
 * @param controller The HIDController that will be configured
 */
void setupHoriRAP3(HIDController *controller) {
  // DPad setup
  generateDPad(2, controller);
  // Button setup
  controller->ConfigButton(BUTTON_COIN,  1, 0x01);
  controller->ConfigButton(BUTTON_START, 1, 0x02);
  controller->ConfigButton(BUTTON_1,  0, 0x01);
  controller->ConfigButton(BUTTON_2,  0, 0x08);
  controller->ConfigButton(BUTTON_3,  0, 0x20);
  controller->ConfigButton(BUTTON_4,  0, 0x02);
  controller->ConfigButton(BUTTON_5,  0, 0x04);
  controller->ConfigButton(BUTTON_6,  0, 0x80);
  controller->ConfigButton(BUTTON_7,  0, 0x10);
  controller->ConfigButton(BUTTON_8,  0, 0x40);
  controller->ConfigButton(BUTTON_9,  1, 0x08);
  controller->ConfigButton(BUTTON_10, 1, 0x04);
}


/**************************
 * Logitech GamePads
 **************************/

/**
 * Configures a Logitech F310 and Compatible devices
 *
 * Logitech F310 Button layout
 * Byte   0x01   0x02   0x04   0x08   0x10   0x20   0x40   0x80
 * 0-3:                           n/a
 * 4:     DPAD ,  DPAD, DPAD,  DPAD, btn 3,  btn 1, btn 2, btn 4
 * 5:     btn 5, btn 6, btn 7, btn 8, coin , start,   n/a,   n/a 
 *
 * @param controller The HIDController that will be configured
 */
void setupLogitech(HIDController *controller) {
  // DPad setup
  generateDPad(4, controller);
  // Button setup
  controller->ConfigButton(BUTTON_COIN,  5, 0x10);
  controller->ConfigButton(BUTTON_START, 5, 0x20);
  controller->ConfigButton(BUTTON_1, 4, 0x20);
  controller->ConfigButton(BUTTON_2, 4, 0x40);
  controller->ConfigButton(BUTTON_3, 4, 0x10);
  controller->ConfigButton(BUTTON_4, 4, 0x80);
  controller->ConfigButton(BUTTON_5, 5, 0x01);
  controller->ConfigButton(BUTTON_6, 5, 0x02);
  controller->ConfigButton(BUTTON_7, 5, 0x04);
  controller->ConfigButton(BUTTON_8, 5, 0x08);
  
}



/**************************
 * Mad Catz GamePads
 **************************/

/**
 * @brief Mad Catz SFV Arcade FightStick TE2+
 *
 * Mad Catz SFV Arcade FightStick TE2+ Button layout
 * Byte 0x01   0x02   0x04   0x08   0x10   0x20   0x40   0x80
 * 0-4:
 * 5:   DPAD,  DPAD,  DPAD,  DPAD,  BTN 1, BTN 4, BTN 5, BTN 2
 * 6:   BTN 7, BTN 3, BTN 8, BTN 6, NA,    NA,    COIN,  START
 *
 * @param controller The HIDController that will be configured
 */
void setupMadCatzTE2Plus(HIDController *controller) {
  /** DPad */
  generateDPad(5, controller);
  /** Buttons */
  controller->ConfigButton(BUTTON_COIN,  6, 0x10);
  controller->ConfigButton(BUTTON_START, 6, 0x20);
  controller->ConfigButton(BUTTON_1,     5, 0x10);
  controller->ConfigButton(BUTTON_2,     5, 0x80);
  controller->ConfigButton(BUTTON_3,     6, 0x02);
  controller->ConfigButton(BUTTON_4,     5, 0x20);
  controller->ConfigButton(BUTTON_5,     5, 0x40);
  controller->ConfigButton(BUTTON_6,     6, 0x08);
  controller->ConfigButton(BUTTON_7,     6, 0x01);
  controller->ConfigButton(BUTTON_8,     6, 0x04);
}



/**************************
 * Razer
 **************************/


/**
 * Configures a Razer controllers and Compatible devices
 *
 * PS4 DS4 Button layout
 * Byte 0x01   0x02   0x04   0x08   0x10   0x20   0x40   0x80
 * 0-4:                          NA
 * 5:   DPAD,  DPAD,  DPAD,  DPAD,  Btn 1, Btn 4, Btn 5, Btn 2
 * 6:   Btn 7, Btn 3, Btn 8, Btn 6, COIN,  START, NA,    NA
 *
 * @param controller The HIDController that will be configured
 */
void setupRJU(HIDController *controller) {
  // DPad setup
  generateDPad(5, controller);
  // Button setup
  controller->ConfigButton(BUTTON_COIN, 6, 0x10);
  controller->ConfigButton(BUTTON_START, 6, 0x20);
  controller->ConfigButton(BUTTON_1, 5, 0x10);
  controller->ConfigButton(BUTTON_2, 5, 0x80);
  controller->ConfigButton(BUTTON_3, 6, 0x02);
  controller->ConfigButton(BUTTON_4, 5, 0x20);
  controller->ConfigButton(BUTTON_5, 5, 0x40);
  controller->ConfigButton(BUTTON_6, 6, 0x08);
  controller->ConfigButton(BUTTON_7, 6, 0x01);
  controller->ConfigButton(BUTTON_8, 6, 0x04);;
}




/**************************
 * Retrobit GamePads
 **************************/

/**
 * Configures a Retrobit Saturn 2.4G controller and Compatible devices
 *
 * Retrobit Saturn 2.4G Button layout
 * Byte 0x01   0x02   0x04   0x08   0x10   0x20   0x40   0x80
 * 0:   Btn 2, Btn 5, Btn 4, Btn 1, Btn 7, Btn 8, Btn 3, Btn 6
 * 1:   COIN,  START, NA,    NA,    Btn 9, NA,    NA,    NA
 * 2:   DPAD,  DPAD,  DPAD,  DPAD,  NA,    NA,    NA,    NA
 *
 * @param controller The HIDController that will be configured
 */
void setupRetrobit_Saturn(HIDController *controller) {
  // DPad setup
  generateDPad(2, controller);
  // Button setup
  controller->ConfigButton(BUTTON_COIN, 1, 0x01);
  controller->ConfigButton(BUTTON_START, 1, 0x02);
  controller->ConfigButton(BUTTON_1, 0, 0x08);
  controller->ConfigButton(BUTTON_2, 0, 0x01);
  controller->ConfigButton(BUTTON_3, 0, 0x40);
  controller->ConfigButton(BUTTON_4, 0, 0x04);
  controller->ConfigButton(BUTTON_5, 0, 0x02);
  controller->ConfigButton(BUTTON_6, 0, 0x80);
  controller->ConfigButton(BUTTON_7, 0, 0x10);
  controller->ConfigButton(BUTTON_8, 0, 0x20);
  controller->ConfigButton(BUTTON_9, 1, 0x10);
}

/**************************
 * Sony GamePads
 **************************/


/**
 * @brief Configures a Sony PlayStation Classic Controller USB and Compatible devices
 *
 * Playstation Classic Button layout
 * Byte 0x01   0x02   0x04   0x08   0x10   0x20   0x40   0x80
 * 0:                   LEFT(0x00)/RIGHT(0xFF)
 * 1:                   UP(0x00)/DOWN(0xFF)
 * 2:   Btn 5, Btn 4, Btn 2, Btn 1, Btn 6, Btn 3, COIN,  START
 *
 * @param controller The HIDController that will be configured
 */
void setupPSC(HIDController *controller) {
	/** DPad */
	controller->ConfigButton(BUTTON_UP,         1, 0xFF, 0x04);
	controller->ConfigButton(BUTTON_UP_RIGHT,   1, 0xFF, 0x08);
	controller->ConfigButton(BUTTON_RIGHT,      1, 0xFF, 0x18);
	controller->ConfigButton(BUTTON_DOWN_RIGHT, 1, 0xFF, 0x28);
	controller->ConfigButton(BUTTON_DOWN,       1, 0xFF, 0x24);
	controller->ConfigButton(BUTTON_DOWN_LEFT,  1, 0xFF, 0x20);
	controller->ConfigButton(BUTTON_LEFT,       1, 0xFF, 0x10);
	controller->ConfigButton(BUTTON_UP_LEFT,    1, 0xFF, 0x00);
	/** Button */
	controller->ConfigButton(BUTTON_COIN  , 1, 0x01);
	controller->ConfigButton(BUTTON_START , 1, 0x02);
	controller->ConfigButton(BUTTON_1     , 0, 0x08); // Q
	controller->ConfigButton(BUTTON_2     , 0, 0x01); // T
	controller->ConfigButton(BUTTON_3     , 0, 0x80); // R1
	controller->ConfigButton(BUTTON_4     , 0, 0x04); // X
	controller->ConfigButton(BUTTON_5     , 0, 0x02); // B
	controller->ConfigButton(BUTTON_6     , 0, 0x20); // R2
	controller->ConfigButton(BUTTON_7     , 0, 0x40); // L1
	controller->ConfigButton(BUTTON_8     , 0, 0x10); // L2
}



/**
 * Configures a Sony PS4 DS4 controller and Compatible devices
 *
 * PS4 DS4 Button layout
 * Byte 0x01   0x02   0x04   0x08   0x10   0x20   0x40   0x80
 * 0-4:                          NA
 * 5:   DPAD,  DPAD,  DPAD,  DPAD,  Btn 1, Btn 4, Btn 5, Btn 2
 * 6:   Btn 7, Btn 3, Btn 8, Btn 6, COIN,  START, NA,    NA
 *
 * @param controller The HIDController that will be configured
 */
void setupPS4(HIDController *controller) {
  // DPad setup
  generateDPad(5, controller);
  // Button setup
  controller->ConfigButton(BUTTON_COIN, 6, 0x10);
  controller->ConfigButton(BUTTON_START, 6, 0x20);
  controller->ConfigButton(BUTTON_1, 5, 0x10);
  controller->ConfigButton(BUTTON_2, 5, 0x80);
  controller->ConfigButton(BUTTON_3, 6, 0x02);
  controller->ConfigButton(BUTTON_4, 5, 0x20);
  controller->ConfigButton(BUTTON_5, 5, 0x40);
  controller->ConfigButton(BUTTON_6, 6, 0x08);
  controller->ConfigButton(BUTTON_7, 6, 0x01);
  controller->ConfigButton(BUTTON_8, 6, 0x04);
}


/**
 * Configures a Sony PS5 DualSense controller and Compatible devices
 *
 * PS5 DualSense Button layout
 * Byte 0x01   0x02   0x04   0x08   0x10   0x20   0x40   0x80
 * 0-7:                          NA
 * 8:   DPAD,  DPAD,  DPAD,  DPAD,  Btn 1, Btn 4, Btn 5, Btn 2
 * 9:   Btn 7, Btn 3, Btn 8, Btn 6, COIN,  START, NA,    NA
 *
 * @param controller The HIDController that will be configured
 */
void setupPS5(HIDController *controller) {
  // DPad setup
  generateDPad(8, controller);
  // Button setup
  controller->ConfigButton(BUTTON_COIN, 9, 0x10);
  controller->ConfigButton(BUTTON_START, 9, 0x20);
  controller->ConfigButton(BUTTON_1, 8, 0x10);
  controller->ConfigButton(BUTTON_2, 8, 0x80);
  controller->ConfigButton(BUTTON_3, 9, 0x02);
  controller->ConfigButton(BUTTON_4, 8, 0x20);
  controller->ConfigButton(BUTTON_5, 8, 0x40);
  controller->ConfigButton(BUTTON_6, 9, 0x08);
  controller->ConfigButton(BUTTON_7, 9, 0x01);
  controller->ConfigButton(BUTTON_8, 9, 0x04);
}


/**************************
 * Sega GamePads
 **************************/

/**
 * Configures an 8BitDo SFC30 Wired Controller and Compatible devices
 *
 * 8BitDo SFC30 Button layout
 * Byte 0x01   0x02   0x04   0x08   0x10   0x20   0x40   0x80
 * 3:                   LEFT(0x00)/RIGHT(0xFF)
 * 4:                   UP(0x00)/DOWN(0xFF)
 * 5:   NA,    NA,    NA,    NA,    Btn 2, Btn 5, Btn 4, Btn 1
 * 6:   Btn 3, Btn 6, NA,    NA,    COIN,  START, NA,    NA
 *
 * @param controller The HIDController that will be configured
 */
void setupSegaAstroCityMini(HIDController *controller) {
  // DPad setup
  controller->ConfigButton(BUTTON_LEFT, 3, 0xFF, 0);
  controller->ConfigButton(BUTTON_RIGHT, 3, 0xFF, 0xFF);
  controller->ConfigButton(BUTTON_UP, 4, 0xFF, 0);
  controller->ConfigButton(BUTTON_DOWN, 4, 0xFF, 0xFF);
  // Button setup
  controller->ConfigButton(BUTTON_COIN, 6, 0x10);
  controller->ConfigButton(BUTTON_START, 6, 0x20);
  controller->ConfigButton(BUTTON_1, 5, 0x80);
  controller->ConfigButton(BUTTON_2, 5, 0x10);
  controller->ConfigButton(BUTTON_3, 6, 0x01);
  controller->ConfigButton(BUTTON_4, 5, 0x40);
  controller->ConfigButton(BUTTON_5, 5, 0x20);
  controller->ConfigButton(BUTTON_6, 6, 0x02);
}

/**************************
 * ShanWan GamePads
 **************************/

/**
 * Configures a Neo Geo Mini Pad and Compatible devices
 *
 * Neo Geo Mini Pad Button layout
 * Byte 0x01   0x02   0x04   0x08   0x10   0x20   0x40   0x80
 * 0:   Btn 4, Btn 2, Btn 1, Btn 3, NA,    NA,    NA,    NA
 * 1:   COIN,  START, NA,    NA,    NA,    NA,    NA,    NA
 * 2:   DPAD,  DPAD,  DPAD,  DPAD,  NA,    NA,    NA,    NA
 *
 * @param controller The HIDController that will be configured
 */
void setupNeoGeoMini(HIDController *controller) {
  // DPad setup
  generateDPad(2, controller);
  // Button setup
  controller->ConfigButton(BUTTON_COIN, 1, 0x01);
  controller->ConfigButton(BUTTON_START, 1, 0x02);
  controller->ConfigButton(BUTTON_1, 0, 0x04);
  controller->ConfigButton(BUTTON_2, 0, 0x02);
  controller->ConfigButton(BUTTON_3, 0, 0x08);
  controller->ConfigButton(BUTTON_4, 0, 0x01);
}
