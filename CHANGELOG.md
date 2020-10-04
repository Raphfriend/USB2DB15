## [0.13.7] - 2020-10-04
- Extended `Mad Catz Street Fighter IV Tournament Edition (PS3)` entry in `drivers.cpp` & `drivers.h` to support both editions (Round 1 & Round 2) of the controller.

## [0.13.6] - 2020-09-28
- Added Support for the Qanba Obsidian in both PS3 and PS4 modes

## [0.13.5] - 2020-09-18
### Added
- Added Support for 8BitDo NES & SNES Bluetooth wireless receivers (requires USB HOST SHIELD 2.0 update)
	- Sony PS4 DS4, Nintendo Switch Pro, 8BitDo M30 Bluetooth controllers tested and verified as working
	- Note: 8BitDo receivers must be on Firmware v1.33
- Added Support For Hori Fighting Stick Mini 4 (requires USB HOST SHIELD 2.0 update)
- Library updated: https://github.com/Raphfriend/USB_Host_Shield_2.0
	- USB Host Shield 2.0 files updated:
		- PS3USB.cpp
		- PS3USB.h
		- PS3Enums.h

## [0.13.4] - 2020-09-16
### Added
- Added Support for 8BitDo SFC30
- Altered 8BitDo M30 to use D-INPUT mode

## [0.13.3] - 2020-09-16
### Added
- Added Support for Generic SNES USB pad
- Refactored the Retrobit Saturn Wireless driver

## [0.13.2] - 2020-09-15
### Added
- Added Support for Honcam HC-J2003

## [0.13.1] - 2020-09-12
### Added

- Added Support for Nacon Daija in PS3 and PS4 modes

## [0.13.0] - 2020-09-12
### Added

- Added a new LED class to control an LED connected to digital pin 8.
	- LED::On(); Turns the LED ON
	- LED::Off(): Turns the LED OFF
	- LED::Blink(n); Blinks the LED where n equals the number of desired blinks
	- The LED is set to turn ON when Bind mode is entered and remains on until Bind mode is exited, thus turning the LED OFF. 
	- The LED will blink with a profile change and blinks an amount of times that corresponds to the profile number.
- Added a resetEEPROM() function that clears all EEPROM memory, thus clearing profiles and bindings. This is controlled via a tactile switch connected to analog pin 6.


## [0.12.0] - 2020-09-11
### Changed

- Profiles are now per controller rather than shared between all controllers
- There are now only 4 profiles, they are mapped to SELECT + directional(UP, DOWN, LEFT, RIGHT)
- Adapters can remember up to 10 controllers, if more then 10 are plugged in, the oldest one will be forgotten.

## [0.11.0] - 2020-08-30
### Added

- Support for user defined profiles. In order to do this it makes the following changes.

### Changed

- Profiles are now stored in EEPROM
- A new EepromManager class is added. It initializes the EEPROM with default values and Manages Access to the EEPROM
- Added a GetButtonClick function to Controller. It returned true only once each time a button is pressed.
- Implemented GetButtonClick for the PS3, XBoxOne and HID controllers
- Pulled Normal Output into it's own function
- Added BindMode in addition to NormalMode to handle when the adapter is binding to profiles


## [0.10.0] - 2020-08-23
### Added
- Refactored the way button input was processed to allow for profiles
- Added profile support to PS3, Xbox and HID controllers
- Controller drivers were rewritten currently the following controllers are supported as HID devices:
  - 8BitDo M30 wired Controller
  - iBUFFALO Snes Classic Controller
  - Brook Universal Fighting PCB
  - HORI Fighting Commander
  - HORIPAD in ps4 mode
  - HORI Wired Controller lite
  - HORI Real Arcade Pro.3 PS3 mode
  - HORI Real Arcade Premium VLX
  - HORI Real Arcade Pro V HAYABUSA PS3 and PS4 Modes
  - Razer Panthera
  - Retrobit Sega Saturn Wireless
  - Sony PS4 and PS3 Controllers
  - Sony Wireless Adapter
  - Universal PCB firmware version 29
  - Xbox One official and some 3rd Party controllers
  - 4 Builtin Profiles.  They can be switch by holding UP + SELECT + BUTTON 1-4 (button number is relative to the default profile)
  - Button 1: Default profile
  - Button 2: Default but the rows are swapped
  - Button 3: SNES style where button 6 is on L1
  - Button 4: SNES with rows swapped
  

## [0.9.1] - 2020-08-21
### Added

- Added support to HORI Real Arcad Pro - Soul Calibur VI Edition for Xbox One (requires USB HOST SHIELD 2.0 update) [@kitsune](https://github.com/kitsune)
- Library updated: https://github.com/Raphfriend/USB_Host_Shield_2.0
- USB Host Shield 2.0 files updated:
	- XBOXONE.h
