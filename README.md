# USB2DB15 - Supergun Adapter

A simple Atmega328P based USB to DB15 adapter designed for use with a Supergun or Neo-Geo system.

### 1. Technical information:

The USB2DB15 adapter is an Open Sourced device based on an AVR microcontroller. It adapts a USB controller for use on a Jamma PCB via a SUPERGUN or a Neo-Geo console.
At present a variety of common controllers are supported; such as ones that follow the PS3, PS4 and Xbox One protocol. Due to the diversity of controllers available and differing standards there may be some controllers that do not work at all or that do not work as intended. Please be patient during these early stages and feel free to contact the developers with additional information and requests for help via GitHub.

We are using a modified USB Host Shield 2.0 library for controller IDs that aren't natively supported. 

In regards to hardware there are currently two options:

- A Custom PCB that accommodates and integrates an Arduino Pro Mini, USB Shield Mini and other relevant hardware.
- A Standalone PCB that incorporates all the required hardware in a standalone form (currently in development).

Alternatively you can simply use a breadboard to tie it all together. 

A basic understanding of Arduino and its programming environment is required. Visit arduino.cc to help you get started.

### 2. Required hardware:

* We are using an Atmega328P AVR. Any Arduino that utilizes this microcontroller such as the Mini Pro, Nano and Uno are suitable. The Custom PCB accommodates an Arduino Mini Pro and USB Host Shield Mini.
* USB Host Shield Mini if using an Arduino Pro Mini / Nano. Full sized USB Host Shield for Uno.
* An FTDI programmer is required for the Arduino Pro Mini. This is not required if using an Arduino Nano or Uno.
* USB2DB15 Custom PCB or a breadboard to complete all the connections between the AVR and USB Host Shield.
* DB15 female connector.

<p align="center">
  <img width="480" height="600" src="https://i.imgur.com/voZmPqe.jpg">
 <br>
  <b>DB15 female connector, Arduino Nano and Usb Host Shield Mini</b><br>
<br>
	</p>  	
    
  		   
If utilizing the Custom PCB in conjunction with an Arduino Pro Mini and Mini USB Shield, a BOM is available from the repositories section within the PCB folder. 

##### Jumper settings:

- JP1: Select 3.3V or 5V power input to the Arduino Pro Mini.
- SJ1: Bridge this jumper to disable 5V protection.
- SJ1+SJ2+SJ3: Bridge all these jumpers to enable 5V protection.
- JP2: 5V solder point (see information below regarding USB VBUS).

Due to variances of Mini Pro PCB's, A4 and A5 may be in different locations. Please pay attention to this.

<p align="center">
<img width="680" height="480" src="https://i.imgur.com/n5mkrv0.png">
 <br>
  <b>Custom PCB for Arduino Pro Mini</b><br>
</p>



### 3. Installation:

Download the  <a href="https://github.com/Raphfriend/USB_Host_Shield_2.0">USB Host Shield 2.0 library</a>  and install it to the Arduino IDE. Open the INO file (RFUSB_to_DB15.ino) and program it to your AVR.  

If you have a USB Host Shield Mini, you will need to CUT the trace after the 2K2 resistor near the USB VBUS pin. Solder a wire from JP2 of the CUSTOM PCB to the highlighted red hole depicted in the picture below.

<p align="center">
  <img width="460" height="300" src="https://i.imgur.com/vGgNsPl.png">
</p>

This enables devices that require +5V power to function correctly, as by default the USB Host Shield Mini only supplies 3.3V to the VBUS pin of the USB connector.

The remainder is plug and play. 

If you are not using a prefabricated PCB, please view the schematic for wiring information.

#### 3.1. Arduino Uno + USB Host Shield:

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

### 4. Supported Controllers:    

- Playstation 3 DS3
- Playstation 4 DS4
- Universal Brook PCB (PS4 Mode)
- Most Hori RAP PS3 and PS4 controllers	
- Razer Panthera PS4
- Xbox One official and some 3rd Party controllers
- RetroFreak CONTROLLER ADAPTER
- iBUFFALO SNES CLASSIC USB GAMEPAD
- MadCatz PS3 compatible controllers
- Retrobit SEGA Saturn® 8-Button Arcade Pad - 2.4 GHz Wireless

### 5. Where can I get more help, if I need it?

Via the Arcade-Projects website forum or via Discord.

<p align="center">
<a href="https://www.arcade-projects.com/forums/index.php?thread/14205-open-source-usb-to-db15-for-supergun-adapter/">
  <img width="460" height="300" src="https://i.imgur.com/7arl7Zn.png">
</p>

<p align="center">
<a href="https://discord.gg/XKCjtpH">
  <img width="460" height="300" src="https://i.imgur.com/LZJjOUJ.png">
</p>
