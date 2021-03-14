# USB2DB15 - Supergun Adapter [![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=HYCK7DF7Q6XLY&source=url)

A simple Atmega328P based USB to DB15 adapter designed for use with a Supergun or Neo-Geo system.


<p align="center">
  <img width="680" height="420" src="https://i.imgur.com/wUzNxtB.png">
 <br>
  <b>USB2DB15 with a Sega Astro City Mini Arcade Stick</b><br>
<br>
	</p>  	

### 1. Features:

- 1ms polling
- Open Source firmware
- 5 save slots for custom remapping types
- 10 save slots for different controllers
- Status LED
- Protection circuit for your belevod controllers
- DB15 pinout standard ready for Minigun and HAS superguns


### 2. Technical information:

The USB2DB15 adapter is an Open Sourced device based on an AVR microcontroller. It adapts a USB controller for use on a Jamma PCB via a SUPERGUN or a Neo-Geo console.
At present a variety of common controllers are supported; such as ones that follow the PS3/PS4 DualShock, PS5 DualSense and Xbox 360/One protocol. Due to the diversity of controllers available and differing standards there may be some controllers that do not work at all or that do not work as intended. Please be patient during these early stages and feel free to contact the developers with additional information and requests for help via GitHub.

We are using a modified USB Host Shield 2.0 library for controller IDs that aren't natively supported. 

In regards to hardware there are currently two options:

- A Custom PCB that accommodates and integrates an Arduino Pro Mini, USB Shield Mini and other relevant hardware.
- A Standalone PCB that incorporates all the required hardware in a standalone form (currently in development).

Alternatively you can simply use a breadboard to tie it all together. 

A basic understanding of Arduino and its programming environment is required. Visit arduino.cc to help you get started.

<p align="center">
  <img width="380" height="280" src="https://i.imgur.com/nXCXxz3.png">
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
  <img width="480" height="480" src="https://i.imgur.com/voZmPqe.jpg">
 <br>
  <b>DB15 female connector, Arduino Nano and Usb Host Shield Mini</b><br>
<br>
	</p>  	
    
  		   
If utilizing the Custom PCB in conjunction with an Arduino Pro Mini and Mini USB Shield, a BOM is available from the repositories section within the PCB folder. 

##### Jumper settings:

- SJ1: Bridge this jumper ONLY to disable 5V protection.
- SJ2: Bridge this jumper ONLY to enable 5V protection.
- JP2: 5V solder point (see information below regarding USB VBUS).

Due to variances of Mini Pro PCB's, A4 and A5 may be in different locations. Please pay attention to this.

<p align="center">
<img width="680" height="460" src="https://i.imgur.com/sYRVOBI.png">
 <br>
  <b>Custom PCB for Arduino Pro Mini 1.1</b><br>
</p>


### 4. Installation:

Download the  <a href="https://github.com/Raphfriend/USB_Host_Shield_2.0">USB Host Shield 2.0 library</a>  and install it to the Arduino IDE. Open the INO file (RFUSB_to_DB15.ino) and program it to your AVR.  

If you have a USB Host Shield Mini, you will need to CUT the trace after the 2K2 resistor near the USB VBUS pin. Solder a wire from JP2 of the CUSTOM PCB to the through-hole VIA labelled '5V' as depicted in the picture below.

<p align="center">
  <img width="460" height="200" src="https://i.imgur.com/vGgNsPl.png">
</p>

This enables devices that require +5V power to function correctly, as by default the USB Host Shield Mini only supplies 3.3V to the VBUS pin of the USB connector.

The remainder is plug and play. 

If you are not using a prefabricated PCB, please view the schematic for wiring information.

#### 4.1. Arduino Uno + USB Host Shield:

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

See COMPATIBILITY.md for a detailed list

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


### LEGAL

USB2DB15 or any of its maintainers are in no way associated with or endorsed by Nintendo, Sony, SNK, Microsoft, 8BitDo, Sega, Brook, Mad Catz, Nacon, Honcam, DaemonBite, Buffalo, Qanba, Razer, Retrobit, Toodles, Hori or any other company not implicitly indicated. All other brands or product names are the property of their respective holders.

### Donation

If this project has helped you, consider financing it and make a donation, so we can continue our work and acquire new controls to implement support.

[![paypal](https://www.paypalobjects.com/en_US/i/btn/btn_donateCC_LG.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=HYCK7DF7Q6XLY&source=url)

### Where can I get more help, if I need it?

Via the Arcade-Projects website forum or via Discord.

<p align="center">
<a href="https://www.arcade-projects.com/forums/index.php?thread/14205-open-source-usb-to-db15-for-supergun-adapter/">
  <img width="460" height="200" src="https://i.imgur.com/7arl7Zn.png">
</p>

<p align="center">
<a href="https://discord.gg/XKCjtpH">
  <img width="460" height="200" src="https://i.imgur.com/LZJjOUJ.png">
</p>
