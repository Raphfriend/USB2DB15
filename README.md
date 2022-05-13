# USB2DB15 - Supergun Adapter [![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=HYCK7DF7Q6XLY&source=url)

A simple Atmega328P based USB to DB15 adapter designed for use with a Supergun or Neo-Geo system.


<p align="center">
  <img width="680" src="https://i.imgur.com/wUzNxtB.png">
 <br>
  <b>USB2DB15 with a Sega Astro City Mini Arcade Stick</b><br>
<br>
	</p>  	

### 1. Features:

- USB input to support most modern HID compatible controllers
- 1ms polling, it means low latency and fast response from USB2DB15
- Open Source firmware, so everyone on the community can colabore
- DB15 pinout standard ready for Neo Geo, Minigun, and HAS superguns
- 4 customizable and storable mapping configurations
- 10 customizable and storable controller profiles
- Status LED
- Protection circuit for your belevod controllers

### 2. Technical information:

The USB2DB15 adapter is an Open Sourced device based on an AVR microcontroller. It adapts a USB controller for use on a Jamma PCB via a Supergun or Neo-Geo hardware with controller ports. Presently, a variety of common controllers are supported; such as ones that follow the PS3/PS4 DualShock, PS5 DualSense and Xbox 360/One protocols. Due to the diversity of controllers available and differing standards there may be some controllers that will not work with the USB2DB15 or may not work as intended. Please be patient during these early stages and feel free to contact the developers with additional information and requests for help with your USB2DB15 adapter via our communication channels (GitHub, Discord, Arcade-Projects forum).

We are using a modified USB Host Shield 2.0 library for controller IDs that aren't natively supported. 

In regards to hardware there are currently two options:

- A Custom PCB that accommodates and integrates an Arduino Pro Mini, USB Shield Mini and other relevant hardware.
- USB2DB15 2.0 Ver, a Standalone PCB that incorporates all the required hardware in a standalone form.
- USB2DB15 CS Ver, same from 2.0 Ver, but now aimed for PCB conzolization, you will have screw terminals and a 20 pin header (compatible with Brook Fighting Board Cable 20-pin).





Alternatively you can simply use a breadboard to tie it all together. 

A basic understanding of Arduino and its programming environment is required. Visit arduino.cc to help you get started.

<p align="center">
  <img width="380" src="https://i.imgur.com/nXCXxz3.png">
 <br>
  <b>USB2DB15 pinout standard</b><br>
<br>



### 3. Required hardware:

* We are using an Atmega328P AVR. Any Arduino that utilizes this microcontroller such as the Mini Pro, Nano and Uno are suitable. The Custom PCB accommodates an Arduino Mini Pro and USB Host Shield Mini.
* USB Host Shield Mini if using an Arduino Pro Mini / Nano. Full sized USB Host Shield for Uno.
* An FTDI programmer is required for the Arduino Pro Mini. This is not required if using an Arduino Nano or Uno.
* USB2DB15 Custom PCB or a breadboard to complete all the connections between the AVR and USB Host Shield.
* DB15 female connector.

<p align="center">
  <img width="680" src="https://i.imgur.com/UeZuOrb.jpg">
 <br>
  <b>Arduino Pro Mini and Usb Host Shield Mini</b><br>
<br>
	</p>  	
    
  		   
If utilizing the Custom PCB in conjunction with an Arduino Pro Mini and Mini USB Shield, a BOM is available from the repositories section within the PCB folder. 

##### Jumper settings:

- SJ1: Bridge this jumper ONLY to disable 5V protection.
- SJ2: Bridge this jumper ONLY to enable 5V protection.
- JP2: 5V solder point (see information below regarding USB VBUS).

Due to variances of Mini Pro PCB's, A4 and A5 may be in different locations. Please pay attention to this.

<p align="center">
<img width="680" src="https://i.imgur.com/sYRVOBI.png">
 <br>
  <b>Custom PCB 1.2 for Arduino Pro Mini</b><br>
</p>


### 4. Installation:

Download the  <a href="https://github.com/Raphfriend/USB_Host_Shield_2.0">USB Host Shield 2.0 library</a>  and install it to the Arduino IDE. Open the INO file (RFUSB_to_DB15.ino) and program it to your AVR.  

