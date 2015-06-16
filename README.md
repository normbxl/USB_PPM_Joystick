# USB PPM Joystick
An Arduino adapter for RC remote control PPM signals to USB HID Joystick.

Many (semi) professionl remote controls can output their control signal via a connector. Usualy the signal is [PPM](https://en.wikipedia.org/wiki/Pulse-position_modulation) modulated.

If you want to train your steering skills before lifting off your RC helicopter you should consider using a RC simulator first. With this adapter-sketch you can use your original remote control on your computer.  

This sketch uses an slightly modified version of the vusb library in order to work with the Arduino framework.
The sketch is based on the UsbJoystick demo of Michel Gutlich (example is included in library).

###Requirements
Arduino IDE >1.0

###Installation

**You need to move the folder UsbJoystick to your sketchbook's libraries folder!**

##Hardware 
This has only been tested with boards based on the Atmega168 MCU (Arduino Duemilanove). It should work with Atmega328  boards (Arduino Uno) as well.

The electric scheme included does not show all the Arduino connections and additions. It just shows a kind of bare minimum required. The voltage regulator is only required because USB signals are 3.3 V based while the USB power is 5 V.
###Connections
Arduino D2 -> USB D+ (fixed)  
Arduino D3 -> USB D- (fixed)  
Arduino D4 -> 1.5 kOhm -> USB D- *(optional)*  
Arduino D8 -> PPM input *(changeable in code)*


