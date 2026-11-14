//          USB2DB15 adapter for Supergun/Neogeo
//
//
//         Author:           Raph_friend
//         Date:             2026-11-09
//         Version:          1.0
//
//         USB Host Shield 2.0 modded lib:         https://github.com/Raphfriend/USB_Host_Shield_2.0
//
//         Compiled with Arduino 1.8.15
//
//
//              Thanks to:
//
//              Oleg Mazurov for USB Host Shield 2.0 and libs
//              HUYE (twitter @huye_4589) for USB controller -> ATARI ver 1.21
//              たねけん (twitter @taneken2000) for kind help on HUYE code understanding
//              NearlyLogical beta testing and code improvement implementation
//              Frank_fjs for beta testing and all in one PCB design
//              fluxcore for beta testing
//              suverman for beta testing
//              akiramishima for beta testing
//              SpaceJockey for beta testing
//
//
//            メインプログラム、コントローラタイプ(TYPE_PS4, TYPE_MDmini) :
//
//            コントローラタイプ(TYPE_PSC, TYPE_RAP3, TYPE_RAP4, TYPE_SNES, TYPE_RETROFREAK) :
//
//

// Override USB Polling frequency --This needs to go before includes--
#define POLLING_OVERRIDE 1000

#include "BuildConfig.h"

#include <XBOXONE.h>
#include <XBOXUSB.h>
#include <PS3USB.h>
#include <EEPROM.h>
#include <usbhid.h>
#include <hiduniversal.h>

#include "EepromManager.h"
#include "HIDController.h"
#include "PS3Controller.h"
#include "XBoxOneController.h"
#include "XBoxUSBController.h"
#include "M30XboxUSB.h"
#include "NeoGeoUSBController.h"
#include "NeoGeoXboxUSB.h"
#include "USB2DB15.h"
#include "LED.h"

#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

#define ANALOG_BUTTON_PIN A6

USB Usb;
PS3USB PS3(&Usb);
XBOXONE Xbox(&Usb);
M30XboxUSB Xbox360(&Usb);
NeoGeoXboxUSB NeoGeo(&Usb);

EepromManager Eeprom;
HIDController HIDCon(&Usb);
PS3Controller PS3Con(&PS3);
XBoxOneController XBoxCon(&Xbox);
XBoxUSBController XBox360Con(&Xbox360);
NeoGeoUSBController NeoGeoCon(&NeoGeo);
USB2DB15 Usb2db15(PS3Con, XBoxCon, XBox360Con, HIDCon, NeoGeoCon, Eeprom);

uint8_t resetLock = 0;

void setup() {
  #ifndef RELEASE_MODE
  Serial.begin(115200);
  #endif
  if (Usb.Init() == -1) {
    #ifndef RELEASE_MODE
    Serial.print(F("\r\nOSC did not start"));
    #endif
    while (1);
  }
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  LED::Update();

  if (analogRead(ANALOG_BUTTON_PIN) > 800) {
    if(!resetLock) {
      LED::Blink(3, LED::BLINK_RATE_RESET);
      Usb2db15.FactoryReset();
      resetLock = 1;
    }
  } else {
    resetLock = 0;
  }

  Usb.Task();
  Usb2db15.GenerateOutput();
}

//******************************************
// End of File
//******************************************
