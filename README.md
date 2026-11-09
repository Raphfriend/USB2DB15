# USB2DB15 - Supergun Adapter  
[![ko-fi](https://ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/raph_friend)

A simple ATmega328P-based USB-to-DB15 adapter designed for use with a Supergun or Neo Geo system.


<p align="center">
  <img width="680" src="https://i.imgur.com/wUzNxtB.png">
 <br>
  <b>USB2DB15 with a Sega Astro City Mini Arcade Stick</b><br>
<br>
	</p>  	

### 1. Features:

- USB input to support most modern HID compatible controllers
- 1ms polling, meaning a low latency and fast response from the USB2DB15
- Open-source firmware, so everyone in the community can collaborate
- DB15 pinout standard, ready for Neo Geo, Minigun, and HAS Superguns
- 4 customizable and storable mapping configurations
- 10 customizable and storable controller profiles
- Status LED
- Protection circuit for your beloved controllers

### 2. Technical information:

The USB2DB15 adapter is an open-source device based on an AVR microcontroller. It adapts a USB controller for use with a JAMMA PCB through a Supergun or with Neo Geo hardware that has controller ports. A variety of common controllers are supported, including controllers that follow the PS3/PS4 DualShock, PS5 DualSense, and Xbox 360/One protocols. Due to the diversity of controllers and differing standards, some controllers may not work with USB2DB15 or may not work as intended. Please be patient during these early stages. Feel free to contact the developers with additional information or requests for help with your USB2DB15 adapter through our communication channels (GitHub, Discord, and the Arcade-Projects forum).

We are using a modified USB Host Shield 2.0 library for controller IDs that aren't natively supported.

With regard to hardware, there are currently three options:

- A Custom PCB that accommodates and integrates an Arduino Pro Mini, USB Shield Mini and other relevant hardware.
- USB2DB15 2.0 Ver, a pre-built single PCB that incorporates all required hardware in a standalone form.
- USB2DB15 CS Ver, similar to 2.0 Ver but focused on PCB consolization. In place of the DB15 controller connector, this board has screw terminals and a 20-pin header compatible with a 20-pin Brook Fighting Board Cable.





Alternatively, you can simply use a breadboard to connect it all together.

If you are building the open source version for yourself, a basic understanding of Arduino and its programming environment is required. Visit [arduino.cc](https://arduino.cc) to help you get started.

<p align="center">
  <img width="380" src="https://i.imgur.com/nXCXxz3.png">
 <br>
  <b>USB2DB15 pinout standard</b><br>
<br>



### 3. Open Source Required Hardware:

* The USB2DB15 uses an ATmega328P AVR. Any Arduino that uses this microcontroller, such as the Pro Mini, Nano, or Uno, is suitable. The custom PCB accommodates an Arduino Pro Mini and USB Host Shield Mini.
* USB Host Shield Mini if using an Arduino Pro Mini / Nano. Full sized USB Host Shield for Uno.
* An FTDI programmer is required for the Arduino Pro Mini. This is not required if using an Arduino Nano or Uno.
* A USB2DB15 custom PCB or a breadboard to complete all connections between the AVR and USB Host Shield.
* DB15 female connector.

<p align="center">
  <img width="680" src="https://i.imgur.com/UeZuOrb.jpg">
 <br>
  <b>Arduino Pro Mini and Usb Host Shield Mini</b><br>
<br>
	</p>  	
    
  		   
If using the custom PCB with an Arduino Pro Mini and USB Host Shield Mini, a BOM is available in the repository's `BOM` folder.

##### Jumper settings:

- SJ1: Bridge this jumper ONLY to disable 5V protection.
- SJ2: Bridge this jumper ONLY to enable 5V protection.
- JP2: 5V solder point (see information below regarding USB VBUS).

Due to variations among Pro Mini PCBs, A4 and A5 may be in different locations. Please pay attention to this.

<p align="center">
<img width="680" src="https://i.imgur.com/sYRVOBI.png">
 <br>
  <b>Custom PCB 1.2 for Arduino Pro Mini</b><br>
</p>


### 4. Installation:

Download the <a href="https://github.com/Raphfriend/USB_Host_Shield_2.0">USB Host Shield 2.0 library</a> and install it in the Arduino IDE. Open the INO file (`RFUSB_to_DB15.ino`) and upload it to your AVR.

If you have a USB Host Shield Mini, you will need to cut the trace after the 2.2 kΩ resistor near the USB VBUS pin. Solder a wire from JP2 on the custom PCB to the through-hole via labeled `5V`, as shown in the picture below.

<p align="center">
  <img width="460" src="https://i.imgur.com/vGgNsPl.png">
</p>

This enables devices that require +5 V power to function correctly, because the USB Host Shield Mini supplies only 3.3 V to the VBUS pin of the USB connector by default.

The remainder is plug and play.

If you are not using a prefabricated PCB, see the schematic for wiring information.

#### 4.1. Arduino Pro Mini + USB Shield Mini:
[See FLASHING.md for instructions.](FLASHING.md)

#### 4.2. Arduino Uno + USB Host Shield:

1. Install Arduino IDE.
2. Mount the USB Host Shield to the Arduino Uno correctly.
3. Connect the Arduino Uno to your PC via its USB Type-B cable.
4. In the Arduino IDE, navigate to "Tools" -> "Board" -> "Arduino AVR Boards" and select "Arduino Uno".
5. In the Arduino IDE, navigate to "Tools" -> "Port" and choose the Arduino device.
6. In the Arduino IDE, navigate to "Tools" -> "Programmer" and select "AVRISP MKII".
7. Find the library path in Arduino IDE: "File" -> "Preferences" -> "Sketchbook location", then copy the required `USB_Host_Shield_20` library into the `libraries` directory.
8. Open the firmware INO file (`RFUSB_to_DB15.ino`).
9. With the Arduino Uno connected to your PC, select "Sketch" -> "Upload".
10. Wait for the sketch to upload, as indicated in the console window at the bottom of the Arduino IDE.

### 5. Supported Controllers:

[See Compatibility.md for a detailed list of supported controllers.](COMPATIBILITY.md)

#### 5.1. DualShock 4 and DualSense left analog stick

USB DualShock 4 and DualSense controllers can use the left analog stick as an additional digital directional input. The analog stick and physical D-pad are combined, so either input can control the DB15 directions.

For these controllers, the firmware applies Hit Box-style SOCD cleaning after combining both directional sources:

| Simultaneous directions | DB15 output |
| --- | --- |
| Left + Right | Neutral |
| Up + Down | Up |

This prevents opposite horizontal directions from reaching the game and makes vertical conflicts deterministic. The feature is enabled automatically for supported DualShock 4 and DualSense USB reports; no separate profile or user configuration is required.

#### 5.2. Release mode and Serial diagnostics

`RFUSB_to_DB15/BuildConfig.h` contains the following build-time setting:

```cpp
#define USB2DB15_RELEASE_MODE 1
```

Use release mode (`1`) for competitive play. It disables all firmware Serial output, preventing diagnostic messages from occupying the serial transmit buffer and delaying controller polling or DB15 updates.

Use diagnostic mode (`0`) when troubleshooting a controller:

```cpp
#define USB2DB15_RELEASE_MODE 0
```

Upload the firmware again after changing the setting, then open the Arduino Serial Monitor at 115200 baud. Set the value back to `1` and upload again before competitive play.

### 6. Software Usage
#### 6.1 Controller Profiles
The adapter can store profiles for up to 10 unique controller models*. Controllers of the same make and model share profiles.
Each controller has 4 associated profiles. If you plug in an 11th controller, the system replaces the oldest controller you have set up, then the next oldest for the 12th, and so on. This may require you to set up older controllers again when you resume using them.

These can be accessed by pressing SELECT (COIN) plus one of the four directions.
For example, press SELECT + UP for the first profile, SELECT + RIGHT for the second, and so on.
The adapter will remember what profile you were using and automatically switch to it when you use the controller again.

To change a profile's key bindings, first switch to the profile you want to set up.
While holding SELECT for at least 3 seconds, press buttons 1–6 in order. Then release SELECT to lock in the binding.

*XBox One controllers appear to the system as the same make and model and thus use the same profiles even if they are different controllers.

### Shell

[AvramCE](https://twitter.com/avramce) has created a [3D printed case](https://www.thingiverse.com/thing:5170415) that you can print for your USB2DB15.

### How does it look in real life?

When you put all the parts together, USB2DB15 should look like this.
<p align="center">
  <img width="680" src="https://i.imgur.com/S0fqrp9.jpg">
	 <br>
  <b>Setup and photo by <a href="https://twitter.com/Focux11">@Focux</a> </b>
	</p>  	
<p align="center">
  <img width="680" src="https://i.imgur.com/m0nqRHR.jpg">
	 <br>
    <b>Setup and photo by <a href="https://twitter.com/_atrac17">@_atrac17</a> </b>
	</p>  	

### Support this project

If you have found USB2DB15 useful, please consider funding it and making a donation so that we may continue our work by obtaining additional controllers, equipment, and everything else required to keep this project alive and growing.

[![ko-fi](https://ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/raph_friend)


### Is this product available to buy? Yes!

We sell USB2DB15 directly. You will receive a premium edition that is slightly different from the open-source version but has the same features.
You can choose from a variety of shell color options, and you will get detailed instructions following your purchase.
There are two versions: the standard DB15 and the CS Ver., which is targeted toward JAMMA PCB consolization.




<a href="https://ko-fi.com/s/b6c3d01043">Access our shop by clicking here or on the images below. </a>

<p align="center">
<a href="https://ko-fi.com/s/b6c3d01043">
  <img width="460" src="https://i.imgur.com/MU63FKN.jpg">
</p>

<p align="center">
<a href="https://ko-fi.com/s/b6c3d01043">
  <img width="460" src="https://i.imgur.com/XsNq3t4.jpg">
</p>

<p align="center">
<a href="https://ko-fi.com/s/b6c3d01043">
  <img width="460" src="https://i.imgur.com/l4fS8WO.jpg">
</p>

<p align="center">
<a href="https://ko-fi.com/s/b6c3d01043">
  <img width="460" src="https://i.imgur.com/xEf0cEx.jpg">
</p>

### Where can I get more help if I need it?

Via Arcade-Projects forum or RF Projects Discord server.

<p align="center">
<a href="https://www.arcade-projects.com/forums/index.php?thread/14205-open-source-usb-to-db15-for-supergun-adapter/">
  <img width="460" src="https://i.imgur.com/7arl7Zn.png">
</p>

<p align="center">
<a href="https://discord.gg/XKCjtpH">
  <img width="460" src="https://i.imgur.com/LZJjOUJ.png">
</p>


### LEGAL

Neither USB2DB15 nor any of its maintainers is associated with or endorsed by Nintendo, Sony, SNK, Microsoft, 8BitDo, Sega, Brook, Mad Catz, Nacon, Honcam, DaemonBite, Buffalo, Qanba, Razer, Retro-Bit, Toodles, Hori, or any other company not explicitly indicated. All other brands and product names are the property of their respective holders.
