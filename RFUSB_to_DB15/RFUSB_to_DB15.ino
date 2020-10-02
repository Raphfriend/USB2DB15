//          USB2DB15 adapter for Supergun/Neogeo
//
//
//         Author:           Raph_friend
//         Date:             06.07.2020
//         Version:          1.0
//
//         USB Host Shield 2.0 modded lib:         https://github.com/Raphfriend/USB_Host_Shield_2.0
//
//         Compiled with Arduino 1.8.13
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

// Override USB Polling freqency --This needs to go before includes--
#define POLLING_OVERRIDE 1000

#include <XBOXONE.h>
#include <PS3USB.h>
#include <EEPROM.h>
#include <usbhid.h>
#include <hiduniversal.h>

#include "EepromManager.h"
#include "HIDController.h"
#include "PS3Controller.h"
#include "XBoxOneController.h"
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

EepromManager Eeprom;
HIDController HIDCon(&Usb);
PS3Controller PS3Con(&PS3);
XBoxOneController XBoxCon(&Xbox);
USB2DB15 Usb2db15(PS3Con, XBoxCon, HIDCon, Eeprom);

uint8_t resetLock = 0;

// JoystickHID Hid1(&Usb);

void setup() {
  Serial.begin(115200);

  while (!Serial);

  byte i, j;

  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1);
  }
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  LED::Update();

  if (analogRead(ANALOG_BUTTON_PIN) > 800) {
    if(!resetLock) {
      LED::Blink(3, LED::BLINK_RATE_RESET);
      Serial.println("Resetting");
      Usb2db15.FactoryReset();
      resetLock = 1;
    }
  } else {
    resetLock = 0;
  }

  Usb.Task();

  delay(1);

  Usb2db15.GenerateOutput();

  delay(1);
}

//******************************************
// End of File
//******************************************
