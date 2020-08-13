# USB2DB15-Supergun Adapter


Simple USB to DB15 for Supergun and Neogeo using the Arduino pro mini.



#### Technical information:

The USB2DB15 is Open Source device based on AVR that adapts a USB controller for use on Jamma PBCs via SUPERGUNs or a Neo Geo console.
In the most of cases, it will work as plug an play, on most of best of sellers controllers, but sometimes we will need to implement the new controller on code, so, be batient and concatact the devs and post issues on our GitHub. 

We are using a modded USB Host shield 2.0 lib for controllers IDs that aren't supported on the original. 

For the moment, there's 2 models for custom PCB, one if you want to use just a Arduino Pro Mini plus USB Host Shield mini, or a all in one designed by Frank_fjs that will cover both AVR and USB decoder IC. 

Of course, you still can just solder everything on a a breadboard and play nicely. 

You will need to know basics about Arduino. There's a bunch of videos explaining how to use the IDE, programming and all, but you are always free to ask for any question.


#### Instalation:

Download the USB Host Shield 2.0 and install, so download the code (RFUSB_to_DB15.ino) and programm it on your AVR.  

If you have a USB Host shield mini, you will need to CUT trace after 2k2 resistor close to USB VBUS pin, so it will feed by 5V (JP2 on the custom PCB), just solder it on the hole at same trace. 

https://imgur.com/vGgNsPl

Now, we devices that only work on 5V will turn on, like DS3.


The rest is plug and play. 

If you are not using any custom PCB, follow the schematic.


#### Supported Controllers:    

- Playstation 3 DS3
- Playstation 4 DS4
- Universal Brook PCB (PS4 Mode)
- Most of Hori RAP PS3 and PS4 controllers	
- Razer Panthera PS4
- Xbox One offical and some 3rd Party controllers
- RetroFreak CONTROLLER ADAPTER
- iBUFFALO SNES CLASSIC USB GAMEPAD





