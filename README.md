# USB2DB15-Supergun Adapter


Simple USB to DB15 for Supergun and Neogeo using Atmega328p.



### Technical information:

The USB2DB15 is Open Source device based on AVR that adapts a USB controller for use on Jamma PBCs via SUPERGUNs or a Neo Geo console.
In the most of cases, it will work as plug an play, on most of best of sellers controllers, but sometimes we will need to implement the new controller on code, so, be batient and concatact the devs and post issues on our GitHub. 

We are using a modded USB Host shield 2.0 lib for controllers IDs that aren't supported on the original. 

For the moment, there's 2 models for custom PCB, one if you want to use just a Arduino Pro Mini plus USB Host Shield mini, or a all in one designed by Frank_fjs that will cover both AVR and USB decoder IC. 

Of course, you still can just solder everything on a a breadboard and play nicely. 

You will need to know basics about Arduino. There's a bunch of videos explaining how to use the IDE, programming and all, but you are always free to ask for any question.

### What is need


* We are using Atmega328p AVR, but you can get any Arduino that uses this microcontroller, like Mini Pro, Nano, Uno, etc.; 
* Usb Host Shield Mino, or, if you are using a Uno like Arduino, you will need the a full shield; 
* A programmer like FDTI, if your Arduino has already a bootloader (Nano, UNO), you won't need it; 
* USB2DB15 Custom PCB or any breadboard for solder the AVR and USB Host Shield.
* DB15 female connector.



https://i.imgur.com/voZmPqe.jpg








### Installation:

Download the USB Host Shield 2.0 and install, so download the code (RFUSB_to_DB15.ino) and programm it on your AVR.  

If you have a USB Host shield mini, you will need to CUT trace after 2k2 resistor close to USB VBUS pin, so it will feed by 5V (JP2 on the custom PCB), just solder it on the hole at same trace. 

https://i.imgur.com/vGgNsPl.png

Now, we devices that only work on 5V will turn on, like DS3.


The rest is plug and play. 

If you are not using any custom PCB, follow the schematic.


####Arduino Uno + USB Host Shield:

1. Install Arduino IDE
2. Mount the USB Host Shield on Arduino Uno correctly
3. Plug the Arduino UNO with the Shield in the PC USB and in the Arduino IDE navigate to "Tools" -> "Ports"  and choose the Arduino Device.
4.  Arduino IDE navigate to "Tools" -> "Programmer"  and choose the "AVRISP MKII".
5. Get library path (Arduino IDE -> File -> Preference-> Settings->Sketchbook location) and drop the required library  "USB_Host_Shield_20"  to the library directory 
6. Get the ino (firmware from): _____________________________
7. Arduino IDE open the ino file (with the Arduino attached to the PC ) and Sketch-> Upload
8. Look for "Done Uploading" on the Arduino IDE.



### Supported Controllers:    

- Playstation 3 DS3
- Playstation 4 DS4
- Universal Brook PCB (PS4 Mode)
- Most of Hori RAP PS3 and PS4 controllers	
- Razer Panthera PS4
- Xbox One official and some 3rd Party controllers
- RetroFreak CONTROLLER ADAPTER
- iBUFFALO SNES CLASSIC USB GAMEPAD





### Where can I get more help, if I need it?


	We are based on the forum Arcade-Projects, so if you need any help, go on the this topic:
	https://www.arcade-projects.com/forums/index.php?thread/14205-open-source-usb-to-db15-for-supergun-adapter/





