## Known Compatible Controllers by Manufacturer
This is a list of tested controllers. Please reach out to our Discord server if you have a controller not listed here and wish to be a tester.  You will need to test each beta version of the software for regressions

### 8BitDo
- M30 Bluetooth Controller
	- Via wired connection
	- Place controller into D-INPUT mode by holding B + START upon powering ON controller
	- Press SELECT + DPAD UP to enable digital DPAD
- SFC30 (Via wired connection)
- NES & SNES Bluetooth Receivers (Firmware v1.33) 
  - NES & SNES Bluetooth receviers tested compatible with the following controllers:
    - Sony PS4 DS4
    - Nintendo Switch Pro controller
    - 8BitDo Arcade Stick (November 2020 version) on Switch mode and Firmware 1.03
    - 8BitDo SFC30
    - 8BitDo M30 Bluetooth controller (controller must be placed in either MacOS mode by holding A + START or X-Input mode by holding X + START upon powering on controller)

### Buffalo
- SNES Classic GamePad

### Brook
- Brook FIGHTING BOARD PS3/PS4
- Brook Universal Fighting PCB
- Brook Retro Board
- Brook Wingman XB Converter Wireless Adapter OG and 2

### DaemonBite Retro Controllers To USB Adapters:
- NES/SNES (VID/PID 2341:8037)
- SEGA Saturn (VID/PID 2341:8030)
- SEGA MegaDrive/Genesis (VID/PID 2341:8031)

	DaemonBite Retro Controllers To USB Adapters project uses an Arduino Pro Micro, when the code is burned, will show as Leonardo and a VID/PID will be attached. Each controller code own especific output patterns and can't use same ID Hardwares on the USB2DB15 adapter, so we choose to change the PID on Saturn and on (NES/SNES is standard) to make the code working well. It won't change anything on the right functionalities for both adapters in any device (Windows PC or MiSTer, for example).
	
	- To do that, just find Arduino > avr > boards.txt file; 
	- Save this file in a another place so you don't lose it. 
	- Find something like it: 
		micro.build.vid=0x2341
		micro.build.pid=0x8037
		micro.build.usb_product="Arduino Micro"

	- Change for something like it:

			micro.build.vid=0x2341		
			micro.build.pid=0x8030 (PID used for Saturn)
			micro.build.usb_product="Arduino Micro"
	- Save the file.

### Feir
- Feir Wired FR-225C Controller for PlayStation 4

### FeralAI
- Pico Fighting Board GP2040 (PS3/DirectInput Mode)

### Generic
- Generic SNES USB pad
- Generic Zero Delay Controller

### Honcam
- HC-J2003

### HORI
- HORIPAD 3 Mini
- Hori Fighting Edge PS4 (requires USB HOST SHIELD 2.0 update)
- HORI Wireless Controller Lite
- Fighting Stick 3
- Fighting Stick V3
- Fighting Commander (PS3 mode)
- Fighting Stick Mini 4 (requires USB HOST SHIELD 2.0 update)
- Real Arcade Pro.3 SA (PS3 mode)
- Real Arcade Pro.3 Premium VLX (PS3 mode)
- Real Arcade Pro V HAYABUSA (PS3 and PS4 modes)
- Real Arcade Pro - Soul Calibur VI Edition for Xbox One (requires USB HOST SHIELD 2.0 update)

### Logitech
- F310 

### Mad Catz
- Street Fighter IV - Arcade FightStick Tournament Edition PS3 (Round 1 & Round 2 versions)
- Street Fighter V FightStick Alpha (PS3 and PS4 Modes)
- Street Fighter V Arcade FightStick TE2+ (PS4 Mode)
- Street Fighter V Arcade FightStick TES+ (PS4 Mode)

### Microsoft
- Xbox One Controller
- Xbox Series S|X Controller

### Nacon
- Daija (PS3 and PS4 Modes)

### PowerA
 - FUSION Wired FightPad Gaming Controller for PlayStation 4

### PXN 
 - PXN 0082 Arcade Stick

### Qanba
- Crystal (PS3 and PS4 Modes)
- Drone (PS4)
- Obsidian (PS3 and PS4 Modes)

### Razer
- Panthera
- Panthera EVO
- Raiju Ultimate 

### Retrobit
- Sega Saturn Wireless Controller 2.4GHz (Via wireless USB receiver)
	- Press START + UP to enable digital DPAD
### SEGA
- Astro City Mini Arcade Stick
- Astro City Mini Control Pad

### SNK
- Neo Geo Mini Pad

### Sony
- Dual Shock 3
- PS4 Offical Controllers
- PS4 Wireless Adapter
- PS5 DualSense
- PlayStation Classic Controller

### Taito
- Egret II Mini Controller

### Toodles
- MC Cthulhu

### Universal PCB Project
- Universal PCB Firmware 29
