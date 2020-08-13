# USB2DB15-Supergun Adapter


A simple USB to DB15 adapter for Supergun and Neo-Geo using an Atmega328P.


### 1. Technical information:

The USB2DB15 adapter is an Open Source device based on an AVR microcontroller that adapts a USB controller for use on Jamma PBCs via SUPERGUNs or a Neo-Geo console.
At present most common controllers are supported, such as ones that follow the PS3, PS4 and Xbox One protocol. Some controllers will not work and require additional coding implementation. Please be patient during these early stages and feel free to contact the developers with additional information, issues and requests for help via GitHub.


We are using a modified USB Host Shield 2.0 library for controller IDs that aren't natively supported. 


In regards to hardware there are currently two options:

- A CUSTOM PCB that accommodates and integrates an Arduino Pro Mini, USB Shield Mini and other relevant hardware.
- A STANDALONE PCB that incorporates all the required hardware in a standalone form.

Alternatively you can simply use a breadboard to tie it all together. 


A basic understanding of Arduino and its programming environment is required. There is plenty of useful information available at arduino.cc to help you get started.

### 2. What is need


* We are using an Atmega328P AVR. Any Arduino that uses this microcontroller such as the Mini Pro, Nano and Uno are suitable. The PCB accommodates an Arduino Mini Pro and USB Host Shield Mini;
* USB Host Shield Mini if using an Arduino Pro Mini / Nano. Full sized USB Host Shield for Uno;
* A FTDI programmer is required for the Arduino Pro Mini. This is not needed if using an Arduino Nano or Uno;
* USB2DB15 CUSTOM PCB or a breadboard to complete all the connections between the AVR and USB Host Shield;
* DB15 female connector;

<p align="center">
  <img width="480" height="600" src="https://i.imgur.com/voZmPqe.jpg">
 <br>
  <b>DB15 female connector, Arduino Nano and Usb Host Shield Mini</b><br>
<br>
	<br>
	<br>
	
<img width="680" height="480" src="https://i.imgur.com/CMmYJeC.png">
 <br>
  <b>Custom PCB for Arduino Pro Mini</b><br>

	
</p>
  		   

### 3. Installation:

Download the USB Host Shield 2.0 library and install it to the Arduino IDE. Open the INO file (RFUSB_to_DB15.ino) and program it to your AVR.  

If you have a USB Host Shield Mini, you will need to CUT the trace after the 2K2 resistor close to the USB VBUS pin, to feed it 5V (JP2 on the custom PCB). Solder it to the hole at same trace. 

<p align="center">
  <img width="460" height="300" src="https://i.imgur.com/vGgNsPl.png">
</p>


This enables devices that require +5V power to function correctly, as by default the USB Host Shield Mini only outputs 3.3V to the VBUS pin of the USB connector.


The rest is plug and play. 

If you are not using any custom PCB, follow the schematic.


#### 3.1. Arduino Uno + USB Host Shield:

1. Install Arduino IDE; 
2. Mount the USB Host Shield to the Arduino Uno correctly; 
3. Connect the Arduino Uno to your PC via its USB cable. 
4. In the Arduino IDE navigate to "Tools" -> "Board" -> "Arduino AVR Boards" and select "Arduino Uno";
5. In the Arduino IDE navigate to "Tools" -> "Port" and choose the Arduino Device; 
6.  Arduino IDE navigate to "Tools" -> "Programmer" and choose the "AVRISP MKII"; 
7. Obtain the library path (Arduino IDE -> File -> Preference-> Settings->Sketchbook location) and drop the required library  "USB_Host_Shield_20"  to the library directory; 
8. Obtain the INO firmware (RFUSB_to_DB15.ino);
9. Open the INO file with the Arduino IDE (with the Arduino Uno connected to your PC) and select Sketch-> Upload; 
10. Wait for the sketch to be uploaded as indicated in the console window at the bottom of the Arduino IDE software.


### 4. Supported Controllers:    

- Playstation 3 DS3
- Playstation 4 DS4
- Universal Brook PCB (PS4 Mode)
- Most of Hori RAP PS3 and PS4 controllers	
- Razer Panthera PS4
- Xbox One official and some 3rd Party controllers
- RetroFreak CONTROLLER ADAPTER
- iBUFFALO SNES CLASSIC USB GAMEPAD



### 5. Where can I get more help, if I need it?

We are based on the forum Arcade-Projects, so if you need any help, go there!

<p align="center">
<a href="https://www.arcade-projects.com/forums/index.php?thread/14205-open-source-usb-to-db15-for-supergun-adapter/">
  [<img width="460" height="300" src="https://i.imgur.com/7arl7Zn.png">]
</p>


	
    