If you have a USB Host Shield Mini, you will need to CUT the trace after the 2K2 resistor near the USB VBUS pin. Solder a wire from JP2 of the CUSTOM PCB to the through-hole VIA labelled '5V' as depicted in the picture below.

<p align="center">
  <img width="460" src="https://i.imgur.com/vGgNsPl.png">
</p>

This enables devices that require +5V power to function correctly, as by default the USB Host Shield Mini only supplies 3.3V to the VBUS pin of the USB connector.

The remainder is plug and play. 

If you are not using a prefabricated PCB, please view the schematic for wiring information.

#### 4.1. Arduino Pro Mini + USB Shield Mini:
[See FLASHING.md for instructions.](FLASHING.md)

#### 4.2. Arduino Uno + USB Host Shield:

1. Install Arduino IDE.
2. Mount the USB Host Shield to the Arduino Uno correctly.
3. Connect the Arduino Uno to your PC via its USB B cable. 
4. In the Arduino IDE navigate to "Tools" -> "Board" -> "Arduino AVR Boards" and select "Arduino Uno".
5. In the Arduino IDE navigate to "Tools" -> "Port" and choose the Arduino Device. 
6. In the Arduino IDE navigate to "Tools" -> "Programmer" and select "AVRISP MKII".
7. Obtain the library path (Arduino IDE -> File -> Preference-> Settings->Sketchbook location) and drop the required library "USB_Host_Shield_20" to the library directory.
8. Obtain the INO firmware (RFUSB_to_DB15.ino).
9. Open the INO file with the Arduino IDE (with the Arduino Uno connected to your PC) and select Sketch -> Upload.
10. Wait for the sketch to be uploaded as indicated in the console window at the bottom of the Arduino IDE software.

### 5. Supported Controllers:

[See Compatibility.md for a detailed list of supported controllers.](COMPATIBILITY.md)

### 6. Software Usage
#### 6.1 Controller Profiles
The Adapter can store profiles for up to 10 unique controller models*. Controllers of the same make and model will share the profiles.
Each controller has 4 profiles associated with it. Should you plug in an 11th controller the system will replace the oldest controller you have set up.
Next oldest for the 12th and so on. This may cause you to have to set up older controllers again if you resume using them.

These can be accessed by pressing SELECT(COIN) + one of the four directions.
Thus if you wanted the first profile you would press SELECT + UP, the Second SELECT + RIGHT, and so on.
The adapter will remember what profile you were using and automatically switch to it when you use the controller again.

To change the key bindings of a profile. First change to the profile you want to setup.
While holding SELECT for at least 3 seconds, press buttons 1 - 6 in order. Then release SELECT to lock in the binding.
Finally release SELECT to lock in the binding.

*XBoxOne controllers appear to the system as the same make and model and thus use the same profiles even if they are different controllers.

###Shell

You can find the 3D printed shell by [avramce](https://twitter.com/avramce "avramce") clicking [here](https://www.thingiverse.com/thing:5170415 "here").

###How it looks in real life?

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

If you have found USB2DB15 useful, please consider funding it and making a donation so that we may continue our work and obtain additional controllers, equipment, and everything else required to keep this project alive.

[![paypal](https://www.paypalobjects.com/en_US/i/btn/btn_donateCC_LG.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=HYCK7DF7Q6XLY&source=url)


### Is this product available to buy? YES!

We sell USB2DB15 directly. You will receive a premium edition that is somewhat different from the open-source version but has the same features.
You can choose from a variety of shell color options, and you will get detailed instructions following your purchase.
There are two versions: the standard DB15 and the CS Ver., which is aimed for Jamma PCB consolizations. 




<a href="https://ko-fi.com/s/b6c3d01043">Access our shop by clicking here or on the images. </a>

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

### Where can I get more help, if I need it?

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

USB2DB15 or any of its maintainers are in no way associated with or endorsed by Nintendo, Sony, SNK, Microsoft, 8BitDo, Sega, Brook, Mad Catz, Nacon, Honcam, DaemonBite, Buffalo, Qanba, Razer, Retrobit, Toodles, Hori or any other company not implicitly indicated. All other brands or product names are the property of their respective holders.
